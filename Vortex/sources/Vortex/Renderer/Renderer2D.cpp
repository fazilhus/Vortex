#include <vtpch.hpp>
#include "Vortex/Renderer/Renderer2D.hpp"

namespace Vortex {

	static Renderer2DStorage s_data;

	void Renderer2D::Init() {
		s_data.quadVao = VertexArray::Create();

		s_data.quadVbo = VertexBuffer::Create(s_data.maxVert * sizeof(QuadVertex));
		s_data.quadVbo->SetLayout({ 
			{"a_position", ShaderDataType::Float3},
			{"a_color", ShaderDataType::Float4},
			{"a_texPos", ShaderDataType::Float2},
			{"a_texInd", ShaderDataType::Float},
			{"a_tilingFactor", ShaderDataType::Float},
			{"a_entityID", ShaderDataType::Int }
		});
		s_data.quadVao->AddVertexBuffer(s_data.quadVbo);

		s_data.quadVertexBufferBase = new QuadVertex[s_data.maxVert];

		const auto ind = new uint32[s_data.maxInd];
		uint32 offset = 0;

		for (uint32 i = 0; i < s_data.maxInd; i += 6) {
			ind[i + 0] = offset + 0;
			ind[i + 1] = offset + 1;
			ind[i + 2] = offset + 2;

			ind[i + 3] = offset + 2;
			ind[i + 4] = offset + 3;
			ind[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> ibo = IndexBuffer::Create(ind, s_data.maxInd);
		s_data.quadVao->AddIndexBuffer(ibo);

		delete ind;

		s_data.texture = Texture2D::Create(1, 1);
		uint32 texData = 0xffffffff;
		s_data.texture->SetData(&texData, sizeof(uint32));

		int32 samplers[s_data.maxTextureSLots];
		for (uint32 i = 0; i < s_data.maxTextureSLots; ++i) {
			samplers[i] = i;
		}

		s_data.shader = Shader::Create("assets/shaders/shader.glsl");
		s_data.shader->Bind();
		s_data.shader->SetIntArray("u_textures", samplers, s_data.maxTextureSLots);

		s_data.texSlots[0] = s_data.texture;

		s_data.quadVertexPos[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_data.quadVertexPos[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_data.quadVertexPos[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_data.quadVertexPos[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DStorage::CameraData), 0);

		VT_CORE_INFO("Renderer2D is initialized");
	}

	void Renderer2D::Shutdown() {
		VT_PROFILE_FUNC();
		delete s_data.quadVertexBufferBase;
		VT_CORE_INFO("Renderer2D is terminated");
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4 transform) {
		VT_PROFILE_FUNC();

		s_data.CameraBuffer.viewproj = camera.GetProjection() * glm::inverse(transform);
		s_data.CameraUniformBuffer->SetData(&s_data.CameraBuffer, sizeof(Renderer2DStorage::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const OrthoCamera& camera) {
		VT_PROFILE_FUNC();

		s_data.shader->Bind();
		s_data.shader->SetMat4("u_viewproj", camera.GetViewProjMat());

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera) {
		VT_PROFILE_FUNC();

		s_data.CameraBuffer.viewproj = camera.GetViewProjection();
		s_data.CameraUniformBuffer->SetData(&s_data.CameraBuffer, sizeof(Renderer2DStorage::CameraData));

		StartBatch();
	}

	void Renderer2D::EndScene() {
		VT_PROFILE_FUNC();
		Flush();
	}

	void Renderer2D::Flush() {
		if(s_data.quadIndCount == 0) {
			return;
		}

		const auto dataSize = static_cast<uint32>(reinterpret_cast<uint8*>(s_data.quadVertexBufferPtr) - reinterpret_cast<uint8*>(s_data.quadVertexBufferBase));
		s_data.quadVbo->SetData(s_data.quadVertexBufferBase, dataSize);

		for (uint32 i = 0; i < s_data.texSlotInd; ++i) {
			VT_CORE_TRACE("Bind texture {0} in slot {1}", s_data.texSlots[i]->GetPath(), i);
			s_data.texSlots[i]->Bind(i);
		}
		s_data.shader->Bind();
		Render::DrawIndexed(s_data.quadVao, s_data.quadIndCount);
		s_data.stats.drawcallsCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID) {
		VT_PROFILE_FUNC();

		constexpr std::size_t vertcount{ 4 };
		constexpr float texInd{ 0.0f };
		constexpr glm::vec2 texCoord[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		constexpr float tiling = 1.0f;

		if (s_data.quadIndCount >= Renderer2DStorage::maxInd) {
			NextBatch();
		}

		for (size_t i = 0; i < vertcount; ++i) {
			auto pos = transform * s_data.quadVertexPos[i];
			s_data.quadVertexBufferPtr->pos = pos;
			s_data.quadVertexBufferPtr->color = color;
			s_data.quadVertexBufferPtr->texPos = texCoord[i];
			s_data.quadVertexBufferPtr->texInd = texInd;
			s_data.quadVertexBufferPtr->tilingFactor = tiling;
			s_data.quadVertexBufferPtr->entityID = entityID;
			s_data.quadVertexBufferPtr++;
		}

		s_data.quadIndCount += 6;

		s_data.stats.quadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID) {
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({pos.x, pos.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {
		VT_PROFILE_FUNC();

		if (s_data.quadIndCount >= Renderer2DStorage::maxInd) {
			NextBatch();
		}

		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture,
		float tilingFactor, const glm::vec4 tintColor) {
		DrawQuad({ pos.x, pos.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture,
		float tilingFactor, const glm::vec4 tintColor) {
		VT_PROFILE_FUNC();

		if (s_data.quadIndCount >= Renderer2DStorage::maxInd) {
			NextBatch();
		}

		float texIndex = 0.0f;
		for (uint32 i = 1; i < s_data.texSlotInd; ++i) {
			if (*s_data.texSlots[i].get() == *texture.get()) {
				texIndex = static_cast<float>(i);
				VT_CORE_TRACE("existing texture {0} in index {1}", texture->GetPath(), texIndex);
				break;
			}
		}
		if (texIndex == 0.0f) {
			if (s_data.texSlotInd >= Renderer2DStorage::maxTextureSLots) {
				NextBatch();
			}

			texIndex = static_cast<float>(s_data.texSlotInd);
			s_data.texSlots[s_data.texSlotInd] = texture;
			VT_CORE_TRACE("new texture {0} in index {1}", texture->GetPath(), texIndex);
			s_data.texSlotInd++;
		}

		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, float rot) {
		DrawRotatedQuad({pos.x, pos.y, 0.0f}, size, color, rot);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float rot) {
		VT_PROFILE_FUNC();

		if (s_data.quadIndCount >= Renderer2DStorage::maxInd) {
			NextBatch();
		}

		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rot), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture,
		float rot, float tilingFactor, const glm::vec4 tintColor) {
		DrawRotatedQuad({ pos.x, pos.y, 0.0f }, size, texture, rot, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture,
		float rot, float tilingFactor, const glm::vec4 tintColor) {
		VT_PROFILE_FUNC();

		if (s_data.quadIndCount >= Renderer2DStorage::maxInd) {
			NextBatch();
		}

		float texIndex = 0.0f;
		for (uint32 i = 1; i < s_data.texSlotInd; ++i) {
			if (*s_data.texSlots[i].get() == *texture.get()) {
				texIndex = static_cast<float>(i);
				VT_CORE_TRACE("existing texture {0} in index {1}", texture->GetPath(), texIndex);
				break;
			}
		}

		if (texIndex == 0.0f) {
			if (s_data.texSlotInd >= Renderer2DStorage::maxTextureSLots) {
				NextBatch();
			}

			texIndex = static_cast<float>(s_data.texSlotInd);
			s_data.texSlots[s_data.texSlotInd] = texture;
			VT_CORE_TRACE("new texture {0} in index {1}", texture->GetPath(), texIndex);
			s_data.texSlotInd++;
		}

		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rot), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	/*void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteComponent& sprite, int entityID) {
		DrawQuad(transform, sprite.Color, entityID);
	}*/

	void Renderer2D::ResetStats() {
		memset(&s_data.stats, 0, sizeof(RendererStatisics));
	}

	RendererStatisics Renderer2D::GetStats() {
		return s_data.stats;
	}

	void Renderer2D::StartBatch() {
		s_data.quadIndCount = 0;
		s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;
		s_data.texSlotInd = 1;
	}

	void Renderer2D::NextBatch() {
		Flush();
		StartBatch();
	}

}

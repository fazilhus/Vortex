#include "vtpch.hpp"
#include "Vortex/Renderer/Renderer2D.hpp"

namespace Vortex {

	static Renderer2DStorage s_data;

	void Renderer2D::Init() {
		s_data.quadVao = VertexArray::Create();

		s_data.quadVbo = VertexBuffer::Create(s_data.maxVert * sizeof(QuadVertex));
		s_data.quadVbo->SetLayout({ 
			{"a_position", ShaderDataType::Float3},
			{"a_color", ShaderDataType::Float4},
			{"a_texPos", ShaderDataType::Float2}
		});
		s_data.quadVao->AddVertexBuffer(s_data.quadVbo);

		s_data.quadVertexBufferBase = new QuadVertex[s_data.maxVert];

		uint32* ind = new uint32[s_data.maxInd];
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

		s_data.shader = Shader::Create("res/shaders/shader.glsl");
		s_data.shader->Bind();
		s_data.shader->SetInt("u_texture", 0);
		VT_CORE_INFO("Renderer2D is initialized");
	}

	void Renderer2D::Shutdown() {
		VT_PROFILE_FUNC();
		delete s_data.quadVertexBufferBase;
		VT_CORE_INFO("Renderer2D is terminated");
	}

	void Renderer2D::BeginScene(const OrthoCamera& camera) {
		VT_PROFILE_FUNC();
		s_data.shader->Bind();
		s_data.shader->SetMat4("u_viewproj", camera.GetViewProjMat());

		s_data.quadIndCount = 0;
		s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;
	}

	void Renderer2D::EndScene() {
		VT_PROFILE_FUNC();
		uint32 dataSize = reinterpret_cast<uint8*>(s_data.quadVertexBufferPtr) - reinterpret_cast<uint8*>(s_data.quadVertexBufferBase);
		s_data.quadVbo->SetData(s_data.quadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush() {
		Render::DrawIndexed(s_data.quadVao, s_data.quadIndCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({pos.x, pos.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {
		VT_PROFILE_FUNC();
		s_data.quadVertexBufferPtr->pos = pos;
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texPos = {0.0f, 0.0f};
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->pos = { pos.x + size.x, pos.y, pos.z };
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texPos = { 1.0f, 0.0f };
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->pos = { pos.x + size.x, pos.y + size.y, pos.z };
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texPos = { 1.0f, 1.0f };
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->pos = { pos.x, pos.y + size.y, pos.z };
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texPos = { 0.0f, 1.0f };
		s_data.quadVertexBufferPtr++;

		s_data.quadIndCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Vortex::Texture2D>& texture,
		float tilingFactor, const glm::vec4 tintColor) {
		DrawQuad({ pos.x, pos.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Vortex::Texture2D>& texture,
		float tilingFactor, const glm::vec4 tintColor) {
		VT_PROFILE_FUNC();
		s_data.shader->SetFloat4("u_color", tintColor);
		s_data.shader->SetFloat("u_tilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data.shader->SetMat4("u_transform", transform);

		s_data.quadVao->Bind();
		Render::DrawIndexed(s_data.quadVao);
		VT_CORE_TRACE("Draw call: DrawQuad ({0} {1}, {2} {3}) texture", pos.x, pos.x + size.x, pos.y, pos.y + size.y);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, float rot) {
		DrawRotatedQuad({pos.x, pos.y, 0.0f}, size, color, rot);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float rot) {
		VT_PROFILE_FUNC();
		s_data.shader->SetFloat4("u_color", color);
		s_data.shader->SetFloat("u_tilingFactor", 1.0f);
		s_data.texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
		* glm::rotate(glm::mat4(1.0f), rot, {0.0f, 0.0f, 1.0f})
		* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data.shader->SetMat4("u_transform", transform);

		s_data.quadVao->Bind();
		Render::DrawIndexed(s_data.quadVao);
		VT_CORE_TRACE("Draw call: DrawRotatedQuad ({0} {1}, {2} {3}) color", pos.x, pos.x + size.x, pos.y, pos.y + size.y);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Vortex::Texture2D>& texture,
		float rot, float tilingFactor, const glm::vec4 tintColor) {
		DrawRotatedQuad({ pos.x, pos.y, 0.0f }, size, texture, rot, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Vortex::Texture2D>& texture,
		float rot, float tilingFactor, const glm::vec4 tintColor) {
		VT_PROFILE_FUNC();
		s_data.shader->SetFloat4("u_color", tintColor);
		s_data.shader->SetFloat("u_tilingFactor", tilingFactor);
		s_data.texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), rot, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data.shader->SetMat4("u_transform", transform);

		s_data.quadVao->Bind();
		Render::DrawIndexed(s_data.quadVao);
		VT_CORE_TRACE("Draw call: DrawRotatedQuad ({0} {1}, {2} {3}) texture", pos.x, pos.x + size.x, pos.y, pos.y + size.y);
	}
}

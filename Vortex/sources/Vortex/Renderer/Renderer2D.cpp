#include "vtpch.hpp"
#include "Vortex/Renderer/Renderer2D.hpp"

namespace Vortex {

	struct Renderer2DStorage {
		Ref<VertexArray> vao;
		Ref<Shader> shader;
		Ref<Texture2D> texture;
	};

	static Scope<Renderer2DStorage> s_data;

	void Renderer2D::Init() {
		s_data = CreateScope<Renderer2DStorage>();
		s_data->vao = VertexArray::Create();

		float vertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> vbo = VertexBuffer::Create(vertices, sizeof(vertices));
		vbo->SetLayout({ 
			{"a_position", ShaderDataType::Float3},
			{"a_texPos", ShaderDataType::Float2}
		});

		s_data->vao->AddVertexBuffer(vbo);

		uint4 indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> ibo = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint4));

		s_data->vao->AddIndexBuffer(ibo);

		s_data->texture = Texture2D::Create(1, 1);
		uint4 texData = 0xffffffff;
		s_data->texture->SetData(&texData, sizeof(uint4));

		s_data->shader = Shader::Create("res/shaders/shader.glsl");
		s_data->shader->Bind();
		s_data->shader->SetInt("u_texture", 0);
		VT_CORE_INFO("Renderer2D is initialized");
	}

	void Renderer2D::Shutdown() {
		s_data.release();
		VT_CORE_INFO("Renderer2D is terminated");
	}

	void Renderer2D::BeginScene(const OrthoCamera& camera) {
		VT_PROFILE_FUNC();
		s_data->shader->Bind();
		s_data->shader->SetMat4("u_viewproj", camera.GetViewProjMat());
	}

	void Renderer2D::EndScene() {
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({pos.x, pos.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {
		VT_PROFILE_FUNC();
		s_data->shader->SetFloat4("u_color", color);
		s_data->shader->SetFloat("u_tilingFactor", 1.0f);
		s_data->texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->shader->SetMat4("u_transform", transform);

		s_data->vao->Bind();
		Render::DrawIndexed(s_data->vao);
		VT_CORE_TRACE("Draw call: DrawQuad ({0} {1}, {2} {3}) color", pos.x, pos.x + size.x, pos.y, pos.y + size.y);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Vortex::Texture2D>& texture,
		float tilingFactor, const glm::vec4 tintColor) {
		DrawQuad({ pos.x, pos.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Vortex::Texture2D>& texture,
		float tilingFactor, const glm::vec4 tintColor) {
		VT_PROFILE_FUNC();
		s_data->shader->SetFloat4("u_color", tintColor);
		s_data->shader->SetFloat("u_tilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->shader->SetMat4("u_transform", transform);

		s_data->vao->Bind();
		Render::DrawIndexed(s_data->vao);
		VT_CORE_TRACE("Draw call: DrawQuad ({0} {1}, {2} {3}) texture", pos.x, pos.x + size.x, pos.y, pos.y + size.y);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, float rot) {
		DrawRotatedQuad({pos.x, pos.y, 0.0f}, size, color, rot);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float rot) {
		VT_PROFILE_FUNC();
		s_data->shader->SetFloat4("u_color", color);
		s_data->shader->SetFloat("u_tilingFactor", 1.0f);
		s_data->texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
		* glm::rotate(glm::mat4(1.0f), rot, {0.0f, 0.0f, 1.0f})
		* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->shader->SetMat4("u_transform", transform);

		s_data->vao->Bind();
		Render::DrawIndexed(s_data->vao);
		VT_CORE_TRACE("Draw call: DrawRotatedQuad ({0} {1}, {2} {3}) color", pos.x, pos.x + size.x, pos.y, pos.y + size.y);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Vortex::Texture2D>& texture,
		float rot, float tilingFactor, const glm::vec4 tintColor) {
		DrawRotatedQuad({ pos.x, pos.y, 0.0f }, size, texture, rot, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Vortex::Texture2D>& texture,
		float rot, float tilingFactor, const glm::vec4 tintColor) {
		VT_PROFILE_FUNC();
		s_data->shader->SetFloat4("u_color", tintColor);
		s_data->shader->SetFloat("u_tilingFactor", tilingFactor);
		s_data->texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), rot, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->shader->SetMat4("u_transform", transform);

		s_data->vao->Bind();
		Render::DrawIndexed(s_data->vao);
		VT_CORE_TRACE("Draw call: DrawRotatedQuad ({0} {1}, {2} {3}) texture", pos.x, pos.x + size.x, pos.y, pos.y + size.y);
	}
}

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

		uint indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> ibo = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint));

		s_data->vao->AddIndexBuffer(ibo);

		s_data->texture = Texture2D::Create(1, 1);
		uint texData = 0xffffffff;
		s_data->texture->SetData(&texData, sizeof(uint));

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
		s_data->shader->Bind();
		s_data->shader->SetMat4("u_viewproj", camera.GetViewProjMat());
	}

	void Renderer2D::EndScene() {
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({pos.x, pos.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {
		s_data->shader->SetFloat4("u_color", color);
		s_data->texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->shader->SetMat4("u_transform", transform);

		s_data->vao->Bind();
		Render::DrawIndexed(s_data->vao);
		VT_CORE_TRACE("Draw call: DrawQuad ({0} {1}, {2} {3}) color", pos.x, pos.x + size.x, pos.y, pos.y + size.y);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Vortex::Texture2D>& texture) {
		DrawQuad({ pos.x, pos.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Vortex::Texture2D>& texture) {
		s_data->shader->SetFloat4("u_color", glm::vec4(1.0f));
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->shader->SetMat4("u_transform", transform);

		s_data->vao->Bind();
		Render::DrawIndexed(s_data->vao);
		VT_CORE_TRACE("Draw call: DrawQuad ({0} {1}, {2} {3}) texture", pos.x, pos.x + size.x, pos.y, pos.y + size.y);
	}
}

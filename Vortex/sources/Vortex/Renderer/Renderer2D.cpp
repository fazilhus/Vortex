#include "vtpch.hpp"
#include "Vortex/Renderer/Renderer2D.hpp"

#include "Platforms/OpenGL/OpenGLShader.hpp"

namespace Vortex {

	struct Renderer2DStorage {
		Ref<VertexArray> vao;
		Ref<Shader> shader;
	};

	static Scope<Renderer2DStorage> s_data;

	void Renderer2D::Init() {
		s_data = CreateScope<Renderer2DStorage>();
		s_data->vao = VertexArray::Create();

		float vertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> vbo = VertexBuffer::Create(vertices, sizeof(vertices));
		vbo->SetLayout({ 
			{"a_position", ShaderDataType::Float3}
		});

		s_data->vao->AddVertexBuffer(vbo);

		uint indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> ibo = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint));

		s_data->vao->AddIndexBuffer(ibo);

		s_data->shader = Shader::Create("res/shaders/flatcolorShader.glsl");
	}

	void Renderer2D::Shutdown() {
		s_data.release();
	}

	void Renderer2D::BeginScene(const OrthoCamera& camera) {
		std::dynamic_pointer_cast<OpenGLShader>(s_data->shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_data->shader)->UploadUniformMat4("u_viewproj", camera.GetViewProjMat());
		std::dynamic_pointer_cast<OpenGLShader>(s_data->shader)->UploadUniformMat4("u_transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene() {
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({pos.x, pos.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {
		std::dynamic_pointer_cast<OpenGLShader>(s_data->shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_data->shader)->UploadUniformFloat4("u_color", color);

		s_data->vao->Bind();
		Render::DrawIndexed(s_data->vao);
	}

}

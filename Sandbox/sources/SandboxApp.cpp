#include <Vortex.hpp>
#include "Vortex/Core/EntryPoint.hpp"

#include "Platforms/OpenGL/OpenGLShader.hpp"

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.hpp"


class SimpleLayer : public Vortex::Layer {
private:
	Vortex::ShaderLib m_shaderLib;

	Vortex::Ref<Vortex::VertexArray> m_vao1;
	Vortex::Ref<Vortex::VertexArray> m_vao2;

	Vortex::Ref<Vortex::Texture2D> m_tex;

	Vortex::OrthoCameraController m_cameraController;

	glm::vec3 camPos{ 0.0f, 0.0f, 0.0f };
	float camPosVel = 1.0f;
	float camRot{ 0.0f };
	float camRotVel = 180.0f;

public:
	SimpleLayer()
		: Layer("Simple"), m_cameraController(1600.0f / 900.0f, true) {}
	~SimpleLayer() = default;

	virtual void OnAttach() override {
		{
			m_vao2 = Vortex::VertexArray::Create();

			float vertices[4 * 7] = {
				-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				 0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f, 1.0f,
				 0.5f,  0.5f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f,
				-0.5f,  0.5f, 0.0f,	0.0f, 0.0f, 0.0f, 1.0f
			};

			std::shared_ptr<Vortex::VertexBuffer> vbo;
			vbo = Vortex::VertexBuffer::Create(vertices, sizeof(vertices));
			Vortex::BufferLayout layout{
				{"a_position", Vortex::ShaderDataType::Float3},
				{"a_color", Vortex::ShaderDataType::Float4}
			};
			vbo->SetLayout(layout);
			m_vao2->AddVertexBuffer(vbo);

			uint indices[6] = { 0, 1, 2, 2, 3, 0 };
			std::shared_ptr<Vortex::IndexBuffer> ibo;
			ibo = Vortex::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint));
			m_vao2->AddIndexBuffer(ibo);

			auto shader = m_shaderLib.Load("res/shaders/boxShader.glsl");
			VT_CL_TRACE("box shader created");
		}

		{
			m_vao1 = Vortex::VertexArray::Create();

			float vertices[4 * 5] = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
			};

			std::shared_ptr<Vortex::VertexBuffer> vbo;
			vbo = Vortex::VertexBuffer::Create(vertices, sizeof(vertices));
			Vortex::BufferLayout layout{
				{"a_position", Vortex::ShaderDataType::Float3},
				{"a_texPos", Vortex::ShaderDataType::Float2}
			};
			vbo->SetLayout(layout);
			m_vao1->AddVertexBuffer(vbo);

			uint indices[6] = { 0, 1, 2, 2, 3, 0 };
			std::shared_ptr<Vortex::IndexBuffer> ibo;
			ibo = Vortex::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint));
			m_vao1->AddIndexBuffer(ibo);

			auto texShader = m_shaderLib.Load("res/shaders/textureShader.glsl");
			VT_CL_TRACE("texture shader created");

			m_tex = Vortex::Texture2D::Create("res/textures/img3.png");

			std::dynamic_pointer_cast<Vortex::OpenGLShader>(texShader)->Bind();
			std::dynamic_pointer_cast<Vortex::OpenGLShader>(texShader)->UploadUniformInt("u_texture", 0);
		}
	}

	virtual void OnDetach() override {
	}

	virtual void OnUpdate(Vortex::Timestep ts) override {
		m_cameraController.OnUpdate(ts);

		Vortex::Render::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Vortex::Render::Clear();

		Vortex::Renderer::BeginScene(m_cameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.15f));

		auto shader = m_shaderLib.Get("boxShader");
		auto textureShader = m_shaderLib.Get("textureShader");

		for (int y = -4; y < 5; ++y) {
			for (int x = -4; x < 5; ++x) {
				glm::vec3 pos(x * 0.16f, y * 0.16f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Vortex::Renderer::Submit(shader, m_vao2, transform);
			}
		}

		m_tex->Bind(0);
		Vortex::Renderer::Submit(textureShader, m_vao1);

		Vortex::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::DragFloat3("Pos", &camPos.x, 0.005f, -1.0f, 1.0f);
		ImGui::SameLine();
		if (ImGui::Button("Reset pos")) { camPos = glm::vec3(0.0f); }
		ImGui::DragFloat("Rot", &camRot, 0.1f, -90.0f, 90.0f);
		ImGui::SameLine();
		if (ImGui::Button("Reset rot")) { camRot = 0.0f; }
	}

	virtual void OnEvent(Vortex::Event& e) override {
		m_cameraController.OnEvent(e);
	}
};

class Sandbox : public Vortex::Application {
public:
	Sandbox() {
		//Vortex::Ref<Vortex::Layer> sl = Vortex::CreateRef<SimpleLayer>();
		//PushLayer(sl);
		Vortex::Ref<Vortex::Layer> sl2d = Vortex::CreateRef<Sandbox2D>();
		PushLayer(sl2d);
	}
	~Sandbox() {}
};

Vortex::Application* Vortex::CreateApplication() {
	return new Sandbox();
}
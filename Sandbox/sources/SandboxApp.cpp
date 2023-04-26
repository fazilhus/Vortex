#include <Vortex.hpp>

#include "Platforms/OpenGL/OpenGLShader.hpp"

#include <memory>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class SimpleLayer : public Vortex::Layer {
private:
	std::shared_ptr<Vortex::VertexArray> m_vao1;
	std::shared_ptr<Vortex::Shader> m_shader1;
	std::shared_ptr<Vortex::VertexArray> m_vao2;
	std::shared_ptr<Vortex::Shader> m_shader2;

	Vortex::OrthoCamera m_camera;

	glm::vec3 camPos{ 0.0f, 0.0f, 0.0f };
	float camPosVel = 1.0f;
	float camRot{ 0.0f };
	float camRotVel = 180.0f;

	glm::vec4 scColor = { 0.2f, 0.3f, 0.8f, 1.0f };

public:
	SimpleLayer()
		: Layer("Simple"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f) {};
	~SimpleLayer() {};

	virtual void OnAttach() override {
		{
			m_vao1.reset(Vortex::VertexArray::Create());

			float vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
				 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
			};

			std::shared_ptr<Vortex::VertexBuffer> vbo;
			vbo.reset(Vortex::VertexBuffer::Create(vertices, sizeof(vertices)));
			Vortex::BufferLayout layout{ {"a_position", Vortex::ShaderDataType::Float3}, {"a_color", Vortex::ShaderDataType::Float4} };
			vbo->SetLayout(layout);
			m_vao1->AddVertexBuffer(vbo);

			uint indices[3] = { 0, 1, 2 };
			std::shared_ptr<Vortex::IndexBuffer> ibo;
			ibo.reset(Vortex::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint)));
			m_vao1->AddIndexBuffer(ibo);

			std::string vertexSrc = R"(
				#version 460 core
				layout(location = 0) in vec3 a_position;
				layout(location = 1) in vec4 a_color;
				uniform mat4 u_viewproj;
				uniform mat4 u_transform;
				out vec3 v_position;
				out vec4 v_color;
				void main() {
					v_position = a_position;
					v_color = a_color;
					gl_Position = u_viewproj * u_transform * vec4(a_position, 1.0);	
				}
			)";

			std::string fragmentSrc = R"(
				#version 460 core
				in vec3 v_position;
				in vec4 v_color;
				layout(location = 0) out vec4 color;
				void main() {
					color = vec4(v_position * 0.5 + 0.5, 1.0);
					color = v_color;
				}
			)";

			m_shader1.reset(Vortex::Shader::Create(vertexSrc.c_str(), fragmentSrc.c_str()));
		}

		{
			m_vao2.reset(Vortex::VertexArray::Create());

			float vertices[3 * 4] = {
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.5f,  0.5f, 0.0f,
				-0.5f,  0.5f, 0.0f
			};

			std::shared_ptr<Vortex::VertexBuffer> vbo;
			vbo.reset(Vortex::VertexBuffer::Create(vertices, sizeof(vertices)));
			Vortex::BufferLayout layout{ {"a_position", Vortex::ShaderDataType::Float3} };
			vbo->SetLayout(layout);
			m_vao2->AddVertexBuffer(vbo);

			uint indices[6] = { 0, 1, 2, 2, 3, 0 };
			std::shared_ptr<Vortex::IndexBuffer> ibo;
			ibo.reset(Vortex::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint)));
			m_vao2->AddIndexBuffer(ibo);

			std::string vertexSrc = R"(
			#version 460 core
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;
			uniform mat4 u_viewproj;
			uniform mat4 u_transform;
			out vec3 v_position;
			out vec4 v_color;
			void main() {
				v_position = a_position;
				v_color = a_color;
				gl_Position = u_viewproj * u_transform * vec4(a_position, 1.0);	
			}
			)";

			std::string fragmentSrc = R"(
			#version 460 core
			in vec3 v_position;
			layout(location = 0) out vec4 color;
			uniform vec4 u_color;
			void main() {
				color = u_color;
			}
			)";

			m_shader2.reset(Vortex::Shader::Create(vertexSrc.c_str(), fragmentSrc.c_str()));
		}
	}

	virtual void OnDetach() override {
	}

	virtual void OnUpdate(Vortex::Timestep ts) override {
		float framePosVel = camPosVel * ts;
		float frameRotVel = camRotVel * ts;
		if (Vortex::Input::IsKeyPressed(VT_KEY_LEFT)) {
			camPos.x += framePosVel;
		}
		else if (Vortex::Input::IsKeyPressed(VT_KEY_RIGHT)) {
			camPos.x -= framePosVel;
		}
		if (Vortex::Input::IsKeyPressed(VT_KEY_UP)) {
			camPos.y -= framePosVel;
		}
		else if (Vortex::Input::IsKeyPressed(VT_KEY_DOWN)) {
			camPos.y += framePosVel;
		}

		if (Vortex::Input::IsKeyPressed(VT_KEY_Q)) {
			camRot -= frameRotVel;
		}
		else if (Vortex::Input::IsKeyPressed(VT_KEY_E)) {
			camRot += frameRotVel;
		}

		if (Vortex::Input::IsKeyPressed(VT_KEY_R)) {
			camPos = glm::vec3();
			camRot = 0.0f;
		}

		Vortex::Render::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Vortex::Render::Clear();

		m_camera.SetPos(camPos);
		m_camera.SetRot(camRot);

		Vortex::Renderer::BeginScene(m_camera);

		m_shader2->Bind();
		std::dynamic_pointer_cast<Vortex::OpenGLShader>(m_shader2)->UploadUniformFloat4("u_color", scColor);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int y = -10; y < 10; ++y) {
			for (int x = -10; x < 10; ++x) {
				glm::vec3 pos(x * 0.12f, y * 0.12f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Vortex::Renderer::Submit(m_shader2, m_vao2, transform);
			}
		}

		Vortex::Renderer::Submit(m_shader1, m_vao1);

		Vortex::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::DragFloat3("Pos", &camPos.x, 0.005f, -1.0f, 1.0f);
		ImGui::SameLine();
		if (ImGui::Button("Reset pos")) { camPos = glm::vec3(0.0f); }
		ImGui::DragFloat("Rot", &camRot, 0.1f, -90.0f, 90.0f);
		ImGui::SameLine();
		if (ImGui::Button("Reset rot")) { camRot = 0.0f; }
		ImGui::ColorEdit3("Square Color", glm::value_ptr(scColor));
	}

	virtual void OnEvent(Vortex::Event& e) override {
	}
};

class Sandbox : public Vortex::Application {
public:
	Sandbox() {
		PushLayer(new SimpleLayer());
	}
	~Sandbox() {}
};

Vortex::Application* Vortex::CreateApplication() {
	return new Sandbox();
}
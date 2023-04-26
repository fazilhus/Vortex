#include <Vortex.hpp>

#include <memory>


class SimpleLayer : public Vortex::Layer {
private:
	std::shared_ptr<Vortex::VertexArray> m_vao1;
	std::shared_ptr<Vortex::Shader> m_shader1;
	std::shared_ptr<Vortex::VertexArray> m_vao2;
	std::shared_ptr<Vortex::Shader> m_shader2;

	Vortex::OrthoCamera m_camera;

	glm::vec3 pos{ 0.0f, 0.0f, 0.0f };
	float posVel = 1.0f;
	float rot{ 0.0f };
	float rotVel = 180.0f;

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
				out vec3 v_position;
				out vec4 v_color;
				void main() {
					v_position = a_position;
					v_color = a_color;
					gl_Position = u_viewproj * vec4(a_position, 1.0);	
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

			m_shader1.reset(new Vortex::Shader(vertexSrc.c_str(), fragmentSrc.c_str()));
		}

		{
			m_vao2.reset(Vortex::VertexArray::Create());

			float vertices[3 * 4] = {
				-0.75f, -0.75f, 0.0f,
				 0.75f, -0.75f, 0.0f,
				 0.75f,  0.75f, 0.0f,
				-0.75f,  0.75f, 0.0f
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
			out vec3 v_position;
			out vec4 v_color;
			void main() {
				v_position = a_position;
				v_color = a_color;
				gl_Position = u_viewproj * vec4(a_position, 1.0);	
			}
			)";

			std::string fragmentSrc = R"(
			#version 460 core
			in vec3 v_position;
			layout(location = 0) out vec4 color;
			void main() {
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
			)";

			m_shader2.reset(new Vortex::Shader(vertexSrc.c_str(), fragmentSrc.c_str()));
		}
	}

	virtual void OnDetach() override {
	}

	virtual void OnUpdate(Vortex::Timestep ts) override {
		float framePosVel = posVel * ts;
		float frameRotVel = rotVel * ts;
		if (Vortex::Input::IsKeyPressed(VT_KEY_LEFT)) {
			pos.x += framePosVel;
		}
		else if (Vortex::Input::IsKeyPressed(VT_KEY_RIGHT)) {
			pos.x -= framePosVel;
		}
		if (Vortex::Input::IsKeyPressed(VT_KEY_UP)) {
			pos.y -= framePosVel;
		}
		else if (Vortex::Input::IsKeyPressed(VT_KEY_DOWN)) {
			pos.y += framePosVel;
		}

		if (Vortex::Input::IsKeyPressed(VT_KEY_Q)) {
			rot -= frameRotVel;
		}
		else if (Vortex::Input::IsKeyPressed(VT_KEY_E)) {
			rot += frameRotVel;
		}

		if (Vortex::Input::IsKeyPressed(VT_KEY_R)) {
			pos = glm::vec3();
			rot = 0.0f;
		}

		Vortex::Render::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Vortex::Render::Clear();

		m_camera.SetPos(pos);
		m_camera.SetRot(rot);

		Vortex::Renderer::BeginScene(m_camera);

		Vortex::Renderer::Submit(m_shader2, m_vao2);
		Vortex::Renderer::Submit(m_shader1, m_vao1);

		Vortex::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::DragFloat3("Pos", &pos.x, 0.005f, -1.0f, 1.0f);
		ImGui::SameLine();
		if (ImGui::Button("Reset pos")) { pos = glm::vec3(0.0f); }
		ImGui::DragFloat("Rot", &rot, 0.1f, -90.0f, 90.0f);
		ImGui::SameLine();
		if (ImGui::Button("Reset rot")) { rot = 0.0f; }
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
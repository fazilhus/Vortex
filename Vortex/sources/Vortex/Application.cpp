#include "vtpch.hpp"

#include <glad/glad.h>

#include "Vortex/Application.hpp"

namespace Vortex {

	Application* Application::s_instance = nullptr;

	Application::Application() {
		VT_CORE_ASSERT(!s_instance, "Application already exists");
		s_instance = this;

		m_window = WinWindow::Create();
		m_running = true;
		m_window->SetEventCallback(VT_BIND_EVENT_FN(Application::OnEvent));

		m_imguiLayer = new ImGuiLayer();
		PushOverlay(m_imguiLayer);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		float verticies[3 * 3] = {
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 0.0f,  1.0f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		uint indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		std::string vertexSrc = R"(
			#version 460 core
			layout(location = 0) in vec3 a_position;
			out vec3 v_position;
			void main() {
				v_position = a_position;
				gl_Position = vec4(a_position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 460 core
			in vec3 v_position;
			layout(location = 0) out vec4 color;
			void main() {
				color = vec4(v_position * 0.5 + 0.5, 1.0);
			}
		)";

		m_shader = new Shader(vertexSrc.c_str(), fragmentSrc.c_str());
	}

	Application::~Application() {
		delete m_window;
		delete m_shader;
	}

	void Application::Run() {
		while (m_running) {
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_shader->Bind();
			glBindVertexArray(m_vao);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_layerStack) {
				layer->OnUpdate();
			}

			m_imguiLayer->Begin();
			for (Layer* layer : m_layerStack)
				layer->OnImGuiRender();
			m_imguiLayer->End();

			m_window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(VT_BIND_EVENT_FN(Application::OnAppClose));

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.m_handled) {
				break;
			}
		}
	}

	void Application::PushLayer(Layer* l) {
		m_layerStack.PushLayer(l);
	}

	void Application::PopLayer(Layer* l) {
		m_layerStack.PopLayer(l);
	}

	void Application::PushOverlay(Layer* o) {
		m_layerStack.PushOverlay(o);
	}

	void Application::PopOverlay(Layer* o) {
		m_layerStack.PopOverlay(o);
	}

	bool Application::OnAppClose(WindowCloseEvent& e) {
		m_running = false;
		return true;
	}

}

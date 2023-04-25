#include "vtpch.hpp"

#include <glad/glad.h>

#include "Vortex/Application.hpp"

namespace Vortex {

	Application* Application::s_instance = nullptr;

	Application::Application() {
		VT_CORE_ASSERT(!s_instance, "Application already exists");
		s_instance = this;

		m_window.reset(WinWindow::Create());
		m_running = true;
		m_window->SetEventCallback(VT_BIND_EVENT_FN(Application::OnEvent));

		m_imguiLayer = new ImGuiLayer();
		PushOverlay(m_imguiLayer);

		m_vao.reset(VertexArray::Create());

		float vertices[4 * 7] = {
			-1.0f, -1.0f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 1.0f, -1.0f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			-1.0f,  1.0f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
			 1.0f,  1.0f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		};

		std::shared_ptr<VertexBuffer> vbo;
		vbo.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout{ {"a_position", ShaderDataType::Float3}, {"a_color", ShaderDataType::Float4} };
		vbo->SetLayout(layout);
		m_vao->AddVertexBuffer(vbo);

		uint indices[6] = { 0, 1, 2, 2, 3, 1 };
		std::shared_ptr<IndexBuffer> ibo;
		ibo.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint)));
		m_vao->AddIndexBuffer(ibo);

		std::string vertexSrc = R"(
			#version 460 core
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;
			out vec3 v_position;
			out vec4 v_color;
			void main() {
				v_position = a_position;
				v_color = a_color;
				gl_Position = vec4(a_position, 1.0);	
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

		m_shader.reset(new Shader(vertexSrc.c_str(), fragmentSrc.c_str()));
	}

	void Application::Run() {
		while (m_running) {
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_shader->Bind();
			m_vao->Bind();
			glDrawElements(GL_TRIANGLES, m_vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (const auto& layer : m_layerStack) {
				layer->OnUpdate();
			}

			m_imguiLayer->Begin();
			for (const auto& layer : m_layerStack)
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

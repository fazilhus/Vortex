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
	}

	Application::~Application() {
		delete m_window;
	}

	void Application::Run() {
		while (m_running) {
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (Layer* layer : m_layerStack) {
				layer->OnUpdate();
			}

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
		l->OnAttach();
	}

	void Application::PopLayer(Layer* l) {
		m_layerStack.PopLayer(l);
		l->OnDetach();
	}

	void Application::PushOverlay(Layer* o) {
		m_layerStack.PushOverlay(o);
		o->OnAttach();
	}

	void Application::PopOverlay(Layer* o) {
		m_layerStack.PopOverlay(o);
		o->OnDetach();
	}

	bool Application::OnAppClose(WindowCloseEvent& e) {
		m_running = false;
		return true;
	}

}

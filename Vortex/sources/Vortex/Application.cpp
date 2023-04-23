#include "vtpch.hpp"

#include "Vortex/Application.hpp"

namespace Vortex {

	Application::Application() {
		m_window = WinWindow::Create();
		m_running = true;
		m_window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application() {
		delete m_window;
	}

	void Application::Run() {
		while (m_running) {
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e) {
		VT_CORE_TRACE("AppEvent: {0}", e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnAppClose));
	}

	bool Application::OnAppClose(WindowCloseEvent& e) {
		m_running = false;
		return true;
	}

}

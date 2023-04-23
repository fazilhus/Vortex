#include "vtpch.hpp"

#include "Vortex/Application.hpp"

namespace Vortex {

	Application::Application() {
		m_window = WinWindow::Create();
		m_running = true;
	}

	Application::~Application() {
	}

	void Application::Run() {
		while (m_running) {
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_window->OnUpdate();
		}
	}

}

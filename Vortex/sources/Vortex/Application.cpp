#include "Vortex/Application.hpp"
#include "Vortex/Events/AppEvent.hpp"
#include "Vortex/Log.hpp"

namespace Vortex {

	Application::Application() {
	}

	Application::~Application() {
	}

	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		VT_TRACE(e);
		while (true);
	}

}

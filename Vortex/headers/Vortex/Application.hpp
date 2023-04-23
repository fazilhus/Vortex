#pragma once

#include "Core.hpp"

#ifdef VT_PLATFORM_WIN
#include "Platforms/Win/WinWindow.hpp"
#endif

namespace Vortex {

	class VORTEX_API Application {
	private:
		WinWindow* m_window;
		bool m_running = true;

	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in the CLIENT
	Application* CreateApplication();

}
#pragma once

#ifdef VT_PLATFORM_WIN
#include "Platforms/Win/WinWindow.hpp"
#endif

#include "Vortex/Events/AppEvent.hpp"
#include "Vortex/Layers/LayerStack.hpp"

namespace Vortex {

	class VORTEX_API Application {
	private:
		WinWindow* m_window;
		bool m_running = true;
		LayerStack m_layerStack;

	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* l);
		void PopLayer(Layer* l);
		void PushOverlay(Layer* o);
		void PopOverlay(Layer* o);

	private:
		bool OnAppClose(WindowCloseEvent& e);
	};

	// To be defined in the CLIENT
	Application* CreateApplication();

}
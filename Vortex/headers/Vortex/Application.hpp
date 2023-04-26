#pragma once
#include "vtpch.hpp"

#ifdef VT_PLATFORM_WIN
#include "Platforms/Win/WinWindow.hpp"
#endif

#include "Vortex/Events/AppEvent.hpp"
#include "Vortex/Layers/ImGuiLayer.hpp"
#include "Vortex/Layers/LayerStack.hpp"

namespace Vortex {

	class Application {
	private:
		std::unique_ptr<Window> m_window;
		bool m_running = true;
		ImGuiLayer* m_imguiLayer;
		LayerStack m_layerStack;

		static Application* s_instance;

		float m_lastFrameTime;

	public:
		Application();
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* l);
		void PopLayer(Layer* l);
		void PushOverlay(Layer* o);
		void PopOverlay(Layer* o);

		inline Window& GetWindow() const { return *m_window; }
		inline static Application& Get() { return *s_instance; }

	private:
		bool OnAppClose(WindowCloseEvent& e);
	};

	// To be defined in the CLIENT
	Application* CreateApplication();

}
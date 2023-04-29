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
		Scope<Window> m_window;
		bool m_running = true;
		Ref<ImGuiLayer> m_imguiLayer;
		LayerStack m_layerStack;

		static Application* s_instance;

		float m_lastFrameTime;

		bool m_isMinimized = false;

	public:
		Application();
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Ref<Layer> l);
		void PopLayer(Ref<Layer> l);
		void PushOverlay(Ref<Layer> o);
		void PopOverlay(Ref<Layer> o);

		inline Window& GetWindow() const { return *m_window; }
		inline static Application& Get() { return *s_instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	};

	// To be defined in the CLIENT
	Application* CreateApplication();

}
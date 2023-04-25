#pragma once
#include "vtpch.hpp"

#ifdef VT_PLATFORM_WIN
#include "Platforms/Win/WinWindow.hpp"
#endif

#include "Vortex/Events/AppEvent.hpp"
#include "Vortex/Layers/ImGuiLayer.hpp"
#include "Vortex/Layers/LayerStack.hpp"
#include "Vortex/Renderer/Shader.hpp"

namespace Vortex {

	class Application {
	private:
		WinWindow* m_window;
		bool m_running = true;
		ImGuiLayer* m_imguiLayer;
		LayerStack m_layerStack;

		uint m_vao;
		std::unique_ptr<VertexBuffer> m_vbo;
		std::unique_ptr<IndexBuffer> m_ibo;
		std::unique_ptr<Shader> m_shader;

		static Application* s_instance;

	public:
		Application();
		virtual ~Application();

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
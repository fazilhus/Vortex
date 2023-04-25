#pragma once
#include "vtpch.hpp"

#ifdef VT_PLATFORM_WIN
#include "Platforms/Win/WinWindow.hpp"
#endif

#include "Vortex/Events/AppEvent.hpp"
#include "Vortex/Layers/ImGuiLayer.hpp"
#include "Vortex/Layers/LayerStack.hpp"
#include "Vortex/Renderer/Shader.hpp"
#include "Vortex/Renderer/Cameras/OrthoCamera.hpp"

namespace Vortex {

	class Application {
	private:
		std::unique_ptr<Window> m_window;
		bool m_running = true;
		ImGuiLayer* m_imguiLayer;
		LayerStack m_layerStack;

		std::shared_ptr<VertexArray> m_vao1;
		std::shared_ptr<Shader> m_shader1;
		std::shared_ptr<VertexArray> m_vao2;
		std::shared_ptr<Shader> m_shader2;

		OrthoCamera m_camera;

		static Application* s_instance;

		glm::vec3 pos{ 0.0f, 0.0f, 0.0f };
		float rot{ 0.0f };

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
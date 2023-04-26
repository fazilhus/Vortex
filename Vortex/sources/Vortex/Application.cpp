#include "vtpch.hpp"
#include "Vortex/Application.hpp"
#include "Vortex/Core/Timestep.hpp"

namespace Vortex {

	Application* Application::s_instance = nullptr;

	Application::Application() : m_lastFrameTime(0.0f) {
		VT_CORE_ASSERT(!s_instance, "Application already exists");
		s_instance = this;

		m_window.reset(WinWindow::Create());
		m_running = true;
		m_window->SetVSync(true);
		m_window->SetEventCallback(VT_BIND_EVENT_FN(Application::OnEvent));

		m_imguiLayer = new ImGuiLayer();
		PushOverlay(m_imguiLayer);
	}

	void Application::Run() {
		while (m_running) {
			float time = (float)Platform::GetTimeSec();
			Timestep timestep = time - m_lastFrameTime;
			m_lastFrameTime = time;

			for (const auto& layer : m_layerStack) {
				layer->OnUpdate(timestep);
			}

			m_imguiLayer->Begin();
			for (const auto& layer : m_layerStack)
				layer->OnImGuiRender();
			m_imguiLayer->End();

			m_window->OnUpdate(timestep);
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
	}

	void Application::PopLayer(Layer* l) {
		m_layerStack.PopLayer(l);
	}

	void Application::PushOverlay(Layer* o) {
		m_layerStack.PushOverlay(o);
	}

	void Application::PopOverlay(Layer* o) {
		m_layerStack.PopOverlay(o);
	}

	bool Application::OnAppClose(WindowCloseEvent& e) {
		m_running = false;
		return true;
	}

}

#include "vtpch.hpp"
#include "Vortex/Core/Application.hpp"
#include "Vortex/Core/Timestep.hpp"
#include "Vortex/Renderer/Renderer.hpp"

namespace Vortex {

	Application* Application::s_instance = nullptr;

	Application::Application() : m_lastFrameTime(0.0f) {
		VT_PROFILE_FUNCTION();
		VT_CORE_ASSERT(!s_instance, "Application already exists");
		s_instance = this;

		m_window = Window::Create({ "Vortex Engine", 1600, 900 });
		m_running = true;
		m_window->SetVSync(true);
		m_window->SetEventCallback(VT_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init({true, true});

		m_imguiLayer = std::make_shared<ImGuiLayer>();
		PushOverlay(m_imguiLayer);
	}

	Application::~Application() {
		VT_PROFILE_FUNCTION();
		Renderer::Shutdown();
	}

	void Application::Run() {
		VT_PROFILE_FUNCTION();
		while (m_running) {
			{
				VT_PROFILE_SCOPE("Application::Run - run loop");
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
	}

	void Application::OnEvent(Event& e) {
		VT_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(VT_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(VT_BIND_EVENT_FN(Application::OnWindowResize));

		VT_CORE_INFO("Application::OnEvent LayerStack size {0}", m_layerStack.GetSize());
		for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it) {
			(*it)->OnEvent(e);
			if (e.m_handled) {
				break;
			}
		}
	}

	void Application::PushLayer(Ref<Layer> l) {
		VT_PROFILE_FUNCTION();
		m_layerStack.PushLayer(l);
	}

	void Application::PopLayer(Ref<Layer> l) {
		VT_PROFILE_FUNCTION();
		m_layerStack.PopLayer(l);
	}

	void Application::PushOverlay(Ref<Layer> o) {
		VT_PROFILE_FUNCTION();
		m_layerStack.PushOverlay(o);
	}

	void Application::PopOverlay(Ref<Layer> o) {
		VT_PROFILE_FUNCTION();
		m_layerStack.PopOverlay(o);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Window close event");
		m_running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Window resize event to {0} by {1}", e.GetWidth(), e.GetHeight());
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_isMinimized = true;
			return false;
		}

		m_isMinimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}

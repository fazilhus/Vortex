#pragma once

#include "vtpch.hpp"

namespace Vortex {

	class WinWindow : public Window {
	private:
		GLFWwindow* m_window;
		Scope<GraphicsContext> m_context;

		struct WindowData
		{
			std::string title = "";
			uint4 width = 0, height = 0;
			bool vsync = false;

			EventCallbackFn eventCallback;
		};

		WindowData m_data;

	public:
		WinWindow(const WindowProps& properties);
		virtual ~WinWindow();

		void OnUpdate(Timestep ts) override;

		inline uint4 GetWidth() const override { return m_data.width; }
		inline uint4 GetHeight() const override { return m_data.height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_data.eventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const override { return m_window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	};

}
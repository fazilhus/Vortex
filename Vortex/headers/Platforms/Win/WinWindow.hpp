#pragma once

#include "vtpch.hpp"

#include "Vortex/Window.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Vortex {

	class WinWindow : public Window {
	private:
		GLFWwindow* m_window;

		struct WindowData
		{
			std::string title = "";
			uint width = 0, height = 0;
			bool vsync = false;

			EventCallbackFn eventCallback;
		};

		WindowData m_data;

	public:
		WinWindow(const WindowProps& properties);
		virtual ~WinWindow();

		void OnUpdate() override;

		inline uint GetWidth() const override { return m_data.width; }
		inline uint GetHeight() const override { return m_data.height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_data.eventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		static WinWindow* Create(const WindowProps& props = WindowProps());

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	};

}
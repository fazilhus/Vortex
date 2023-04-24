#pragma once

#include "vtpch.hpp"

#include "Vortex/Events/Event.hpp"

namespace Vortex {

	struct WindowProps
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "Hazel Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: title(title), width(width), height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

	};

}
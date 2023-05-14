#pragma once
#include <vtpch.hpp>

namespace Vortex {

	class WindowResizeEvent : public Event {
	private:
		uint4 m_width;
		uint4 m_height;

	public:
		WindowResizeEvent(uint4 width, uint4 height) 
			: m_width(width), m_height(height) {}

		inline uint4 GetWidth() const { return m_width; }
		inline uint4 GetHeight() const { return m_height; }

#ifdef VT_DEBUG
		std::string ToStr() const override {
			std::stringstream ss;
			ss << "WindowResizedEvent: " << m_width << ", " << m_height << std::endl;
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CAT(EventCatApp)
	};

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CAT(EventCatApp)
	};

	class WindowFocusEvent : public Event {
	public:
		WindowFocusEvent() = default;

		EVENT_CLASS_TYPE(WindowFocus)
		EVENT_CLASS_CAT(EventCatApp)
	};

	class WindowLostFocusEvent : public Event {
	public:
		WindowLostFocusEvent() = default;

		EVENT_CLASS_TYPE(WindowLostFocus)
		EVENT_CLASS_CAT(EventCatApp)
	};

	class WindowMovedEvent : public Event {
	public:
		WindowMovedEvent() = default;

		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CAT(EventCatApp)
	};

	class AppTickEvent : public Event {
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CAT(EventCatApp)
	};

	class AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CAT(EventCatApp)
	};

	class AppRenderEvent : public Event {
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CAT(EventCatApp)
	};
}
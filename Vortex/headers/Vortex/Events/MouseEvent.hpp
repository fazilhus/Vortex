#pragma once

#include <sstream>

#include "Event.hpp"

namespace Vortex {
	
	class VORTEX_API MouseMovedEvent : public Event {
	private:
		float m_mousex, m_mousey;

	public:
		MouseMovedEvent(int x, int y) 
			: m_mousex(x), m_mousey(y) {}

		inline float GetX() const { return m_mousex; }
		inline float GetY() const { return m_mousey; }

#ifdef VT_DEBUG
		std::string ToStr() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << GetX() << ", " << GetY() << std::endl;
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CAT(EventCatMouse | EventCatInput)
	};

	class VORTEX_API MouseScrolledEvent : public Event {
	private:
		float m_offsetx, m_offsety;

	public:
		MouseScrolledEvent(float offsetx, float offsety)
			: m_offsetx(offsetx), m_offsety(offsety) {}

		inline float GetOffsetX() const { return m_offsetx; }
		inline float GetOffsetY() const { return m_offsety; }

#ifdef VT_DEBUG
		std::string ToStr() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetOffsetX() << ", " << GetOffsetY();
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CAT(EventCatMouse | EventCatInput)
	};

	// abstract class for any mousebutton events
	class VORTEX_API MouseButtonEvent : public Event {
	protected:
		MouseButtonEvent(int button)
			: m_button(button) {}

		int m_button;

	public:
		inline int GetMouseButton() const { return m_button; }

		EVENT_CLASS_CAT(EventCatMouse | EventCatInput)
	};

	class VORTEX_API MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

#ifdef VT_DEBUG
		std::string ToStr() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_button;
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class VORTEX_API MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

#ifdef VT_DEBUG
		std::string ToStr() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_button;
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}
#pragma once

#include <vtpch.hpp>

namespace Vortex {
	
	class MouseMovedEvent : public Event {
	private:
		float m_mousex, m_mousey;

	public:
		MouseMovedEvent(const float x, const float y) 
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

	class MouseScrolledEvent : public Event {
	private:
		float m_offsetx, m_offsety;

	public:
		MouseScrolledEvent(const float offsetx, const float offsety)
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
	class MouseButtonEvent : public Event {
	protected:
		MouseButtonEvent(const MouseCode button)
			: m_button(button) {}

		MouseCode m_button;

	public:
		inline MouseCode GetMouseButton() const { return m_button; }

		EVENT_CLASS_CAT(EventCatMouse | EventCatInput)
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(const MouseCode button)
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

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(const MouseCode button)
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
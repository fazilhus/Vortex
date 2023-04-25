#pragma once

#include "vtpch.hpp"

#include "Event.hpp"

namespace Vortex {

	// Abstract event for any key events
	class KeyEvent : public Event {
	protected:
		KeyEvent(int keycode)
			: m_keycode(keycode) {}

		int m_keycode;

	public:
		inline int GetKeyCode() { return m_keycode; }

		EVENT_CLASS_CAT(EventCatKeyboard | EventCatInput)
	};

	class KeyPressedEvent : public KeyEvent {
	private:
		int m_repeatCount;

	public:
		KeyPressedEvent(int keycode, int repeatCount) 
			: KeyEvent(keycode), m_repeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_repeatCount; }

#ifdef VT_DEBUG
		std::string ToStr() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keycode << " pressed " << m_repeatCount << " times" << std::endl;
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(KeyPressed)
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keycode) 
			: KeyEvent(keycode) {}

#ifdef VT_DEBUG
		std::string ToStr() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_keycode << " released" << std::endl;
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(int keycode) 
			: KeyEvent(keycode) {}

#ifdef VT_DEBUG
		std::string ToStr() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_keycode << " typed" << std::endl;
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(KeyTyped);
	};
}
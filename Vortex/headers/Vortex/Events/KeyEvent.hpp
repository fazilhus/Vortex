#pragma once

#include <vtpch.hpp>

namespace Vortex {

	// Abstract event for any key events
	class KeyEvent : public Event {
	protected:
		KeyEvent(KeyCode keycode)
			: m_keycode(keycode) {}

		KeyCode m_keycode;

	public:
		inline KeyCode GetKeyCode() const { return m_keycode; }

		EVENT_CLASS_CAT(EventCatKeyboard | EventCatInput)
	};

	class KeyPressedEvent : public KeyEvent {
	private:
		int m_repeatCount;

	public:
		KeyPressedEvent(KeyCode keycode, int repeatCount)
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
		KeyReleasedEvent(KeyCode keycode)
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
		KeyTypedEvent(KeyCode keycode)
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
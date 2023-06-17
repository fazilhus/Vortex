#pragma once

#include <vtpch.hpp>

namespace Vortex {

	// Abstract event for any key events
	class KeyEvent : public Event {
	protected:
		KeyEvent(const KeyCode keycode)
			: m_keycode(keycode) {}

		KeyCode m_keycode;

	public:
		inline KeyCode GetKeyCode() const { return m_keycode; }

		EVENT_CLASS_CAT(EventCatKeyboard | EventCatInput)
	};

	class KeyPressedEvent : public KeyEvent {
	private:
		bool m_isRepeat;

	public:
		KeyPressedEvent(const KeyCode keycode, const bool isRepeat)
			: KeyEvent(keycode), m_isRepeat(isRepeat) {}

		inline bool IsRepeat() const { return m_isRepeat; }

#ifdef VT_DEBUG
		std::string ToStr() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keycode << " pressed =  " << m_isRepeat;
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(KeyPressed)
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(const KeyCode keycode)
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
		KeyTypedEvent(const KeyCode keycode)
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

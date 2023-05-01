#pragma once

#include "vtpch.hpp"

namespace Vortex {

	// Events are currently blocking, which means that the event will be handled as soon as it was called
	// Will do some kind of event buffer in the future

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCat {
		None = 0,
		EventCatApp =         BIT(0),
		EventCatInput =       BIT(1),
		EventCatKeyboard =    BIT(2),
		EventCatMouse =       BIT(3),
		EventCatMouseButton = BIT(4)
	};

// macros for some of virtual functions that are too trivial to implement in each event class
#define EVENT_CLASS_CAT(cat) virtual int GetCatFlags() const override { return cat; }

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
	                           virtual EventType GetEventType() const override { return GetStaticType(); }\
                               virtual const char* GetName() const override { return #type; }


	class Event {
	public:
		bool m_handled = false;

	private:
		friend class EventDispatcher;

	public:
		virtual const char* GetName() const = 0;
#ifdef VT_DEBUG
		virtual std::string ToStr() const { return GetName(); }
#endif
		virtual EventType GetEventType() const = 0;
		virtual int GetCatFlags() const = 0;

		inline bool IsInCat(EventCat cat) {
			return GetCatFlags() & cat;
		}
	};

	class EventDispatcher {
	private:
		Event& m_event;

	public:
		EventDispatcher(Event& event) 
			: m_event(event) {}

		// TODO typesafety
		template <typename T, typename Fn >
		bool Dispatch(const Fn& fn) {
			if (m_event.GetEventType() == T::GetStaticType()) {
				m_event.m_handled = fn(static_cast<T&>(m_event));
				return true;
			}
			return false;
		}		
	};

#ifdef VT_DEBUG
	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToStr();
	}
#endif
}


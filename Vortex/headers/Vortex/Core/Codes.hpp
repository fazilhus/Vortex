#pragma once
#include "Vortex/Core/Core.hpp"

namespace Vortex {

	typedef enum class KeyCode : uint16 {
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	typedef enum class MouseCode : uint16
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keycode) {
		os << static_cast<int32>(keycode);
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, MouseCode mousecode)
	{
		os << static_cast<int32>(mousecode);
		return os;
	}

}

// From glfw3.h
#define VT_KEY_SPACE           ::Vortex::Key::Space
#define VT_KEY_APOSTROPHE      ::Vortex::Key::Apostrophe    /* ' */
#define VT_KEY_COMMA           ::Vortex::Key::Comma         /* , */
#define VT_KEY_MINUS           ::Vortex::Key::Minus         /* - */
#define VT_KEY_PERIOD          ::Vortex::Key::Period        /* . */
#define VT_KEY_SLASH           ::Vortex::Key::Slash         /* / */
#define VT_KEY_0               ::Vortex::Key::D0
#define VT_KEY_1               ::Vortex::Key::D1
#define VT_KEY_2               ::Vortex::Key::D2
#define VT_KEY_3               ::Vortex::Key::D3
#define VT_KEY_4               ::Vortex::Key::D4
#define VT_KEY_5               ::Vortex::Key::D5
#define VT_KEY_6               ::Vortex::Key::D6
#define VT_KEY_7               ::Vortex::Key::D7
#define VT_KEY_8               ::Vortex::Key::D8
#define VT_KEY_9               ::Vortex::Key::D9
#define VT_KEY_SEMICOLON       ::Vortex::Key::Semicolon     /* ; */
#define VT_KEY_EQUAL           ::Vortex::Key::Equal         /* = */
#define VT_KEY_A               ::Vortex::Key::A
#define VT_KEY_B               ::Vortex::Key::B
#define VT_KEY_C               ::Vortex::Key::C
#define VT_KEY_D               ::Vortex::Key::D
#define VT_KEY_E               ::Vortex::Key::E
#define VT_KEY_F               ::Vortex::Key::F
#define VT_KEY_G               ::Vortex::Key::G
#define VT_KEY_H               ::Vortex::Key::H
#define VT_KEY_I               ::Vortex::Key::I
#define VT_KEY_J               ::Vortex::Key::J
#define VT_KEY_K               ::Vortex::Key::K
#define VT_KEY_L               ::Vortex::Key::L
#define VT_KEY_M               ::Vortex::Key::M
#define VT_KEY_N               ::Vortex::Key::N
#define VT_KEY_O               ::Vortex::Key::O
#define VT_KEY_P               ::Vortex::Key::P
#define VT_KEY_Q               ::Vortex::Key::Q
#define VT_KEY_R               ::Vortex::Key::R
#define VT_KEY_S               ::Vortex::Key::S
#define VT_KEY_T               ::Vortex::Key::T
#define VT_KEY_U               ::Vortex::Key::U
#define VT_KEY_V               ::Vortex::Key::V
#define VT_KEY_W               ::Vortex::Key::W
#define VT_KEY_X               ::Vortex::Key::X
#define VT_KEY_Y               ::Vortex::Key::Y
#define VT_KEY_Z               ::Vortex::Key::Z
#define VT_KEY_LEFT_BRACKET    ::Vortex::Key::LeftBracket   /* [ */
#define VT_KEY_BACKSLASH       ::Vortex::Key::Backslash     /* \ */
#define VT_KEY_RIGHT_BRACKET   ::Vortex::Key::RightBracket  /* ] */
#define VT_KEY_GRAVE_ACCENT    ::Vortex::Key::GraveAccent   /* ` */
#define VT_KEY_WORLD_1         ::Vortex::Key::World1        /* non-US #1 */
#define VT_KEY_WORLD_2         ::Vortex::Key::World2        /* non-US #2 */

/* Function keys */
#define VT_KEY_ESCAPE          ::Vortex::Key::Escape
#define VT_KEY_ENTER           ::Vortex::Key::Enter
#define VT_KEY_TAB             ::Vortex::Key::Tab
#define VT_KEY_BACKSPACE       ::Vortex::Key::Backspace
#define VT_KEY_INSERT          ::Vortex::Key::Insert
#define VT_KEY_DELETE          ::Vortex::Key::Delete
#define VT_KEY_RIGHT           ::Vortex::Key::Right
#define VT_KEY_LEFT            ::Vortex::Key::Left
#define VT_KEY_DOWN            ::Vortex::Key::Down
#define VT_KEY_UP              ::Vortex::Key::Up
#define VT_KEY_PAGE_UP         ::Vortex::Key::PageUp
#define VT_KEY_PAGE_DOWN       ::Vortex::Key::PageDown
#define VT_KEY_HOME            ::Vortex::Key::Home
#define VT_KEY_END             ::Vortex::Key::End
#define VT_KEY_CAPS_LOCK       ::Vortex::Key::CapsLock
#define VT_KEY_SCROLL_LOCK     ::Vortex::Key::ScrollLock
#define VT_KEY_NUM_LOCK        ::Vortex::Key::NumLock
#define VT_KEY_PRINT_SCREEN    ::Vortex::Key::PrintScreen
#define VT_KEY_PAUSE           ::Vortex::Key::Pause
#define VT_KEY_F1              ::Vortex::Key::F1
#define VT_KEY_F2              ::Vortex::Key::F2
#define VT_KEY_F3              ::Vortex::Key::F3
#define VT_KEY_F4              ::Vortex::Key::F4
#define VT_KEY_F5              ::Vortex::Key::F5
#define VT_KEY_F6              ::Vortex::Key::F6
#define VT_KEY_F7              ::Vortex::Key::F7
#define VT_KEY_F8              ::Vortex::Key::F8
#define VT_KEY_F9              ::Vortex::Key::F9
#define VT_KEY_F10             ::Vortex::Key::F10
#define VT_KEY_F11             ::Vortex::Key::F11
#define VT_KEY_F12             ::Vortex::Key::F12
#define VT_KEY_F13             ::Vortex::Key::F13
#define VT_KEY_F14             ::Vortex::Key::F14
#define VT_KEY_F15             ::Vortex::Key::F15
#define VT_KEY_F16             ::Vortex::Key::F16
#define VT_KEY_F17             ::Vortex::Key::F17
#define VT_KEY_F18             ::Vortex::Key::F18
#define VT_KEY_F19             ::Vortex::Key::F19
#define VT_KEY_F20             ::Vortex::Key::F20
#define VT_KEY_F21             ::Vortex::Key::F21
#define VT_KEY_F22             ::Vortex::Key::F22
#define VT_KEY_F23             ::Vortex::Key::F23
#define VT_KEY_F24             ::Vortex::Key::F24
#define VT_KEY_F25             ::Vortex::Key::F25

/* Keypad */
#define VT_KEY_KP_0            ::Vortex::Key::KP0
#define VT_KEY_KP_1            ::Vortex::Key::KP1
#define VT_KEY_KP_2            ::Vortex::Key::KP2
#define VT_KEY_KP_3            ::Vortex::Key::KP3
#define VT_KEY_KP_4            ::Vortex::Key::KP4
#define VT_KEY_KP_5            ::Vortex::Key::KP5
#define VT_KEY_KP_6            ::Vortex::Key::KP6
#define VT_KEY_KP_7            ::Vortex::Key::KP7
#define VT_KEY_KP_8            ::Vortex::Key::KP8
#define VT_KEY_KP_9            ::Vortex::Key::KP9
#define VT_KEY_KP_DECIMAL      ::Vortex::Key::KPDecimal
#define VT_KEY_KP_DIVIDE       ::Vortex::Key::KPDivide
#define VT_KEY_KP_MULTIPLY     ::Vortex::Key::KPMultiply
#define VT_KEY_KP_SUBTRACT     ::Vortex::Key::KPSubtract
#define VT_KEY_KP_ADD          ::Vortex::Key::KPAdd
#define VT_KEY_KP_ENTER        ::Vortex::Key::KPEnter
#define VT_KEY_KP_EQUAL        ::Vortex::Key::KPEqual

#define VT_KEY_LEFT_SHIFT      ::Vortex::Key::LeftShift
#define VT_KEY_LEFT_CONTROL    ::Vortex::Key::LeftControl
#define VT_KEY_LEFT_ALT        ::Vortex::Key::LeftAlt
#define VT_KEY_LEFT_SUPER      ::Vortex::Key::LeftSuper
#define VT_KEY_RIGHT_SHIFT     ::Vortex::Key::RightShift
#define VT_KEY_RIGHT_CONTROL   ::Vortex::Key::RightControl
#define VT_KEY_RIGHT_ALT       ::Vortex::Key::RightAlt
#define VT_KEY_RIGHT_SUPER     ::Vortex::Key::RightSuper
#define VT_KEY_MENU            ::Vortex::Key::Menu

#define VT_MOUSE_BUTTON_0      ::Vortex::Mouse::Button0
#define VT_MOUSE_BUTTON_1      ::Vortex::Mouse::Button1
#define VT_MOUSE_BUTTON_2      ::Vortex::Mouse::Button2
#define VT_MOUSE_BUTTON_3      ::Vortex::Mouse::Button3
#define VT_MOUSE_BUTTON_4      ::Vortex::Mouse::Button4
#define VT_MOUSE_BUTTON_5      ::Vortex::Mouse::Button5
#define VT_MOUSE_BUTTON_6      ::Vortex::Mouse::Button6
#define VT_MOUSE_BUTTON_7      ::Vortex::Mouse::Button7
#define VT_MOUSE_BUTTON_LAST   ::Vortex::Mouse::ButtonLast
#define VT_MOUSE_BUTTON_LEFT   ::Vortex::Mouse::ButtonLeft
#define VT_MOUSE_BUTTON_RIGHT  ::Vortex::Mouse::ButtonRight
#define VT_MOUSE_BUTTON_MIDDLE ::Vortex::Mouse::ButtonMiddle
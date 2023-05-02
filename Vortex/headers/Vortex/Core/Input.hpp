#pragma once

#include "vtpch.hpp"

#include "Vortex/Core/Codes.hpp"

namespace Vortex {

	class Input {
	protected:
		Input() = default;

	public:
		Input(const Input& other) = delete;
		Input& operator=(const Input& other) = delete;

	private:
		static Input* s_instance;

	public:
		inline static bool IsKeyPressed(KeyCode keycode) { return s_instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseButtonPressed(MouseCode button) { return s_instance->IsMouseButtonPressedImpl(button); }

		inline static std::pair<float, float> GetMousePos() { return s_instance->GetMousePosImpl(); }
		inline static float GetMouseX() { return s_instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(KeyCode keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;

		virtual std::pair<float, float> GetMousePosImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	};

}

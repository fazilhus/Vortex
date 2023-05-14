#pragma once

#include <vtpch.hpp>

#include "Vortex/Core/Codes.hpp"

namespace Vortex {

	class Input {
	public:
		static bool IsKeyPressed(KeyCode keycode);
		static bool IsMouseButtonPressed(MouseCode button);

		static std::pair<float, float> GetMousePos();
		static float GetMouseX();
		static float GetMouseY();
	};

}

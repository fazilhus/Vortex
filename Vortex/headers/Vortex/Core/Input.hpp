#pragma once

//#include <vtpch.hpp>

#include <glm/glm.hpp>
#include "Vortex/Core/Codes.hpp"

namespace Vortex {

	class Input {
	public:
		static bool IsKeyPressed(const KeyCode keycode);
		static bool IsMouseButtonPressed(const MouseCode button);

		static glm::vec2 GetMousePos();
		static float GetMouseX();
		static float GetMouseY();
	};

}

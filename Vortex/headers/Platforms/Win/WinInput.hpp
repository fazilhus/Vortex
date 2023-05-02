#pragma once
#include "vtpch.hpp"

namespace Vortex{

	class WinInput : public Input {
	protected:
		bool IsKeyPressedImpl(KeyCode keycode) override;
		bool IsMouseButtonPressedImpl(MouseCode button) override;

		std::pair<float, float> GetMousePosImpl() override;
		float GetMouseXImpl() override;
		float GetMouseYImpl() override;
	};

}
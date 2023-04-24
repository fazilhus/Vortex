#pragma once

#include "vtpch.hpp"

namespace Vortex{

	class WinInput : public Input {
	protected:
		bool IsKeyPressedImpl(int keycode) override;
		bool IsMouseButtonPressedImpl(int button) override;

		std::pair<float, float> GetMousePosImpl() override;
		float GetMouseXImpl() override;
		float GetMouseYImpl() override;
	};

}
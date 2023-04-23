#pragma once

#include "vtpch.hpp"

namespace Vortex {

	class VORTEX_API ImGuiLayer : public Layer {
	private:
		float m_time;

	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& e) override;
	};

}

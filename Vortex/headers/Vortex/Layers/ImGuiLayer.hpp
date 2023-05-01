#pragma once

#include "vtpch.hpp"

namespace Vortex {

	class ImGuiLayer : public Layer {
	private:
		float m_time;

	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		virtual void Begin();
		virtual void End();

	};

}

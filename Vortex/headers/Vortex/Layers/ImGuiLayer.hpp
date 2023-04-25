#pragma once

#include "vtpch.hpp"

#include "Vortex/Events/AppEvent.hpp"
#include "Vortex/Events/KeyEvent.hpp"
#include "Vortex/Events/MouseEvent.hpp"

namespace Vortex {

	class ImGuiLayer : public Layer {
	private:
		float m_time;

	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		virtual void Begin();
		virtual void End();

	};

}

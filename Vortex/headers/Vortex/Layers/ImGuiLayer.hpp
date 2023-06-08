#pragma once

#include <vtpch.hpp>

namespace Vortex {

	class ImGuiLayer : public Layer {
	private:
		float m_time;
		bool m_blockEvents;

	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() override = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

		virtual void Begin();
		virtual void End();

		void BlockEvents(bool block) { m_blockEvents = block; }
		void SetDarkTheme();
	};

}

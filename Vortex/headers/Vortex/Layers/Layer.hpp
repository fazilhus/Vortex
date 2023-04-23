#pragma once

#include "vtpch.hpp"

#include "Vortex/Events/Event.hpp"

namespace Vortex {

	class VORTEX_API Layer {
	public:
		Layer(const std::string& name);
		virtual ~Layer() {}

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_name; }

	private:
		std::string m_name;
	};

}
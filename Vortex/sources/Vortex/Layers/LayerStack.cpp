#include "vtpch.hpp"
#include "Vortex/Layers/LayerStack.hpp"

namespace Vortex {

	LayerStack::LayerStack() 
		: m_layers(std::vector<Ref<Layer>>()), m_layerInsertInd(0) {
		VT_PROFILE_FUNCTION();
	}

	void LayerStack::PushLayer(Ref<Layer> l) {
		VT_PROFILE_FUNCTION();
		VT_CORE_INFO("Layer pushed {0}", l->GetName());
		m_layers.emplace(m_layers.begin() + m_layerInsertInd, l);
		l->OnAttach();
	}

	void LayerStack::PushOverlay(Ref<Layer> o) {
		VT_PROFILE_FUNCTION();
		VT_CORE_INFO("Overlay pushed {0}", o->GetName());
		m_layers.emplace_back(o);
		o->OnAttach();
	}

	void LayerStack::PopLayer(Ref<Layer> l) {
		VT_PROFILE_FUNCTION();
		VT_CORE_INFO("Layer popped {0}", l->GetName());
		auto it = std::find(m_layers.begin() + m_layerInsertInd, end(), l);
		if (it != begin() + m_layerInsertInd) {
			l->OnDetach();
			m_layers.erase(it);
			m_layerInsertInd--;
		}
	}

	void LayerStack::PopOverlay(Ref<Layer> o) {
		VT_PROFILE_FUNCTION();
		VT_CORE_INFO("Overlay popped {0}", o->GetName());
		auto it = std::find(m_layers.begin() + m_layerInsertInd, end(), o);
		if (it != end()) {
			o->OnDetach();
			m_layers.erase(it);
		}
	}

}
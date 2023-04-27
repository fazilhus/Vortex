#include "vtpch.hpp"
#include "Vortex/Layers/LayerStack.hpp"

namespace Vortex {



	LayerStack::LayerStack() 
		: m_layers(std::vector<Ref<Layer>>()), m_layerInsertInd(0) {}

	LayerStack::~LayerStack() {
	}

	void LayerStack::PushLayer(Ref<Layer> l) {
		m_layers.emplace(m_layers.begin() + m_layerInsertInd, l);
		l->OnAttach();
	}

	void LayerStack::PushOverlay(Ref<Layer> o) {
		m_layers.emplace_back(o);
		o->OnAttach();
	}

	void LayerStack::PopLayer(Ref<Layer> l) {
		auto it = std::find(m_layers.begin() + m_layerInsertInd, end(), l);
		if (it != begin() + m_layerInsertInd) {
			l->OnDetach();
			m_layers.erase(it);
			m_layerInsertInd--;
		}
	}

	void LayerStack::PopOverlay(Ref<Layer> o) {
		auto it = std::find(m_layers.begin() + m_layerInsertInd, end(), o);
		if (it != end()) {
			o->OnDetach();
			m_layers.erase(it);
		}
	}

}
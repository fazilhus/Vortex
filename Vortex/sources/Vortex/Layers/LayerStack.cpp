#include "vtpch.hpp"
#include "Vortex/Layers/LayerStack.hpp"

namespace Vortex {



	LayerStack::LayerStack() 
		: m_layers(std::vector<Layer*>()), m_layerInsertInd(0) {}

	LayerStack::~LayerStack() {
		for (auto l : m_layers) {
			delete l;
		}
	}

	void LayerStack::PushLayer(Layer* l) {
		m_layers.emplace(m_layers.begin() + m_layerInsertInd, l);
		l->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* o) {
		m_layers.emplace_back(o);
		o->OnAttach();
	}

	void LayerStack::PopLayer(Layer* l) {
		auto it = std::find(m_layers.begin() + m_layerInsertInd, end(), l);
		if (it != end()) {
			m_layers.erase(it);
			m_layerInsertInd--;
			l->OnDetach();
		}
	}

	void LayerStack::PopOverlay(Layer* o) {
		auto it = std::find(m_layers.begin() + m_layerInsertInd, end(), o);
		if (it != end()) {
			m_layers.erase(it);
			o->OnDetach();
		}
	}

}
#include "vtpch.hpp"
#include "Vortex/Layers/LayerStack.hpp"

namespace Vortex {



	LayerStack::LayerStack() {
		m_layerInsert = begin();
	}

	LayerStack::~LayerStack() {
		for (auto l : m_layers) {
			delete l;
		}
	}

	void LayerStack::PushLayer(Layer* l) {
		m_layerInsert = m_layers.emplace(m_layerInsert, l);
	}

	void LayerStack::PushOverlay(Layer* o) {
		m_layers.emplace_back(o);
	}

	void LayerStack::PopLayer(Layer* l) {
		auto it = std::find(begin(), end(), l);
		if (it != end()) {
			m_layers.erase(it);
			m_layerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer* o) {
		auto it = std::find(begin(), end(), o);
		if (it != end()) {
			m_layers.erase(it);
		}
	}

}
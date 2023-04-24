#pragma once

#include "vtpch.hpp"

#include "Vortex/Layers/Layer.hpp"

namespace Vortex {

	class VORTEX_API LayerStack {
	private:
		std::vector<Layer*> m_layers;
		uint m_layerInsertInd;

	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* l);
		void PushOverlay(Layer* o);
		void PopLayer(Layer* l);
		void PopOverlay(Layer* o);

		inline std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		inline std::vector<Layer*>::iterator end() { return m_layers.end(); }
	};

}
#pragma once
#include "vtpch.hpp"

namespace Vortex {

	class LayerStack {
	private:
		std::vector<Ref<Layer>> m_layers;
		uint4 m_layerInsertInd;

	public:
		LayerStack();
		~LayerStack() = default;

		void PushLayer(Ref<Layer> l);
		void PushOverlay(Ref<Layer> o);
		void PopLayer(Ref<Layer> l);
		void PopOverlay(Ref<Layer> o);

		inline std::vector<Ref<Layer>>::iterator begin() { return m_layers.begin(); }
		inline std::vector<Ref<Layer>>::iterator end() { return m_layers.end(); }
	};

}
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

		inline uint8 GetSize() const { return m_layers.size(); }

		inline std::vector<Ref<Layer>>::iterator begin() { return m_layers.begin(); }
		inline std::vector<Ref<Layer>>::const_iterator begin() const { return m_layers.begin(); }
		inline std::vector<Ref<Layer>>::iterator end() { return m_layers.end(); }
		inline std::vector<Ref<Layer>>::const_iterator end() const { return m_layers.end(); }

		inline std::vector<Ref<Layer>>::reverse_iterator rbegin() { return m_layers.rbegin(); }
		inline std::vector<Ref<Layer>>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
		inline std::vector<Ref<Layer>>::reverse_iterator rend() { return m_layers.rend(); }
		inline std::vector<Ref<Layer>>::const_reverse_iterator rend() const { return m_layers.rend(); }
	};

}
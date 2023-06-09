#pragma once
#include <vtpch.hpp>

namespace Vortex {

	class LayerStack {
	private:
		Vector<Ref<Layer>> m_layers;
		uint4 m_layerInsertInd;

	public:
		LayerStack();
		~LayerStack() = default;

		void PushLayer(Ref<Layer> l);
		void PushOverlay(Ref<Layer> o);
		void PopLayer(Ref<Layer> l);
		void PopOverlay(Ref<Layer> o);

		inline std::size_t GetSize() const { return m_layers.size(); }

		inline Vector<Ref<Layer>>::iterator begin() { return m_layers.begin(); }
		inline Vector<Ref<Layer>>::const_iterator begin() const { return m_layers.begin(); }
		inline Vector<Ref<Layer>>::iterator end() { return m_layers.end(); }
		inline Vector<Ref<Layer>>::const_iterator end() const { return m_layers.end(); }

		inline Vector<Ref<Layer>>::reverse_iterator rbegin() { return m_layers.rbegin(); }
		inline Vector<Ref<Layer>>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
		inline Vector<Ref<Layer>>::reverse_iterator rend() { return m_layers.rend(); }
		inline Vector<Ref<Layer>>::const_reverse_iterator rend() const { return m_layers.rend(); }
	};

}

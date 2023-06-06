#pragma once

//#include "Vortex/Core/Core.hpp"
//#include "Vortex/Core/Log.hpp"
//#include "Vortex/Scene/Scene.hpp"
//#include "Vortex/Scene/Entity.hpp"

#include <Vortex.hpp>

namespace Vortex {

	class SceneHierarchyPanel {
	private:
		Ref<Scene> m_context;
		Entity m_selectionContext;

	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);
		~SceneHierarchyPanel() = default;

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

	private:
		void DrawEntityMode(Entity entity);
	};

}
#pragma once

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
		void DrawEntityComponents(Entity entity);
	};

}
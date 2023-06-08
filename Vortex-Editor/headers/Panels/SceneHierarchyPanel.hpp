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
		void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
	};

}
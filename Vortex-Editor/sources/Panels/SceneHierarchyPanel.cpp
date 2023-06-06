#include "Panels/SceneHierarchyPanel.hpp"

#include <imgui.h>
#include "Vortex/Scene/Components.hpp"

namespace Vortex {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene) {
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene) {
		m_context = scene;
		m_selectionContext = { entt::null, m_context.get() };
	}

	void SceneHierarchyPanel::OnImGuiRender() {
		ImGui::Begin("Scene Hierarchy");

		m_context->m_registry.each([&](auto entityID) {
			Entity entity{ entityID, m_context.get() };
			DrawEntityMode(entity);
		});

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityMode(Entity entity) {
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64)(uint32)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked()) {
			m_selectionContext = entity;
		}

		if (opened) {
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened) {
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}

}
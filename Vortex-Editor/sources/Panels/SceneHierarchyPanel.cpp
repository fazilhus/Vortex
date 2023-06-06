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

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_selectionContext = {};
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_selectionContext) {
			DrawEntityComponents(m_selectionContext);
		}

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
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawEntityComponents(Entity entity) {
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputTextWithHint("Tag", "Entity name", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>()) {
			auto flags = ImGuiTreeNodeFlags_DefaultOpen;
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), flags, "Transform")) {
				auto& transform = entity.GetComponent<TransformComponent>().Transform;

				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<SpriteComponent>()) {
			auto flags = ImGuiTreeNodeFlags_DefaultOpen;
			if (ImGui::TreeNodeEx((void*)typeid(SpriteComponent).hash_code(), flags, "Sprite")) {
				auto& color = entity.GetComponent<SpriteComponent>().Color;

				ImGui::ColorPicker4("Color", glm::value_ptr(color));
				ImGui::TreePop();
			}
		}
	}

}
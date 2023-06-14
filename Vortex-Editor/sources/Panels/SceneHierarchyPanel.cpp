#include "Panels/SceneHierarchyPanel.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "Vortex/Scene/Components.hpp"

#include <cstring>

namespace Vortex {

	extern const std::filesystem::path g_assetPath;

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

		if (ImGui::BeginPopupContextWindow("Create entity", 1 | ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::MenuItem("Create empty entity")) {
				m_context->CreateEntity("Empty entity");
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_selectionContext) {
			DrawComponents(m_selectionContext);

			if (ImGui::BeginPopupContextWindow("Entity Properties", 1 | ImGuiPopupFlags_NoOpenOverItems)) {
				if (ImGui::BeginMenu("Add Component")) {
					if (ImGui::MenuItem("Transform Component")) {
						if (!m_selectionContext.HasComponent<TransformComponent>()) {
							VT_CORE_TRACE("Added Transform Component to entity {0}", (uint32)m_selectionContext);
							m_selectionContext.AddComponent<TransformComponent>();
						}
						else {
							VT_CORE_WARN("Entity {0} already has Transform Component", (uint32)m_selectionContext);
						}
					}
					if (ImGui::MenuItem("Sprite Component")) {
						if (!m_selectionContext.HasComponent<SpriteComponent>()) {
							VT_CORE_TRACE("Added Sprite Component to entity {0}", (uint32)m_selectionContext);
							m_selectionContext.AddComponent<SpriteComponent>();
						}
						else {
							VT_CORE_WARN("Entity {0} already has Sprite Component", (uint32)m_selectionContext);
						}
					}
					if (ImGui::MenuItem("Camera Component")) {
						if (!m_selectionContext.HasComponent<CameraComponent>()) {
							VT_CORE_TRACE("Added Camera Component to entity {0}", (uint32)m_selectionContext);
							m_selectionContext.AddComponent<CameraComponent>();
						}
						else {
							VT_CORE_WARN("Entity {0} already has Camera Component", (uint32)m_selectionContext);
						}
					}

					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityMode(Entity entity) {
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)(uint64)(uint32)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked()) {
			m_selectionContext = entity;
		}

		bool deleted = false;
		// std::to_string((uint32)entity).c_str()) is needed to generate an unique id for imgui to use
		if (ImGui::BeginPopupContextItem(std::to_string((uint32)entity).c_str())) {
			if (ImGui::MenuItem("Delete entity")) {
				deleted = true;
			}

			ImGui::EndPopup();
		}

		if (opened) {
			// For parenting
			ImGui::TreePop();
		}

		if (deleted) {
			m_context->DestroyEntity(entity);
			if (m_selectionContext == entity) {
				m_selectionContext = {};
			}
		}
	}

	template<typename T, typename UIFunction>
	void SceneHierarchyPanel::DrawComponent(const std::string& label, Entity entity, UIFunction fn) {
		const auto flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<T>()) {
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

			if (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, label.c_str())) {
				bool removed = false;
				if (ImGui::BeginPopupContextWindow("Delete Component", 1)) {
					if (ImGui::MenuItem("Delete Component") ||
						Input::IsKeyPressed(Key::Delete)) {
						removed = true;
					}

					ImGui::EndPopup();
				}

				fn(component);

				if (removed) {
					entity.RemoveComponent<T>();
				}

				ImGui::TreePop();
			}
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity) {
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));
			if (ImGui::InputTextWithHint("##Tag", "Entity name", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
		}

		DrawComponent<TransformComponent>("Transform Component", entity, [&](auto& component) {
			DrawVec3Control("Translation", component.Translation);
			auto rot = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rot);
			component.Rotation = glm::radians(rot);
			DrawVec3Control("Scale", component.Scale, 1.0f);
		});

		DrawComponent<SpriteComponent>("Sprite Component", entity, [&](auto& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

			ImGui::Button("Texture", ImVec2{ 100.0f, 0.0f });
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath = std::filesystem::path(g_assetPath) / path;
					component.Texture = Texture2D::Create(texturePath.string());
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
		});

		DrawComponent<CameraComponent>("Camera Component", entity, [&](auto& component) {
			auto& camera = component.Camera;

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

			if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			switch (camera.GetProjectionType()) {
				case SceneCamera::ProjectionType::Perspective: {
					float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
					float persNear = camera.GetPerspectiveNearClip();
					float persFar = camera.GetPerspectiveFarClip();

					if (ImGui::DragFloat("Vertical FOV", &verticalFOV)) {
						camera.SetPerspectiveVerticalFOV(glm::radians(verticalFOV));
					}

					if (ImGui::DragFloat("Near", &persNear)) {
						camera.SetPerspectiveNearClip(persNear);
					}

					if (ImGui::DragFloat("Far", &persFar)) {
						camera.SetPerspectiveFarClip(persFar);
					}
					break;
				}
				case SceneCamera::ProjectionType::Orthographic: {
					float orthoSize = camera.GetOrthographicSize();
					float orthoNear = camera.GetOrthographicNearClip();
					float orthoFar = camera.GetOrthographicFarClip();

					if (ImGui::DragFloat("Size", &orthoSize)) {
						camera.SetOrthographicSize(orthoSize);
					}

					if (ImGui::DragFloat("Near", &orthoNear)) {
						camera.SetOrthographicNearClip(orthoNear);
					}

					if (ImGui::DragFloat("Far", &orthoFar)) {
						camera.SetOrthographicFarClip(orthoFar);
					}
					break;
				}
			}
		});
	}

	void SceneHierarchyPanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth) {
		ImGuiIO& io = ImGui::GetIO();
		auto bold = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

		ImGui::PushFont(bold);
		if (ImGui::Button("X", buttonSize)) {
			values.x = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

		ImGui::PushFont(bold);
		if (ImGui::Button("Y", buttonSize)) {
			values.y = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

		ImGui::PushFont(bold);
		if (ImGui::Button("Z", buttonSize)) {
			values.z = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	bool SceneHierarchyPanel::DrawDeleteComponentPopup() {
		bool removed = false;
		if (ImGui::BeginPopupContextWindow("Delete Component", 1)) {
			if (ImGui::MenuItem("Delete Component") ||
				Input::IsKeyPressed(Key::Delete)) {
				removed = true;
			}

			ImGui::EndPopup();
		}
		return removed;
	}

}

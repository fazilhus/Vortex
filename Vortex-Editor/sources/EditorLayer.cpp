#include "EditorLayer.hpp"
#include "Platforms/OpenGL/OpenGLShader.hpp"
#include "Vortex/Controllers/CameraController.hpp"
#include "Vortex/Scene/SceneSerializer.hpp"
#include "Vortex/Utils/Platform.hpp"
#include "ImGuizmo.h"
#include "Vortex/Math/Math.hpp"

namespace Vortex {

	extern const std::filesystem::path g_assetPath;

    EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_viewportPanelSize({ 1600, 900 }), m_viewportBounds(),
        m_viewportFocused(false), m_viewportHovered(false), m_gizmoType(-1), 
		m_sceneState(SceneState::Edit), m_isPaused(false), m_showColliders(false) {}

    void EditorLayer::OnAttach() {
        Layer::OnAttach();

        /*m_texture1 = Texture2D::Create("res/textures/img3.png");
        m_texture2 = Texture2D::Create("res/textures/img2.png");*/

        m_frameBuffer = FrameBuffer::Create(
            { 1600, 900, 1, 
            { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth }, 
            false }
        );

		m_playIcon = Texture2D::Create("res/icons/play-button.png");
		m_simulateIcon = Texture2D::Create("res/icons/simulate-button.png");
		m_stopIcon = Texture2D::Create("res/icons/stop-button.png");
		m_pauseIcon = Texture2D::Create("res/icons/pause-button.png");

        m_currentScene = CreateRef<Scene>();
        m_sceneHierarchyPanel.SetContext(m_currentScene);

		auto commandLineArgs = Application::Get().GetCommandlineArgs();
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(m_currentScene);
			serializer.Deserialize(sceneFilePath);
		}

        m_editorCamera = EditorCamera(30.0f, 1.778f, 0.01f, 1000.0f);

        m_scenes.push_back(m_currentScene);
    }

    void EditorLayer::OnDetach() {
        Layer::OnDetach();
    }

    void EditorLayer::OnUpdate(Timestep ts) {
        VT_PROFILE_FUNC();
		m_frametime = ts;

		if (FrameBufferSpec spec = m_frameBuffer->GetSpec();
			m_viewportPanelSize.x > 0.0f && m_viewportPanelSize.y > 0.0f &&
			(spec.width != m_viewportPanelSize.x || spec.height != m_viewportPanelSize.y)) {
			m_frameBuffer->Resize(static_cast<uint32>(m_viewportPanelSize.x), static_cast<uint32>(m_viewportPanelSize.y));
			m_editorCamera.SetViewportSize(m_viewportPanelSize.x, m_viewportPanelSize.y);
			m_currentScene->OnViewportResize((uint32)m_viewportPanelSize.x, (uint32)m_viewportPanelSize.y);
		}

		Renderer2D::ResetStats();

		{
			VT_PROFILE_SCOPE("Screen Prep");
			m_frameBuffer->Bind();
			Render::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			Render::Clear();

			m_frameBuffer->ClearAttachment(1, -1);
		}

		{
			VT_PROFILE_SCOPE("Draw all");

			if (!m_isPaused) {
				switch (m_sceneState) {
				case SceneState::Play:
					m_currentScene->OnUpdate(ts);
					break;
				case SceneState::Simulate:
					m_editorCamera.OnUpdate(ts);
					m_currentScene->OnUpdateSimulation(ts, m_editorCamera);
					break;
				case SceneState::Edit:
					m_editorCamera.OnUpdate(ts);
					m_currentScene->OnUpdateEditor(ts, m_editorCamera);
					break;
				}
			}
			else {
				m_editorCamera.OnUpdate(ts);
				m_currentScene->OnUpdateEditor(ts, m_editorCamera);
			}


			auto [mx, my] = ImGui::GetMousePos();
			mx -= m_viewportBounds[0].x;
			my -= m_viewportBounds[0].y;
			glm::vec2 viewportSize = m_viewportBounds[1] - m_viewportBounds[0];
			my = viewportSize.y - my;
			int mouseX = (int)mx;
			int mouseY = (int)my;

			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
				int pixelData = m_frameBuffer->ReadPixel(1, mouseX, mouseY);
				m_hoveredEntity = pixelData == -1 ? Entity{} : Entity{ (entt::entity)pixelData, m_currentScene.get() };
				VT_CORE_INFO("EditorLayer::OnUpdate pixel data : {0}", pixelData);
			}

			OnOverlayRender();

			m_frameBuffer->Unbind();
		}
    }

    void EditorLayer::OnImGuiRender() {
        VT_PROFILE_FUNC();
        VT_CORE_TRACE("EditorLayer::OnImGuiRender");
        Layer::OnImGuiRender();

        static bool open = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Dock Space", &open, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 350.0f;

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New", "Ctrl+N")) {
                    NewScene();
                }

                if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                    OpenScene();
                }

                if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S")) {
                    SaveSceneAs();
                }

                if (ImGui::MenuItem("Exit")) {
                    Application::Get().Close();
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        m_sceneHierarchyPanel.OnImGuiRender();
		m_contentBrowserPanel.OnImGuiRender();

		ImGui::Begin("Settings");

		ImGui::Checkbox("Show Colliders", &m_showColliders);

		ImGui::End();

		{
			ImGui::Begin("Debug");

			std::string state;
			switch (m_sceneState) {
			case SceneState::Play:
				state = "Play";
				break;
			case SceneState::Simulate:
				state = "Simulate";
				break;
			case SceneState::Edit:
				state = "Edit";
				break;
			}

			ImGui::Text("Scene State %s", state.c_str());
			ImGui::Text("Paused %s", m_isPaused ? "Yes" : "No");
			if (m_hoveredEntity) {
				ImGui::Text("Hovered Entity %s, %i", m_hoveredEntity.GetName().c_str(), m_hoveredEntity.GetUUID());
			}
			if (Entity selected = m_sceneHierarchyPanel.GetSelectedEntity()) {
				auto selectedName = selected.GetName();
				auto selectedID = selected.GetUUID();
				ImGui::Text("Selected Entity %s %i", selectedName.c_str(), selectedID);
			}

			ImGui::End();
		}

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");
        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();
        m_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
        m_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

        m_viewportFocused = ImGui::IsWindowFocused();
        m_viewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_viewportFocused && !m_viewportHovered);

        ImVec2 size = ImGui::GetContentRegionAvail();
        m_viewportPanelSize = { size.x, size.y };

        uint32 texID = m_frameBuffer->GetColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(texID), ImVec2{ m_viewportPanelSize.x, m_viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path filePath = std::filesystem::path(path);

				if (filePath.extension().string() == ".vortex")
				{
					OpenScene(std::filesystem::path(g_assetPath) / path);
				}
				else if (filePath.extension().string() == ".png")
				{
					std::filesystem::path texturePath = std::filesystem::path(g_assetPath) / path;
					Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
					if (texture->IsLoaded())
					{
						if (m_hoveredEntity && m_hoveredEntity.HasComponent<SpriteComponent>())
							m_hoveredEntity.GetComponent<SpriteComponent>().Texture = texture;
					}
					else
					{
						VT_CL_WARN("Could not load texture {0}", texturePath.filename().string());
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

        Entity selectedEntity = m_sceneHierarchyPanel.GetSelectedEntity();
        if (selectedEntity && m_gizmoType != -1) {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            ImGuizmo::SetRect(m_viewportBounds[0].x, m_viewportBounds[0].y,
                m_viewportBounds[1].x - m_viewportBounds[0].x,
                m_viewportBounds[1].y - m_viewportBounds[0].y);

            const glm::mat4& cameraProjection = m_editorCamera.GetProjection();
            glm::mat4 cameraView = m_editorCamera.GetViewMatrix();

            auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = transformComponent.GetTransform();

            bool snap = Input::IsKeyPressed(Key::LeftControl);
            float snapVal = 0.5f;

            if (m_gizmoType == ImGuizmo::OPERATION::ROTATE) {
                snapVal = 45.0f;
            }

            float snapVals[3] = {snapVal, snapVal, snapVal};

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), 
                (ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), 
                nullptr, snap ? snapVals : nullptr);

            if (ImGuizmo::IsUsing()) {
                glm::vec3 translation, rotation, scale;
                Math::DecomposeTransform(transform, translation, rotation, scale);

                glm::vec3 deltaRotation = rotation - transformComponent.Rotation;
                transformComponent.Translation = translation;
                transformComponent.Rotation += deltaRotation;
                transformComponent.Scale = scale;
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();

		UIToolbar();

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& e) {
        VT_PROFILE_FUNC();
        VT_CORE_TRACE("EditorLayer::OnEvent {0}", e.GetName());

		if (m_sceneState == SceneState::Edit) {
			m_editorCamera.OnEvent(e);
		}

        ImGuiIO& io = ImGui::GetIO();
        e.m_handled |= e.IsInCat(EventCatMouse) & io.WantCaptureMouse;
        e.m_handled |= e.IsInCat(EventCatKeyboard) & io.WantCaptureKeyboard;

        EventDispatcher dispatcher{ e };
        dispatcher.Dispatch<KeyPressedEvent>(VT_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(VT_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
    }

	void EditorLayer::OnOverlayRender() {
		if (m_sceneState == SceneState::Play && !m_isPaused) {
			Entity camera = m_currentScene->GetPrimaryCamera();
			if (!camera) return;
			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
		}
		else {
			Renderer2D::BeginScene(m_editorCamera);
		}

		if (m_showColliders) {
			{
				auto view = m_currentScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view) {
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);
					glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
				}
			}

			{
				auto view = m_currentScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view) {
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);
					glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.01f);
				}
			}
		}

		if (Entity selected = m_sceneHierarchyPanel.GetSelectedEntity()) {
			auto transform = selected.GetComponent<TransformComponent>().GetTransform();
			Renderer2D::DrawRect(transform, glm::vec4{1.0f, 1.0f, 0.0f, 1.0f});
		}

		Renderer2D::EndScene();
	}

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e) {
        if (e.IsRepeat()) return false;

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

        switch (e.GetKeyCode()) {
            case Key::N:
                if (control) NewScene();
                break;
            case Key::O:
                if (control) OpenScene();
                break;
            case Key::S:
				if (control) {
					if (shift) {
						SaveSceneAs();
					}
					else {
						SaveScene();
					}
				}
                break;
			case Key::D:
				if (control) {
					OnDuplicateEntity();
				}

            // Gizmos controls
            case Key::Q:
                if (!ImGuizmo::IsUsing()) {
                    m_gizmoType = -1;
                }
                break;
            case Key::W:
                if (!ImGuizmo::IsUsing()) {
                    m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
                }
                break;
            case Key::E:
                if (!ImGuizmo::IsUsing()) {
                    m_gizmoType = ImGuizmo::OPERATION::ROTATE;
                }
                break;
            case Key::R:
                if (!ImGuizmo::IsUsing()) {
                    m_gizmoType = ImGuizmo::OPERATION::SCALE;
                }
                break;
        }
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
        if (e.GetMouseButton() == Mouse::ButtonLeft) {
            if (m_viewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt)) {
                m_sceneHierarchyPanel.SetSelectedEntity(m_hoveredEntity);
            }
        }
        return false;
    }

	void EditorLayer::OnScenePlay() {
		if (m_sceneState == SceneState::Simulate) OnSceneStop();

		m_sceneState = SceneState::Play;

		m_currentScene = Scene::Copy(m_editorScene);
		m_sceneHierarchyPanel.SetContext(m_currentScene);
		m_currentScene->OnRuntimStart();
	}

	void EditorLayer::OnSceneSimulate() {
		if (m_sceneState == SceneState::Play) OnSceneStop();

		m_sceneState = SceneState::Simulate;

		m_currentScene = Scene::Copy(m_editorScene);
		m_sceneHierarchyPanel.SetContext(m_currentScene);
		m_currentScene->OnSimulateStart();
	}

	void EditorLayer::OnSceneResume() {
		m_isPaused = false;
		if (m_sceneState == SceneState::Play) {
			m_currentScene->OnRuntimeResume();
		}
		else if (m_sceneState == SceneState::Simulate) {
			m_currentScene->OnSimulateResume();
		}
		else {
			OnSceneStop();
		}
	}

	void EditorLayer::OnScenePause() {
		m_isPaused = true;
		if (m_sceneState == SceneState::Play) {
			m_currentScene->OnRuntimePause();
		}
		else if (m_sceneState == SceneState::Simulate) {
			m_currentScene->OnSimulatePause();
		}
		else {
			OnSceneStop();
		}
	}

	void EditorLayer::OnSceneStop() {
		m_isPaused = false;
		if (m_sceneState == SceneState::Play) {
			m_currentScene->OnRuntimeStop();
		}
		else if (m_sceneState == SceneState::Simulate) {
			m_currentScene->OnSimulateStop();
		}
		m_sceneState = SceneState::Edit;

		m_currentScene = m_editorScene;
		m_sceneHierarchyPanel.SetContext(m_currentScene);
	}

	void EditorLayer::OnDuplicateEntity() {
		if (m_sceneState != SceneState::Edit) return;

		if (Entity selectedEntity = m_sceneHierarchyPanel.GetSelectedEntity()) {
			m_editorScene->DuplicateEntity(selectedEntity);
		}
	}

	void EditorLayer::UIToolbar() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3, 0.3, 0.3, 0.5));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		auto flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::Begin("##toolbar", nullptr, flags);

		float size = ImGui::GetWindowHeight() - 4.0f;
		bool toolbarEnabled = (bool)m_currentScene;
		ImVec4 tintColor = ImVec4(1, 1, 1, 1);
		if (!toolbarEnabled) {
			tintColor.w = 0.5f;
		}

		{
			Ref<Texture2D> icon = m_playIcon;
			if (m_sceneState == SceneState::Play && !m_isPaused) {
				icon = m_pauseIcon;
			}
			ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.01f) - (size * 0.5f));
			if (ImGui::ImageButton("Play-Pause", (ImTextureID)icon->GetID(), ImVec2(size, size),
				ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor)) {
				if (m_sceneState == SceneState::Play) {
					if (m_isPaused) {
						OnSceneResume();
					}
					else {
						OnScenePause();
					}
				}
				else if (m_sceneState == SceneState::Edit) {
					OnScenePlay();
				}
			}
			ImGui::SameLine();
		}

		{
			Ref<Texture2D> icon = m_simulateIcon;
			if (m_sceneState == SceneState::Simulate && !m_isPaused) {
				icon = m_pauseIcon;
			}
			//ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.15f) - (size * 0.5f));
			if (ImGui::ImageButton("Simulate-Pause", (ImTextureID)icon->GetID(), ImVec2(size, size),
				ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor)) {
				if (m_sceneState == SceneState::Simulate) {
					if (m_isPaused) {
						OnSceneResume();
					}
					else {
						OnScenePause();
					}
				}
				else if (m_sceneState == SceneState::Edit) {
					OnSceneSimulate();
				}
			}
			ImGui::SameLine();
		}

		//ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.02f) - (size * 0.5f));
		if (ImGui::ImageButton("Stop", (ImTextureID)m_stopIcon->GetID(), ImVec2(size, size), 
			ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor)) {
			if (m_sceneState == SceneState::Play || m_sceneState == SceneState::Simulate) {
				OnSceneStop();
			}
		}


		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::NewScene() {
		if (m_sceneState != SceneState::Edit) return;

        m_editorScene = CreateRef<Scene>();
		m_editorScenePath = std::filesystem::path();
		m_editorScene->OnViewportResize((uint32)m_viewportPanelSize.x, (uint32)m_viewportPanelSize.y);
        m_sceneHierarchyPanel.SetContext(m_editorScene);
        
		m_currentScene = m_editorScene;
    }

    void EditorLayer::OpenScene() {
		if (m_sceneState != SceneState::Edit) {
			OnSceneStop();
		}

        auto filepath = FileIO::OpenFile("Vortex Scene (*.vortex)\0*.vortex\0");
        if (!filepath.empty()) {
			OpenScene(filepath);
		}
    }

	void EditorLayer::OpenScene(const std::filesystem::path& path) {
		if (path.extension().string() != ".vortex") {
			VT_CL_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer{ newScene };
		if (serializer.Deserialize(path.string())) {
			m_editorScene = newScene;
			m_editorScene->OnViewportResize((uint32)m_viewportPanelSize.x, (uint32)m_viewportPanelSize.y);
			m_sceneHierarchyPanel.SetContext(m_editorScene);
			m_editorScenePath = path;

			m_currentScene = m_editorScene;
		}

		m_cameraEntity = m_currentScene->GetPrimaryCamera();
		if (m_cameraEntity) {
			m_cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		}
	}

	void EditorLayer::SaveScene() {
		if (!m_editorScenePath.empty()) {
			SerializeScene(m_currentScene, m_editorScenePath);
		}
		else {
			SaveSceneAs();
		}
	}

    void EditorLayer::SaveSceneAs() {
        auto filepath = FileIO::SaveFile("Vortex Scene (*.vortex)\0*.vortex\0");
        if (!filepath.empty()) {
			SerializeScene(m_currentScene, filepath);
			m_editorScenePath = filepath;
        }
    }

	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path) {
		SceneSerializer serializer(scene);
		serializer.Serialize(path.string());
	}

}

#include "EditorLayer.hpp"
#include "Platforms/OpenGL/OpenGLShader.hpp"
#include "Vortex/Controllers/CameraController.hpp"
#include "Vortex/Scene/SceneSerializer.hpp"
#include "Vortex/Utils/FileIO.hpp"
#include "ImGuizmo.h"
#include "Vortex/Math/Math.hpp"

namespace Vortex {

	extern const std::filesystem::path g_assetPath;

    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_viewportPanelSize({ 1600, 900 }),
        m_viewportFocused(false), m_viewportHovered(false), m_gizmoType(-1), m_sceneState(SceneState::Edit) {}

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
		m_stopIcon = Texture2D::Create("res/icons/stop-button.png");

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

			switch (m_sceneState) {
			case SceneState::Play:
				m_currentScene->OnUpdate(ts);
				break;
			case SceneState::Edit:
				m_editorCamera.OnUpdate(ts);
				m_currentScene->OnUpdateEditor(ts, m_editorCamera);
				break;
			}

            //m_currentScene->OnUpdateEditor(ts, m_editorCamera);

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
				OpenScene(std::filesystem::path(g_assetPath) / path);
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

        m_editorCamera.OnEvent(e);

        ImGuiIO& io = ImGui::GetIO();
        e.m_handled |= e.IsInCat(EventCatMouse) & io.WantCaptureMouse;
        e.m_handled |= e.IsInCat(EventCatKeyboard) & io.WantCaptureKeyboard;

        EventDispatcher dispatcher{ e };
        dispatcher.Dispatch<KeyPressedEvent>(VT_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(VT_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e) {
        if (e.GetRepeatCount() > 0) return false;

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

        switch (e.GetKeyCode()) {
            case Key::N: {
                if (control) NewScene();
                break;
            }
            case Key::O: {
                if (control) OpenScene();
                break;
            }
            case Key::S: {
                if (control && shift) SaveSceneAs();
                break;
            }
            // Gizmos controls
            case Key::Q: {
                if (!ImGuizmo::IsUsing()) {
                    m_gizmoType = -1;
                }
                break;
            }
            case Key::W: {
                if (!ImGuizmo::IsUsing()) {
                    m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
                }
                break;
            }
            case Key::E: {
                if (!ImGuizmo::IsUsing()) {
                    m_gizmoType = ImGuizmo::OPERATION::ROTATE;
                }
                break;
            }
            case Key::R: {
                if (!ImGuizmo::IsUsing()) {
                    m_gizmoType = ImGuizmo::OPERATION::SCALE;
                }
                break;
            }
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
		m_sceneState = SceneState::Play;
		m_currentScene->OnRuntimStart();
	}

	void EditorLayer::OnSceneStop() {
		m_sceneState = SceneState::Edit;
		m_currentScene->OnRuntimeStop();
	}

	void EditorLayer::UIToolbar() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		auto flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::Begin("##toolbar", nullptr, flags);

		float size = ImGui::GetWindowHeight() - 4.0f;
		Ref<Texture2D> icon = m_sceneState == SceneState::Edit ? m_playIcon : m_stopIcon;
		ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)icon->GetID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (m_sceneState == SceneState::Edit)
				OnScenePlay();
			else if (m_sceneState == SceneState::Play)
				OnSceneStop();
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::NewScene() {
        m_currentScene = CreateRef<Scene>();
        m_currentScene->OnViewportResize((uint32)m_viewportPanelSize.x, (uint32)m_viewportPanelSize.y);
        m_sceneHierarchyPanel.SetContext(m_currentScene);
    }

    void EditorLayer::OpenScene() {
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
			m_currentScene = newScene;
			m_currentScene->OnViewportResize((uint32)m_viewportPanelSize.x, (uint32)m_viewportPanelSize.y);
			m_sceneHierarchyPanel.SetContext(m_currentScene);
		}
	}

    void EditorLayer::SaveSceneAs() {
        auto filepath = FileIO::SaveFile("Vortex Scene (*.vortex)\0*.vortex\0");
        if (!filepath.empty()) {
            SceneSerializer serializer{ m_currentScene };
            serializer.Serialize(filepath);
        }
    }

}

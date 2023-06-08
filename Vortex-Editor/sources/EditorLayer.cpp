#include "EditorLayer.hpp"
#include "Platforms/OpenGL/OpenGLShader.hpp"
#include "Vortex/Controllers/CameraController.hpp"

namespace Vortex {

    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_cameraController(16.0f / 9.0f, true), m_viewportPanelSize({ 1600, 900 }),
        m_viewportFocused(false), m_viewportHovered(false) {}

    void EditorLayer::OnAttach() {
        Layer::OnAttach();

        m_texture1 = Texture2D::Create("res/textures/img3.png");
        m_texture2 = Texture2D::Create("res/textures/img2.png");

        m_frameBuffer = FrameBuffer::Create({ 1600, 900, 1, false });

        m_currentScene = CreateRef<Scene>();
        m_sceneHierarchyPanel.SetContext(m_currentScene);

        auto square = m_currentScene->CreateEntity("White Square");
        square.AddComponent<SpriteComponent>(glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});

        auto rsquare = m_currentScene->CreateEntity("Red Square");
        rsquare.AddComponent<SpriteComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        m_square = square;

        m_scenes.push_back(m_currentScene);

        m_primaryCamera = m_currentScene->CreateEntity("Primary camera");
        m_primaryCamera.AddComponent<CameraComponent>();
        m_primaryCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
    }

    void EditorLayer::OnDetach() {
        Layer::OnDetach();
    }

    void EditorLayer::OnUpdate(Timestep ts) {
        VT_PROFILE_FUNC();
        m_frametime = ts;

        if (FrameBufferSpec spec = m_frameBuffer->GetSpec();
            m_viewportPanelSize.x > 0.0f && m_viewportPanelSize.y > 0.0f &&
            (spec.width != m_viewportPanelSize.x || spec.height != m_viewportPanelSize.y))
        {
            m_frameBuffer->Resize(static_cast<uint32>(m_viewportPanelSize.x), static_cast<uint32>(m_viewportPanelSize.y));
            m_cameraController.OnResize(m_viewportPanelSize.x, m_viewportPanelSize.y);
            m_currentScene->OnViewportResize((uint32)m_viewportPanelSize.x, (uint32)m_viewportPanelSize.y);
        }

        Renderer2D::ResetStats();

        {
            VT_PROFILE_SCOPE("CameraController::OnUpdate");
            if (m_viewportFocused) {
                m_cameraController.OnUpdate(ts);
            }
        }

        {
            VT_PROFILE_SCOPE("Screen Prep");
            m_frameBuffer->Bind();
            Render::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
            Render::Clear();
        }

        {
            VT_PROFILE_SCOPE("Draw all");
            Renderer2D::BeginScene(m_cameraController.GetCamera());

            m_currentScene->OnUpdate(ts);

            Renderer2D::EndScene();

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

        static bool statsOpen = false;
        static bool testOpen = false;

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
                if (ImGui::MenuItem("Exit")) {
                    Application::Get().Close();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Stats")) {
                if (ImGui::MenuItem("BatchRender stats", nullptr, &statsOpen)) {}
                if (ImGui::MenuItem("Close", nullptr, false)) {
                    statsOpen = false;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Test")) {
                if (ImGui::MenuItem("ECS test", nullptr, &testOpen)) {}
                if (ImGui::MenuItem("Close", nullptr, false)) {
                    testOpen = false;
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        m_sceneHierarchyPanel.OnImGuiRender();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");

        m_viewportFocused = ImGui::IsWindowFocused();
        m_viewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_viewportFocused || !m_viewportHovered);

        ImVec2 size = ImGui::GetContentRegionAvail();
        m_viewportPanelSize = { size.x, size.y };

        uint32 texID = m_frameBuffer->GetColorAttachmentID();
        ImGui::Image(reinterpret_cast<void*>(texID), ImVec2{ m_viewportPanelSize.x, m_viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        ImGui::End();

        if (statsOpen) {
            auto stats = Renderer2D::GetStats();

            ImGui::Begin("Stats");

            ImGui::Text("Renderer2D stats:");
            ImGui::Text("Draw Calls: %d", stats.drawcallsCount);
            ImGui::Text("Quads: %d", stats.quadCount);
            ImGui::Text("Vertices: %d", stats.GetVertexesCount());
            ImGui::Text("Indices: %d", stats.GetIndicesCount());
            
            ImGui::Separator();
            
            ImGui::Text("Frame time: %0.2f", m_frametime.GetMilliSeconds());

            ImGui::End();
        }
        if (testOpen) {
            
        }

        ImGui::PopStyleVar();
        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& e) {
        VT_PROFILE_FUNC();
        VT_CORE_TRACE("EditorLayer::OnEvent {0}", e.GetName());

        ImGuiIO& io = ImGui::GetIO();
        e.m_handled |= e.IsInCat(EventCatMouse) & io.WantCaptureMouse;
        e.m_handled |= e.IsInCat(EventCatKeyboard) & io.WantCaptureKeyboard;

        m_cameraController.OnEvent(e);
    }

}

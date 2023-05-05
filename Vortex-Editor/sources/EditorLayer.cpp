#include "EditorLayer.hpp"
#include "Platforms/OpenGL/OpenGLShader.hpp"

EditorLayer::EditorLayer()
: Layer("EditorLayer"), m_cameraController(16.0f / 9.0f, true), m_viewportPanelSize({1600, 900}),
 m_viewportFocused(false), m_viewportHovered(false) {}

void EditorLayer::OnAttach() {
	Layer::OnAttach();

	m_texture1 = Vortex::Texture2D::Create("res/textures/img3.png");
	m_texture2 = Vortex::Texture2D::Create("res/textures/img2.png");

    m_frameBuffer = Vortex::FrameBuffer::Create({1600, 900, 1, false});
}

void EditorLayer::OnDetach() {
	Layer::OnDetach();
}

void EditorLayer::OnUpdate(Vortex::Timestep ts) {
	VT_PROFILE_FUNC();

	Vortex::Renderer2D::ResetStats();

	{
		VT_PROFILE_SCOPE("CameraController::OnUpdate");
        if (m_viewportFocused) {
			m_cameraController.OnUpdate(ts);
        }
	}

	{
		VT_PROFILE_SCOPE("Screen Prep");
        m_frameBuffer->Bind();
		Vortex::Render::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Vortex::Render::Clear();
	}

	{
		VT_PROFILE_SCOPE("Draw all");
		static float rot = 0.0f;
		rot += ts * 50.0f;
		if (rot > 360.0f) { rot -= 360.0f; }

		Vortex::Renderer2D::BeginScene(m_cameraController.GetCamera());

		Vortex::Renderer2D::DrawQuad({ -0.5f, 0.0f}, { 1.0f, 1.0f }, m_texture2, 1.0f, {0.8f, 0.2f, 0.3f, 1.0f});
		Vortex::Renderer2D::DrawRotatedQuad({ 0.5f, 0.0f }, { 1.0f, 1.0f }, m_texture1, rot);
		Vortex::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Vortex::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Vortex::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, -45.0f);

		Vortex::Renderer2D::EndScene();

        Vortex::Renderer2D::BeginScene(m_cameraController.GetCamera());

        float f = 5.0f;
        for (float y = -f; y < f; y += 0.5f) {
            for (float x = -f; x < f; x += 0.5f) {
                glm::vec4 color = { (x + f) / (2 * f), 0.4f, (y + f) / (2 * f), 0.7f };
                Vortex::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
            }
        }

        Vortex::Renderer2D::EndScene();

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
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                Vortex::Application::Get().Close();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options")) {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false))
                open = false;
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Stats")) {
            if (ImGui::MenuItem("BatchRender stats", nullptr, &statsOpen));
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Viewport");

    m_viewportFocused = ImGui::IsWindowFocused();
    m_viewportHovered = ImGui::IsWindowHovered();
    Vortex::Application::Get().GetImGuiLayer()->BlockEvents(!m_viewportFocused || !m_viewportHovered);

    ImVec2 size = ImGui::GetContentRegionAvail();

    if (m_viewportPanelSize.x != size.x || m_viewportPanelSize.y != size.y) {
        m_frameBuffer->Resize(static_cast<uint32>(size.x), static_cast<uint32>(size.x));
        m_viewportPanelSize = { size.x, size.y };
        m_cameraController.OnResize(size.x, size.y);

    }
    uint32 texID = m_frameBuffer->GetColorAttachmentID();
    ImGui::Image(reinterpret_cast<void*>(texID), ImVec2{ m_viewportPanelSize.x, m_viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::End();
    ImGui::PopStyleVar();

    if (statsOpen) {
        auto stats = Vortex::Renderer2D::GetStats();

        ImGui::Begin("Settings");

        ImGui::Text("Renderer2D stats:");
        ImGui::Text("Draw Calls: %d", stats.drawcallsCount);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.GetVertexesCount());
        ImGui::Text("Indices: %d", stats.GetIndicesCount());

        ImGui::End();
    }

    ImGui::End();
}

void EditorLayer::OnEvent(Vortex::Event& e) {
	VT_PROFILE_FUNC();
	VT_CORE_TRACE("EditorLayer::OnEvent {0}", e.GetName());

    ImGuiIO& io = ImGui::GetIO();
    e.m_handled |= e.IsInCat(Vortex::EventCatMouse) & io.WantCaptureMouse;
    e.m_handled |= e.IsInCat(Vortex::EventCatKeyboard) & io.WantCaptureKeyboard;

	m_cameraController.OnEvent(e);
}

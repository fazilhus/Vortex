#include "Sandbox2D.hpp"
#include "Platforms/OpenGL/OpenGLShader.hpp"

Sandbox2D::Sandbox2D()
: Layer("Sandbox2D"), m_cameraController(16.0f / 9.0f, true) {}

void Sandbox2D::OnAttach() {
	Layer::OnAttach();

	m_texture1 = Vortex::Texture2D::Create("res/textures/img3.png");
	m_texture2 = Vortex::Texture2D::Create("res/textures/img2.png");
}

void Sandbox2D::OnDetach() {
	Layer::OnDetach();
}

void Sandbox2D::OnUpdate(Vortex::Timestep ts) {
	VT_PROFILE_FUNC();

	Vortex::Renderer2D::ResetStats();

	{
		VT_PROFILE_SCOPE("CameraController::OnUpdate");
		m_cameraController.OnUpdate(ts);
	}

	{
		VT_PROFILE_SCOPE("Screen Prep");
		Vortex::Render::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Vortex::Render::Clear();
	}

	{
		VT_PROFILE_SCOPE("Draw all");
		static float rot = 0.0f;
		rot += ts * 50.0f;
		if (rot > 360.0f) { rot -= 360.0f; }

		Vortex::Renderer2D::BeginScene(m_cameraController.GetCamera());

		Vortex::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.01f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, -45.0f);
		Vortex::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Vortex::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Vortex::Renderer2D::DrawRotatedQuad({ 0.5f, 0.0f, 0.01f }, { 1.0f, 1.0f }, m_texture1, rot);
		Vortex::Renderer2D::DrawQuad({ -0.5f, 0.0f, 0.02f }, { 1.0f, 1.0f }, m_texture2, 1.0f, {0.8f, 0.2f, 0.3f, 1.0f});

		Vortex::Renderer2D::EndScene();

		Vortex::Renderer2D::BeginScene(m_cameraController.GetCamera());

		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Vortex::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}

		Vortex::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender() {
	VT_PROFILE_FUNC();
	VT_CORE_TRACE("Sandbox2D::OnImGuiRender");
	Layer::OnImGuiRender();

	auto stats = Vortex::Renderer2D::GetStats();

	ImGui::Begin("Settings");

	ImGui::Text("Renderer2D stats:");
	ImGui::Text("Draw Calls: %d", stats.drawcallsCount);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.GetVertexesCount());
	ImGui::Text("Indices: %d", stats.GetIndicesCount());

	ImGui::End();
}

void Sandbox2D::OnEvent(Vortex::Event& e) {
	VT_PROFILE_FUNC();
	VT_CORE_TRACE("Sandbox2D::OnEvent");
	m_cameraController.OnEvent(e);
}

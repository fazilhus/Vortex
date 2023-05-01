#include "Sandbox2D.hpp"
#include "Platforms/OpenGL/OpenGLShader.hpp"

Sandbox2D::Sandbox2D()
: Layer("Sandbox2D"), m_cameraController(16.0f / 9.0f, true) {}

void Sandbox2D::OnAttach() {
	Layer::OnAttach();

	m_texture = Vortex::Texture2D::Create("res/textures/img3.png");
}

void Sandbox2D::OnDetach() {
	Layer::OnDetach();
}

void Sandbox2D::OnUpdate(Vortex::Timestep ts) {
	VT_PROFILE_FUNCTION();
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
		Vortex::Renderer2D::BeginScene(m_cameraController.GetCamera());

		Vortex::Renderer2D::DrawQuad({ -0.6f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Vortex::Renderer2D::DrawQuad({ 0.5f, -0.375f }, { 0.5f, 0.75f }, { 0.2f, 0.8f, 0.3f, 1.0f });
		Vortex::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 2.0f, 1.5f }, { 0.9f, 0.9f, 0.9f, 1.0f });
		Vortex::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f }, { 2.0f, 1.5f }, m_texture);

		Vortex::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender() {
	VT_PROFILE_FUNCTION();
	Layer::OnImGuiRender();
}

void Sandbox2D::OnEvent(Vortex::Event& e) {
	m_cameraController.OnEvent(e);
}

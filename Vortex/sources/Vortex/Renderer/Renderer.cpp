#include "vtpch.hpp"

#include "Platforms/OpenGL/OpenGLShader.hpp"


namespace Vortex {

	Renderer::SceneData* Renderer::s_sceneData = new Renderer::SceneData();

	void Renderer::Init(const RendererConfig& cfg) {
		VT_PROFILE_FUNCTION();
		Render::Init(cfg);
		Renderer2D::Init();
		VT_CORE_INFO("Renderer is initialized");
	}

	void Renderer::Shutdown() {
		VT_PROFILE_FUNCTION();
		Renderer2D::Shutdown();
		VT_CORE_INFO("Renderer is terminated");
	}

	void Renderer::OnWindowResize(uint width, uint height) {
		VT_PROFILE_FUNCTION();
		Render::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthoCamera& cam) {
		VT_PROFILE_FUNCTION();
		s_sceneData->viewproj = cam.GetViewProjMat();
	}

	void Renderer::EndScene() {
		VT_PROFILE_FUNCTION();
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& va, const glm::mat4& transform) {
		VT_PROFILE_FUNCTION();
		shader->Bind();
		shader->SetMat4("u_viewproj", s_sceneData->viewproj);
		shader->SetMat4("u_transform", transform);

		va->Bind();
		Render::DrawIndexed(va);
	}

}
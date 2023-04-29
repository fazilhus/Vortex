#include "vtpch.hpp"

#include "Platforms/OpenGL/OpenGLShader.hpp"


namespace Vortex {

	Renderer::SceneData* Renderer::s_sceneData = new Renderer::SceneData();

	void Renderer::Init(const RendererConfig& cfg) {
		Render::Init(cfg);
	}

	void Renderer::OnWindowResize(uint width, uint height) {
		Render::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthoCamera& cam) {
		s_sceneData->viewproj = cam.GetViewProjMat();
	}

	void Renderer::EndScene() {
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& va, const glm::mat4& transform) {
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_viewproj", s_sceneData->viewproj);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_transform", transform);

		va->Bind();
		Render::DrawIndexed(va);
	}

}
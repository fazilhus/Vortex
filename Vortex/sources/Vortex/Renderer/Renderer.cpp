#include "vtpch.hpp"
#include "Vortex/Renderer/Renderer.hpp"

#include "Platforms/OpenGL/OpenGLShader.hpp"


namespace Vortex {

	Renderer::SceneData* Renderer::s_sceneData = new Renderer::SceneData();

	void Renderer::BeginScene(OrthoCamera& cam) {
		s_sceneData->viewproj = cam.GetViewProjMat();
	}

	void Renderer::EndScene() {
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, 
		const std::shared_ptr<VertexArray>& va, const glm::mat4& transform) {
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_viewproj", s_sceneData->viewproj);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_transform", transform);

		va->Bind();
		Render::DrawIndexed(va);
	}

}
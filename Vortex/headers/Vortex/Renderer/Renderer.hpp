#pragma once
#include "vtpch.hpp"

#include "Vortex/Renderer/Shader.hpp"
#include "Vortex/Renderer/Cameras/OrthoCamera.hpp"

#include <glm/glm.hpp>

namespace Vortex {

	class Renderer {
	private:
		struct SceneData {
			glm::mat4 viewproj;
		};
		static SceneData* s_sceneData;

	public:
		static void Init();

		static void BeginScene(OrthoCamera& cam);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, 
			const std::shared_ptr<VertexArray>& va, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}
#pragma once
#include "Vortex/Renderer/RendererAPI.hpp"
#include "Vortex/Renderer/Cameras/OrthoCamera.hpp"

#include <glm/glm.hpp>

namespace Vortex {

	struct RendererConfig {
		bool enableBlending;
		bool enableDepthTest;
	};

	class Renderer {
	private:
		struct SceneData {
			glm::mat4 viewproj;
		};
		static SceneData* s_sceneData;

	public:
		static void Init(const RendererConfig& cfg);
		static void Shutdown();

		static void OnWindowResize(uint width, uint height);

		static void BeginScene(OrthoCamera& cam);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& va, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}
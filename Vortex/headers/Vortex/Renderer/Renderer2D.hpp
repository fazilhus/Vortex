#pragma once
#include "vtpch.hpp"

#include "Vortex/Renderer/Texture.hpp"

namespace Vortex {

	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthoCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Vortex::Texture2D>& texture, 
			float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Vortex::Texture2D>& texture,
			float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, float rot = 0.0f);
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float rot = 0.0f);
		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Vortex::Texture2D>& texture,
			float rot = 0.0f, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Vortex::Texture2D>& texture,
			float rot = 0.0f, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));
	};

}

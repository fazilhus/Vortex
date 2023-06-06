#pragma once
#include <vtpch.hpp>

#include "Vortex/Renderer/Texture.hpp"
#include "Vortex/Renderer/Cameras/Camera.hpp"

namespace Vortex {

	struct QuadVertex {
		glm::vec3 pos;
		glm::vec4 color;
		glm::vec2 texPos;
		float texInd;
		float tilingFactor;
	};

	struct RendererStatisics {
		uint32 drawcallsCount = 0;
		uint32 quadCount = 0;

		uint32 GetVertexesCount() const { return quadCount * 4; }
		uint32 GetIndicesCount() const { return quadCount * 6; }
	};

	struct Renderer2DStorage {
		static const uint32 maxQuads = 50000;
		static const uint32 maxVert = maxQuads * 4;
		static const uint32 maxInd = maxQuads * 6;
		static const uint32 maxTextureSLots = 32;

		Ref<VertexArray> quadVao;
		Ref<VertexBuffer> quadVbo;
		Ref<Shader> shader;
		Ref<Texture2D> texture;

		uint32 quadIndCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		Array<Ref<Texture2D>, maxTextureSLots> texSlots;
		uint32 texSlotInd = 1;

		glm::vec4 quadVertexPos[4];

		RendererStatisics stats;
	};

	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4 projection);
		static void BeginScene(const OrthoCamera& camera);
		static void EndScene();

		static void Flush();

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture,  float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, float rot = 0.0f);
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float rot = 0.0f);
		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture, float rot = 0.0f, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture, float rot = 0.0f, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		static void ResetStats();
		static RendererStatisics GetStats();

	private:
		//static void SetQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float texIndex, float tilingFactor, float rot);

		static void FlushAndReset();
	};

}

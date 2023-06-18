#pragma once
#include <vtpch.hpp>

#include "Vortex/Renderer/Texture.hpp"
#include "Vortex/Renderer/Cameras/Camera.hpp"
#include "Vortex/Renderer/Cameras/EditorCamera.hpp"
#include "Vortex/Scene/Components.hpp"

namespace Vortex {

	struct QuadVertex {
		glm::vec3 pos;
		glm::vec4 color;
		glm::vec2 texPos;
		float texInd;
		float tilingFactor;

		// Editor only
		int entityID;
	};

	struct CircleVertex {
		glm::vec3 worldPos;
		glm::vec3 localPos;
		glm::vec4 color;
		float thickness;
		float fade;

		// Editor only
		int entityID;
	};

	struct LineVertex {
		glm::vec3 pos;
		glm::vec4 color;

		// Editor only
		int entityID;
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
		Ref<Shader> quadShader;
		Ref<Texture2D> texture;

		Ref<VertexArray> circleVao;
		Ref<VertexBuffer> circleVbo;
		Ref<Shader> circleShader;

		Ref<VertexArray> lineVao;
		Ref<VertexBuffer> lineVbo;
		Ref<Shader> lineShader;

		uint32 quadIndCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		uint32 circleIndCount = 0;
		CircleVertex* circleVertexBufferBase = nullptr;
		CircleVertex* circleVertexBufferPtr = nullptr;

		uint32 lineIndCount = 0;
		LineVertex* lineVertexBufferBase = nullptr;
		LineVertex* lineVertexBufferPtr = nullptr;

		float lineWidth = 2.0f;

		Array<Ref<Texture2D>, maxTextureSLots> texSlots;
		uint32 texSlotInd = 1;

		glm::vec4 quadVertexPos[4];

		RendererStatisics stats;

		struct CameraData {
			glm::mat4 viewproj;
		};

		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4 projection);
		static void BeginScene(const OrthoCamera& camera);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		static void Flush();

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);

		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture,  float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, float rot = 0.0f);
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float rot = 0.0f);
		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture, float rot = 0.0f, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture, float rot = 0.0f, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		static void DrawSprite(const glm::mat4& transform, SpriteComponent& sprite, int entityID);
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);
		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID = -1);
		static void DrawRect(const glm::vec3& pos, const glm::vec2& dim, const glm::vec4& color, int entityID = -1);
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);

		static void ResetStats();
		static RendererStatisics GetStats();

	private:
		static void StartBatch();
		static void NextBatch();
	};

}

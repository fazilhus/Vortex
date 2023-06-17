#pragma once
#include "Vortex/Renderer/Renderer.hpp"
#include "Vortex/Renderer/RendererAPI.hpp"

namespace Vortex {

	class Render {
	private:
		static inline Scope<RendererAPI> s_rendererAPI{ RendererAPI::Create() };

	public:
		inline static void Init(const RendererConfig& cfg) {
			s_rendererAPI->Init(cfg);
		}

		inline static void SetViewport(uint4 x, uint4 y, uint4 width, uint4 height) {
			s_rendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color) {
			s_rendererAPI->SetClearColor(color);
		}

		inline static void Clear() {
			s_rendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& va, uint32 indexCount = 0) {
			s_rendererAPI->DrawIndexed(va, indexCount);
		}

		inline static void DrawLines(const Ref<VertexArray>& va, uint32 vertexCount = 0) {
			s_rendererAPI->DrawLines(va, vertexCount);
		}

		inline static void SetLineWidth(float width) {
			s_rendererAPI->SetLineWidth(width);
		}
	};

}

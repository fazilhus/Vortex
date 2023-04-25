#pragma once
#include "vtpch.hpp"

namespace Vortex {

	class Render {
	private:
		static RendererAPI* s_rendererAPI;

	public:
		inline static void SetClearColor(const glm::vec4& color) {
			s_rendererAPI->SetClearColor(color);
		}

		inline static void Clear() {
			s_rendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& va) {
			s_rendererAPI->DrawIndexed(va);
		}
	};

}
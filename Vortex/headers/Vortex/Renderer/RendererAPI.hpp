#pragma once

#include <glm/glm.hpp>

namespace Vortex {

	struct RendererConfig;

	class RendererAPI {
	public:
		enum class API {
			NONE = 0,
			OPENGL = 1
		};

	private:
		static API s_api;

	public:
		virtual ~RendererAPI() = default;
		virtual void Init(const RendererConfig& cfg) = 0;
		virtual void SetViewport(uint4 x, uint4 y, uint4 width, uint4 height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& va, uint32 count = 0) = 0;

		inline static API GetAPI() { return s_api; }

		static Scope<RendererAPI> Create();
	};

}

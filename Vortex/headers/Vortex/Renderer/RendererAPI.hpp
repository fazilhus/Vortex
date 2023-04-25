#pragma once
#include "vtpch.hpp"

#include <glm/glm.hpp>

namespace Vortex {

	class RendererAPI {
	public:
		enum class API {
			NONE = 0,
			OPENGL = 1
		};

	private:
		static API s_api;

	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& va) = 0;

		inline static API GetAPI() { return s_api; }
	};

}
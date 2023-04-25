#pragma once
#include "vtpch.hpp"

namespace Vortex {

	class Renderer {
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& va);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}
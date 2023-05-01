#pragma once
#include "vtpch.hpp"

namespace Vortex {

	class OpenGLRendererAPI : public RendererAPI {
	public:
		virtual void Init(const RendererConfig& cfg) override;
		virtual void SetViewport(uint x, uint y, uint width, uint height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Ref<VertexArray>& va) override;
	};

}
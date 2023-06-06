#pragma once
#include <vtpch.hpp>

namespace Vortex {

	void OpenGLMessageCallback(uint4 source, uint4 type, uint4 id, uint4 severity, int len, const char* msg, const void* params);

	class OpenGLRendererAPI : public RendererAPI {
	public:
		virtual void Init(const RendererConfig& cfg) override;
		virtual void SetViewport(uint4 x, uint4 y, uint4 width, uint4 height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Ref<VertexArray>& va, uint32 count = 0) override;
	};

}
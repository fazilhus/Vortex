#include "vtpch.hpp"
#include "Vortex/Renderer/Buffer.hpp"

#include "Vortex/Renderer/Renderer.hpp"

#include "Platforms/OpenGL/OpenGLBuffer.hpp"

namespace Vortex {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint size)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::NONE: {
				VT_CORE_ASSERT(false, "RendererAPI::NONE");
				return nullptr;
			}
			case RendererAPI::OPENGL: {
				return new OpenGLVertexBuffer(vertices, size);
			}
		}
	}

	IndexBuffer* IndexBuffer::Create(uint* indices, uint count)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::NONE: {
				VT_CORE_ASSERT(false, "RendererAPI::NONE");
				return nullptr;
			}
			case RendererAPI::OPENGL: {
				return new OpenGLIndexBuffer(indices, count);
			}
		}
	}

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::NONE: {
				VT_CORE_ASSERT(false, "RendererAPI::NONE");
				return nullptr;
			}
			case RendererAPI::OPENGL: {
				return new OpenGLVertexArray();
			}
		}
	}

}
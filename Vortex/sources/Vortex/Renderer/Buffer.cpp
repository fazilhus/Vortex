#include "vtpch.hpp"

#include "Platforms/OpenGL/OpenGLBuffer.hpp"

namespace Vortex {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint size)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::NONE: {
				VT_CORE_ASSERT(false, "RendererAPI::API::NONE");
				return nullptr;
			}
			case RendererAPI::API::OPENGL: {
				return new OpenGLVertexBuffer(vertices, size);
			}
		}
	}

	IndexBuffer* IndexBuffer::Create(uint* indices, uint count)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::NONE: {
				VT_CORE_ASSERT(false, "RendererAPI::API::NONE");
				return nullptr;
			}
			case RendererAPI::API::OPENGL: {
				return new OpenGLIndexBuffer(indices, count);
			}
		}
	}

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
		    case RendererAPI::API::NONE: {
				VT_CORE_ASSERT(false, "RendererAPI::API::NONE");
				return nullptr;
			}
		    case RendererAPI::API::OPENGL: {
				return new OpenGLVertexArray();
			}
		}
	}

}
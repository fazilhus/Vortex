#include "vtpch.hpp"

#include "Platforms/OpenGL/OpenGLBuffer.hpp"

namespace Vortex {

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint size)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::NONE: {
				VT_CORE_ASSERT(false, "RendererAPI::API::NONE");
				return nullptr;
			}
			case RendererAPI::API::OPENGL: {
				return CreateRef<OpenGLVertexBuffer>(vertices, size);
			}
		}
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint* indices, uint count)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::NONE: {
				VT_CORE_ASSERT(false, "RendererAPI::API::NONE");
				return nullptr;
			}
			case RendererAPI::API::OPENGL: {
				return CreateRef<OpenGLIndexBuffer>(indices, count);
			}
		}
	}

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
		    case RendererAPI::API::NONE: {
				VT_CORE_ASSERT(false, "RendererAPI::API::NONE");
				return nullptr;
			}
		    case RendererAPI::API::OPENGL: {
				return CreateRef<OpenGLVertexArray>();
			}
		}
	}

}
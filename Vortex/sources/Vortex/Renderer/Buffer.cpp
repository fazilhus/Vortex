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
		VT_CORE_ERROR("Unsupported api");
		VT_CORE_ASSERT(false, "Unsupported api");
		return nullptr;
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
		VT_CORE_ERROR("Unsupported api");
		VT_CORE_ASSERT(false, "Unsupported api");
		return nullptr;
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
		VT_CORE_ERROR("Unsupported api");
		VT_CORE_ASSERT(false, "Unsupported api");
		return nullptr;
	}

}
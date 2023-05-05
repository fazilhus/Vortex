#include "vtpch.hpp"

#include "Platforms/OpenGL/OpenGLBuffer.hpp"

namespace Vortex {

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint4 size)
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

	Ref<VertexBuffer> VertexBuffer::Create(uint32 size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::NONE: {
			VT_CORE_ASSERT(false, "RendererAPI::API::NONE");
			return nullptr;
		}
		case RendererAPI::API::OPENGL: {
			return CreateRef<OpenGLVertexBuffer>(size);
		}
		}
		VT_CORE_ERROR("Unsupported api");
		VT_CORE_ASSERT(false, "Unsupported api");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32* indices, uint32 count)
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

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpec& spec) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::NONE: {
			VT_CORE_ASSERT(false, "RendererAPI::API::NONE");
			return nullptr;
		}
		case RendererAPI::API::OPENGL: {
			return CreateRef<OpenGLFrameBuffer>(spec);
		}
		}
		VT_CORE_ERROR("Unsupported api");
		VT_CORE_ASSERT(false, "Unsupported api");
		return nullptr;
	}
}

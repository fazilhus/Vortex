#include "vtpch.hpp"

#include "Platforms/OpenGL/OpenGLTexture.hpp"

namespace Vortex {

	Ref<Texture2D> Texture2D::Create(const std::string& path) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::NONE: {
				VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr; 
			}
			case RendererAPI::API::OPENGL:  
				return CreateRef<OpenGLTexture2D>(path);
		}
		VT_CORE_ERROR("Unknown renderer api");
		VT_CORE_ASSERT(false, "Unknown renderer api");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(uint width, uint height) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::NONE: {
			VT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		case RendererAPI::API::OPENGL:
			return CreateRef<OpenGLTexture2D>(width, height);
		}
		VT_CORE_ERROR("Unknown renderer api");
		VT_CORE_ASSERT(false, "Unknown renderer api");
		return nullptr;
	}
}

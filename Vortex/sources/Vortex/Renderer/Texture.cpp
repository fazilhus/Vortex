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
				return std::make_shared<OpenGLTexture2D>(path);
			default: {
				VT_CORE_ASSERT(false, "Unknown renderer type");
				return nullptr;
			}
		}
	}

}
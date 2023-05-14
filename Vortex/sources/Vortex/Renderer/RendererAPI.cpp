#include <vtpch.hpp>

#include "Platforms/OpenGL/OpenGLRendererAPI.hpp"

namespace Vortex {

	RendererAPI::API RendererAPI::s_api = RendererAPI::API::OPENGL;

	Scope<RendererAPI> RendererAPI::Create() {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::NONE: {
				VT_CORE_ERROR("RendererAPI::API::NONE");
				VT_CORE_ASSERT(false, "RendererAPI::API::NONE");
				return nullptr;
			}
			case RendererAPI::API::OPENGL: {
				return CreateScope<OpenGLRendererAPI>();
			}
		}
		VT_CORE_ERROR("Unknown renderer api");
		VT_CORE_ASSERT(false, "Unknown renderer api");
		return nullptr;
	}

}

#include <vtpch.hpp>
#include "Vortex/Renderer/GraphicsContext.hpp"

#include "Platforms/OpenGL/OpenGLContext.hpp"

namespace Vortex {

	Scope<GraphicsContext> GraphicsContext::Create(void* window) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::NONE: {
				VT_CORE_ASSERT(false, "RendererAPI::API::NONE");
				return nullptr;
			}
			case RendererAPI::API::OPENGL: {
				return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
			}
		}
		VT_CORE_ERROR("Unknown renderer api");
		VT_CORE_ASSERT(false, "Unknown renderer api");
		return nullptr;
	}
}

#include <vtpch.hpp>

#include "Platforms/OpenGL/OpenGLRendererAPI.hpp"

namespace Vortex {

	Scope<RendererAPI> Render::s_rendererAPI = RendererAPI::Create();

}
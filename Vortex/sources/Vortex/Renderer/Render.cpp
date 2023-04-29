#include "vtpch.hpp"

#include "Platforms/OpenGL/OpenGLRendererAPI.hpp"

namespace Vortex {

	RendererAPI* Render::s_rendererAPI = new OpenGLRendererAPI();

}
#include "vtpch.hpp"
#include "Vortex/Renderer/Render.hpp"
#include "Platforms/OpenGL/OpenGLRendererAPI.hpp"

namespace Vortex {

	RendererAPI* Render::s_rendererAPI = new OpenGLRendererAPI();

}
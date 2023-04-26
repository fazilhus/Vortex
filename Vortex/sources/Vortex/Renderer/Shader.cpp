#include "vtpch.hpp"
#include "Vortex/Renderer/Shader.hpp"
#include "Vortex/Renderer/Renderer.hpp"

#include "Platforms/OpenGL/OpenGLShader.hpp"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Vortex {

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::NONE:
				VT_CORE_ASSERT(false, "RendererAPI::API::NONE ");
				return nullptr;
			case RendererAPI::API::OPENGL:
				return new OpenGLShader(vertexSrc, fragmentSrc);
		}
	}

}
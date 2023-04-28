#include "vtpch.hpp"
#include "Vortex/Renderer/Shader.hpp"
#include "Vortex/Renderer/Renderer.hpp"

#include "Platforms/OpenGL/OpenGLShader.hpp"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Vortex {

	Ref<Shader> Shader::Create(const std::string& filepath) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::NONE:
				VT_CORE_ASSERT(false, "RendererAPI::API::NONE ");
				return nullptr;
			case RendererAPI::API::OPENGL:
				return std::make_shared<OpenGLShader>(filepath);
			default:
				VT_CORE_ASSERT(false, "unknown renderer api");
				return nullptr;
		}
	}

	Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::NONE:
				VT_CORE_ASSERT(false, "RendererAPI::API::NONE ");
				return nullptr;
			case RendererAPI::API::OPENGL:
				return std::make_shared<OpenGLShader>(vertexSrc, fragmentSrc);
			default:
				VT_CORE_ASSERT(false, "unknown renderer api");
				return nullptr;
		}
	}

}
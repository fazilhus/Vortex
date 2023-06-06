#include <vtpch.hpp>

#include "Platforms/OpenGL/OpenGLShader.hpp"

namespace Vortex {

	Ref<Shader> Shader::Create(const std::string& filepath) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::NONE:
				VT_CORE_ERROR("RendererAPI::API::NONE");
				VT_CORE_ASSERT(false, "[Shader] RendererAPI::API::NONE ");
				return nullptr;
			case RendererAPI::API::OPENGL:
				return CreateRef<OpenGLShader>(filepath);
			default: {
				VT_CORE_ERROR("Unknown renderer api");
				VT_CORE_ASSERT(false, "[Shader] Unknown renderer api");
				return nullptr;
			}
		}
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::NONE:
				VT_CORE_ERROR("RendererAPI::API::NONE");
				VT_CORE_ASSERT(false, "[Shader] RendererAPI::API::NONE ");
				return nullptr;
			case RendererAPI::API::OPENGL:
				return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
			default: {
				VT_CORE_ERROR("Unknown renderer api");
				VT_CORE_ASSERT(false, "[Shader] Unknown renderer api");
				return nullptr;
			}
		}
	}

	void ShaderLib::Add(const std::string& name, const Ref<Shader>& shader) {
		VT_CORE_ASSERT(!Exists(name), "[Shader] Shader with name {0} already exists", name);
		m_shaders[name] = shader;
	}

	void ShaderLib::Add(const Ref<Shader>& shader) {
		Add(shader->GetName(), shader);
	}

	Ref<Shader> ShaderLib::Load(const std::string& name, const std::string& filepath) {
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLib::Load(const std::string& filepath) {
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLib::Get(const std::string& name) {
		VT_CORE_ASSERT(Exists(name), "[Shader] Shader {0} does not exists", name);
		return m_shaders[name];
	}

	bool ShaderLib::Exists(const std::string& name) const {
		return m_shaders.find(name) != m_shaders.end();
	}

}
#pragma once
#include <vtpch.hpp>

#include "glm/glm.hpp"

namespace Vortex {

	class Shader {
	public:
		virtual ~Shader() = default;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		virtual void SetInt(const std::string& name, int v) = 0;
		virtual void SetIntArray(const std::string& name, int* v, uint32 count) = 0;
		virtual void SetFloat(const std::string& name, float v) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& v) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& v) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& v) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& v) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& v) = 0;
	};

	class ShaderLib {
	private:
		HashMap<std::string, Ref<Shader>> m_shaders;

	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);
		Ref<Shader> Load(const std::string& filepath);

		Ref<Shader> Get(const std::string& name);
		bool Exists(const std::string& name) const;
	};

}

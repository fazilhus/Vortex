#pragma once
#include <vtpch.hpp>

typedef uint4 GLenum;

namespace Vortex {

	class OpenGLShader : public Shader {
	private:
		uint4 m_rendererID;
		std::string m_name;
		HashMap<std::string, uint4> m_uniformLoc;

	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override { return m_name; }

		virtual void SetInt(const std::string& name, int v) override;
		virtual void SetIntArray(const std::string& name, int* v, uint32 count) override;
		virtual void SetFloat(const std::string& name, float v) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& v) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& v) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& v) override;
		virtual void SetMat3(const std::string& name, const glm::mat3& v) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& v) override;

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadShaderFile(const std::string& filepath);
		void PreProcess(const std::string& src, HashMap<GLenum, std::string>&);
		void CompileShader(const HashMap<GLenum, std::string>& shaderSrcs);
	};

};

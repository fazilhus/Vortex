#include "vtpch.hpp"
#include "Platforms/OpenGL/OpenGLShader.hpp"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Vortex {

	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex") {
			return GL_VERTEX_SHADER;
		}
		else if (type == "fragment" || type == "pixel") {
			return GL_FRAGMENT_SHADER;
		}
		else {
			VT_CORE_ASSERT(false, "Unknown shader type");
			return 0;
		}
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
		: m_rendererID(0) {
		std::string src = ReadShaderFile(filepath);
		std::unordered_map<GLenum, std::string> shaderSrcs;
		PreProcess(src, shaderSrcs);
		CompileShader(shaderSrcs);

		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_rendererID(0), m_name(name) {
		std::unordered_map<GLenum, std::string> shaderSrcs;
		shaderSrcs[GL_VERTEX_SHADER] = vertexSrc;
		shaderSrcs[GL_FRAGMENT_SHADER] = fragmentSrc;
		CompileShader(shaderSrcs);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_rendererID);
	}

	void OpenGLShader::Bind() const {
		glUseProgram(m_rendererID);
	}

	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int v) {
		VT_CORE_TRACE("Shader SetInt: name = {0}, v = {1}", name, v);
		UploadUniformInt(name, v);
	}

	void OpenGLShader::SetFloat(const std::string& name, float v) {
		VT_CORE_TRACE("Shader SetFloat: name = {0}, v = {1}", name, v);
		UploadUniformFloat(name, v);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& v) {
		VT_CORE_TRACE("Shader SetFloat2: name = {0}, v = ({1}, {2})", name, v.x, v.y);
		UploadUniformFloat2(name, v);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& v) {
		VT_CORE_TRACE("Shader SetFloat3: name = {0}, v = ({1}, {2}, {3})", name, v.x, v.y, v.z);
		UploadUniformFloat3(name, v);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& v) {
		VT_CORE_TRACE("Shader SetFloat4: name = {0}, v = ({1}, {2}, {3}, {4})", name, v.x, v.y, v.z, v.w);
		UploadUniformFloat4(name, v);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& v) {
		VT_CORE_TRACE("Shader SetMat3: name = {0}", name);
		UploadUniformMat3(name, v);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& v) {
		VT_CORE_TRACE("Shader SetMat4: name = {0}", name);
		UploadUniformMat4(name, v);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value) {
		if (m_uniformLoc.find(name) == m_uniformLoc.end()) {
			m_uniformLoc[name] = glGetUniformLocation(m_rendererID, name.c_str());
		}
		glUniform1i(m_uniformLoc[name], value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value) {
		if (m_uniformLoc.find(name) == m_uniformLoc.end()) {
			m_uniformLoc[name] = glGetUniformLocation(m_rendererID, name.c_str());
		}
		glUniform1f(m_uniformLoc[name], value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) {
		if (m_uniformLoc.find(name) == m_uniformLoc.end()) {
			m_uniformLoc[name] = glGetUniformLocation(m_rendererID, name.c_str());
		}
		glUniform2f(m_uniformLoc[name], value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) {
		if (m_uniformLoc.find(name) == m_uniformLoc.end()) {
			m_uniformLoc[name] = glGetUniformLocation(m_rendererID, name.c_str());
		}
		glUniform3f(m_uniformLoc[name], value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) {
		if (m_uniformLoc.find(name) == m_uniformLoc.end()) {
			m_uniformLoc[name] = glGetUniformLocation(m_rendererID, name.c_str());
		}
		glUniform4f(m_uniformLoc[name], value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
		if (m_uniformLoc.find(name) == m_uniformLoc.end()) {
			m_uniformLoc[name] = glGetUniformLocation(m_rendererID, name.c_str());
		}
		glUniformMatrix3fv(m_uniformLoc[name], 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
		if (m_uniformLoc.find(name) == m_uniformLoc.end()) {
			m_uniformLoc[name] = glGetUniformLocation(m_rendererID, name.c_str());
		}
		glUniformMatrix4fv(m_uniformLoc[name], 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadShaderFile(const std::string& filepath) {
		std::string res{};
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (!in) {
			VT_CORE_ERROR("Could not open file: '{0}'", filepath);
			return res;
		}

		size_t size = in.tellg();
		if (size != -1) {
			res.resize(size);
			in.seekg(0, std::ios::beg);
			in.read(&res[0], size);
			in.close();
		}
		else {
			VT_CORE_ERROR("Could not read from file '{0}'", filepath);
		}
		VT_CORE_INFO("shader was read from file");
		return res;
	}

	void OpenGLShader::PreProcess(const std::string& src, std::unordered_map<GLenum, std::string>& shaderSrcs) {
		const char* token = "#type";
		std::size_t tokenLen = strlen(token);
		std::size_t pos = src.find(token, 0);

		while (pos != std::string::npos) {
			std::size_t eol = src.find_first_of("\r\n", pos);
			VT_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			std::size_t begin = pos + tokenLen + 1;
			std::string type = src.substr(begin, eol - begin);
			VT_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type");

			std::size_t nextLinePos = src.find_first_not_of("\r\n", eol);
			VT_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = src.find(token, nextLinePos);
			shaderSrcs[ShaderTypeFromString(type)] = src.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? src.size() - 1 : nextLinePos));
		}

		VT_CORE_INFO("{0} shaders was preprocessed", shaderSrcs.size());
	}

	void OpenGLShader::CompileShader(const std::unordered_map<GLenum, std::string>& shaderSrcs) {
		GLuint program = glCreateProgram();

		VT_CORE_ASSERT(shaderSrcs.size() <= 2, "Only two types of shaders are supported for now");

		std::array<GLenum, 2> shaderIDs;
		int shaderIdIndex = 0;

		for (const auto& item : shaderSrcs) {
			GLenum type = item.first;
			const std::string src = item.second;

			GLuint shader = glCreateShader(type);
			const GLchar* srcCstr = src.c_str();
			glShaderSource(shader, 1, &srcCstr, 0);

			glCompileShader(shader);

			GLint isCompiled;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			
			if (isCompiled == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				VT_CORE_ERROR("{0}", infoLog.data());
				VT_CORE_ASSERT(false, "shader compilation failure");
				return;
			}

			glAttachShader(program, shader);
			shaderIDs[shaderIdIndex++] = shader;
		}

		m_rendererID = program;

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			VT_CORE_ERROR("{0}", infoLog.data());
			VT_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto& id : shaderIDs) {
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		VT_CORE_INFO("Shader compiling successful");
	}

}

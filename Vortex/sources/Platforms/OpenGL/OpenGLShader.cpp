#include <vtpch.hpp>
#include "Platforms/OpenGL/OpenGLShader.hpp"

#include <glad/glad.h>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace Vortex {

	static std::string ShaderCacheDir { "assets/cache/shader/opengl" };

	namespace Utils {

		static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage) {
			switch (stage) {
			case GL_VERTEX_SHADER: return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
			default:
				VT_CORE_ASSERT(false, "Utils::GLShaderStageToShader does not support this {0} shader stage", stage);
				return (shaderc_shader_kind)0;
			}
		}

		static const char* GLShaderStageToString(GLenum stage) {
			switch (stage) {
			case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
			default:
				VT_CORE_ASSERT(false, "Utils::GLShaderStageToShader does not support this {0} shader stage", stage);
				return nullptr;
			}
		}

		static void CreateCacheDirIfNeeded(const std::string& dir) {
			if (!std::filesystem::exists(dir)) {
				std::filesystem::create_directories(dir);
			}
		}

		static const char* GetCacheDir() {
			CreateCacheDirIfNeeded(ShaderCacheDir);
			return ShaderCacheDir.c_str();
		}

		static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER: return ".cached_opengl.vert";
			case GL_FRAGMENT_SHADER: return ".cached_opengl.frag";
			default:
				VT_CORE_ASSERT(false, "Utils::GLShaderStageCachedOpenGLFileExtension does not support this {0} shader stage", stage);
				return "";
			}
		}

		static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER: return ".cached_vulkan.vert";
			case GL_FRAGMENT_SHADER: return ".cached_vulkan.frag";
			default:
				VT_CORE_ASSERT(false, "Utils::GLShaderStageCachedVulkanFileExtension does not support this {0} shader stage", stage);
				return "";
			}
		}

		static GLenum ShaderTypeFromString(const std::string& type) {
			if (type == "vertex") {
				return GL_VERTEX_SHADER;
			}

			if (type == "fragment" || type == "pixel") {
				return GL_FRAGMENT_SHADER;
			}

			VT_CORE_ASSERT(false, "Unknown shader type");
			return 0;
		}

	}

	

	OpenGLShader::OpenGLShader(const std::string& filepath)
		: m_rendererID(0), m_filepath(filepath) {
		Utils::CreateCacheDirIfNeeded(ShaderCacheDir);

		std::string src = ReadShaderFile(filepath);
		HashMap<GLenum, std::string> shaderSrcs;
		PreProcess(src, shaderSrcs);

		{
			Timer timer;
			CompileOrGetVulkanBinaries(shaderSrcs);
			CompileOrGetOpenGLBinaries();
			CreateProgram();
			VT_CORE_INFO("Shader compilation took {0} ms", timer.ElapsedMillis());
		}

		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_rendererID(0), m_name(name) {
		HashMap<GLenum, std::string> shaderSrcs;
		shaderSrcs[GL_VERTEX_SHADER] = vertexSrc;
		shaderSrcs[GL_FRAGMENT_SHADER] = fragmentSrc;

		{
			Timer timer;
			CompileOrGetVulkanBinaries(shaderSrcs);
			CompileOrGetOpenGLBinaries();
			CreateProgram();
			VT_CORE_INFO("Shader compilation took {0} ms", timer.ElapsedMillis());
		}
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

	void OpenGLShader::SetIntArray(const std::string& name, int* v, uint32 count) {
		VT_CORE_TRACE("Shader SetIntArray: name = {0}, count = {1}", name, count);
		UploadUniformIntArray(name, v, count);
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

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count) {
		if (m_uniformLoc.find(name) == m_uniformLoc.end()) {
			m_uniformLoc[name] = glGetUniformLocation(m_rendererID, name.c_str());
		}
		glUniform1iv(m_uniformLoc[name], count, values);
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

		in.seekg(0, std::ios::end);
		res.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&res[0], res.size());
		in.close();
		VT_CORE_INFO("shader was read from file");
		return res;
	}

	void OpenGLShader::PreProcess(const std::string& src, HashMap<GLenum, std::string>& shaderSrcs) {
		const char* token = "#type";
		std::size_t tokenLen = strlen(token);
		std::size_t pos = src.find(token, 0);

		while (pos != std::string::npos) {
			std::size_t eol = src.find_first_of("\r\n", pos);
			VT_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			std::size_t begin = pos + tokenLen + 1;
			std::string type = src.substr(begin, eol - begin);
			VT_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type");

			std::size_t nextLinePos = src.find_first_not_of("\r\n", eol);
			VT_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = src.find(token, nextLinePos);
			shaderSrcs[Utils::ShaderTypeFromString(type)] = src.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? src.size() - 1 : nextLinePos));
		}

		VT_CORE_INFO("{0} shaders was preprocessed", shaderSrcs.size());
	}

	void OpenGLShader::CompileOrGetVulkanBinaries(const HashMap<GLenum, std::string>& shaderSources) {
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		bool optimize = true;
		if (optimize) {
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		}

		std::filesystem::path cacheDir = Utils::GetCacheDir();

		auto& shaderData = m_vulkanSPIRV;
		shaderData.clear();

		for (auto&& [stage, src] : shaderSources) {
			std::filesystem::path shaderFilepath = m_filepath;
			std::filesystem::path cachedPath = cacheDir / (shaderFilepath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in{cachedPath, std::ios::in | std::ios::binary};
			if (in.is_open()) {
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else {
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(src, Utils::GLShaderStageToShaderC(stage), m_filepath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
					VT_CORE_ERROR(module.GetErrorMessage());
					VT_CORE_ASSERT(false, "OpenGLShader::CompileOrGetVulkanBinaries Shader compilation was not successful");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open()) {
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data] : shaderData) {
			Reflect(stage, data);
		}
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries() {
		auto& shaderData = m_openglSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDir();

		shaderData.clear();
		m_openglSourceCode.clear();
		for (auto&& [stage, spirv] : m_vulkanSPIRV)
		{
			std::filesystem::path shaderFilePath = m_filepath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_openglSourceCode[stage] = glslCompiler.compile();
				auto& source = m_openglSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_filepath.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					VT_CORE_ERROR(module.GetErrorMessage());
					VT_CORE_ASSERT(false, "OpenGLShader::CompileOrGetOpenGLBinaries Shader compilation was not successful");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGLShader::CreateProgram() {
		GLuint program = glCreateProgram();

		Vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_openglSPIRV) {
			GLuint shader = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32));
			glSpecializeShader(shader, "main", 0, nullptr, nullptr);
			glAttachShader(program, shader);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			VT_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_filepath, infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_rendererID = program;
	}

	void OpenGLShader::Reflect(GLenum stage, const Vector<uint32>& shaderData) {
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		VT_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_filepath);
		VT_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
		VT_CORE_TRACE("    {0} resources", resources.sampled_images.size());

		VT_CORE_TRACE("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			VT_CORE_TRACE("  {0}", resource.name);
			VT_CORE_TRACE("    Size = {0}", bufferSize);
			VT_CORE_TRACE("    Binding = {0}", binding);
			VT_CORE_TRACE("    Members = {0}", memberCount);
		}
	}

}

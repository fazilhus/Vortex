#pragma once
#include "vtpch.hpp"

namespace Vortex {

	class Shader {
	private:
		uint m_rendererID;
		std::unordered_map<std::string, uint> m_uniformLoc;

	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~Shader();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& mat);
	};

}
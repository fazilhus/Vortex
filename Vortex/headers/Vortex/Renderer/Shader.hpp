#pragma once
#include "vtpch.hpp"

namespace Vortex {

	class Shader {
	/*private:
		uint m_rendererID;
		std::unordered_map<std::string, uint> m_uniformLoc;*/

	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);

		//virtual void UploadUniformMat4(const std::string& name, const glm::mat4& mat);
	};

}
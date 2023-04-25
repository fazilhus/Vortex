#pragma once
#include "vtpch.hpp"

namespace Vortex {

	class Shader {
	private:
		uint m_rendererID;

	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~Shader();

		virtual void Bind() const;
		virtual void Unbind() const;
	};

}
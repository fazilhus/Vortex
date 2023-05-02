#pragma once
#include "vtpch.hpp"

namespace Vortex {
	
	class OpenGLTexture2D : public Texture2D {
	private:
		std::string m_path;
		uint4 m_width, m_height;
		uint4 m_rendererID;
		uint4 m_internalFormat, m_format;

	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(uint4 width, uint4 height);
		virtual ~OpenGLTexture2D();

		virtual uint4 GetWidth() const override { return m_width; }
		virtual uint4 GetHeight() const override { return m_height; }

		virtual void SetData(void* data, uint4 size) override;

		virtual void Bind(uint4 slot = 0) const override;
	};

}
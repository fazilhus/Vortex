#pragma once
#include "vtpch.hpp"

namespace Vortex {
	
	class OpenGLTexture2D : public Texture2D {
	private:
		std::string m_path;
		uint m_width, m_height;
		uint m_rendererID;
		uint m_internalFormat, m_format;

	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(uint width, uint height);
		virtual ~OpenGLTexture2D();

		virtual uint GetWidth() const override { return m_width; }
		virtual uint GetHeight() const override { return m_height; }

		virtual void SetData(void* data, uint size) override;

		virtual void Bind(uint slot = 0) const override;
	};

}
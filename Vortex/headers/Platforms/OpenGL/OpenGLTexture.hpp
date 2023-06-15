#pragma once
#include <vtpch.hpp>

namespace Vortex {
	
	class OpenGLTexture2D : public Texture2D {
	private:
		std::string m_path;
		uint32 m_width, m_height;
		uint32 m_rendererID;
		uint32 m_internalFormat, m_format;
		bool m_isLoaded;

	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(uint32 width, uint32 height);
		virtual ~OpenGLTexture2D() override;

		virtual uint32 GetWidth() const override { return m_width; }
		virtual uint32 GetHeight() const override { return m_height; }
		virtual const std::string& GetPath() const override { return m_path; }
		virtual uint32 GetID() const override { return m_rendererID; }

		virtual bool IsLoaded() const override { return m_isLoaded; }

		virtual void SetData(void* data, uint32 size) override;

		virtual void Bind(uint32 slot = 0) const override;

		virtual bool operator==(const Texture& other) const override;
	};

}

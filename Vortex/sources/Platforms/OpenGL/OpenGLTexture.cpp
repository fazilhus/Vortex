#include <vtpch.hpp>
#include "Platforms/OpenGL/OpenGLTexture.hpp"

#include <glad/glad.h>

namespace Vortex {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) 
		: m_path(path), m_isLoaded(false) {
		VT_PROFILE_FUNC();
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = nullptr;
		{
			VT_PROFILE_SCOPE("OpenGLTexture2D::OpenGLTexture2D - stbi_load");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		if (data) {
			m_isLoaded = true;
			m_width = width;
			m_height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			switch (channels) {
			case 3: {
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
				VT_CORE_INFO("Texture format is rgb");
				break;
			}
			case 4: {
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
				VT_CORE_INFO("Texture format is rgba");
				break;
			}
			default:
				VT_CORE_ASSERT(internalFormat & dataFormat, "Format is not supported");
			}

			m_internalFormat = internalFormat;
			m_format = dataFormat;

			glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
			glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

			glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_format, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32 width, uint32 height)
	: m_width(width), m_height(height), m_rendererID(0) {
		VT_PROFILE_FUNC();
		m_internalFormat = GL_RGBA8;
		m_format = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		VT_PROFILE_FUNC();
		glDeleteTextures(1, &m_rendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32 size) {
		uint32 bpp = m_format == GL_RGBA ? 4 : 3;
		VT_CORE_ASSERT(size == m_width * m_height * bpp, "Not enought data to cover entire texture");
		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_format, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32 slot) const {
		VT_CORE_TRACE("OpenGLTexture::Bind at slot {0}", slot);
		glBindTextureUnit(slot, m_rendererID);
	}

	bool OpenGLTexture2D::operator==(const Texture& other) const {
		return m_rendererID == other.GetID();
	}
}

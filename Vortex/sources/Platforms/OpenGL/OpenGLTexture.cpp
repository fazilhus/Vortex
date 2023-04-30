#include "vtpch.hpp"
#include "Platforms/OpenGL/OpenGLTexture.hpp"

#include <glad/glad.h>

namespace Vortex {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) 
		: m_path(path) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		VT_CORE_ASSERT(data, "Failed to load image");

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


		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &m_rendererID);
	}

	void OpenGLTexture2D::Bind(uint slot) const {
		glBindTextureUnit(slot, m_rendererID);
	}

}

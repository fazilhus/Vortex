#include <vtpch.hpp>
#include "Platforms/OpenGL/OpenGLBuffer.hpp"

#include <glad/glad.h>

namespace Vortex {

	// VERTEX BUFFER

	OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, uint4 size) : m_rendererID(0) {
		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32 size) : m_rendererID(0) {
		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1, &m_rendererID);
	}

	void OpenGLVertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	}

	void OpenGLVertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32 size) {
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	// INDEXBUFFER

	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32* indices, uint32 count) 
		: m_rendererID(0), m_count(count) {
		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint4), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		glDeleteBuffers(1, &m_rendererID);
	}

	void OpenGLIndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
	}

	void OpenGLIndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// VERTEX ARRAY

	static uint4 ShaderDataTypeToOpenGLType(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
			default: {
				VT_CORE_ASSERT(false, "Unknown data type");
				return 0;
			}
		}
	}

	OpenGLVertexArray::OpenGLVertexArray() 
		: m_rendererID(0), m_vertexBufferInd(0) {
		glCreateVertexArrays(1, &m_rendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_rendererID);
	}

	void OpenGLVertexArray::Bind() const {
		glBindVertexArray(m_rendererID);
	}

	void OpenGLVertexArray::Unbind() const {
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vb) {
		VT_CORE_ASSERT(vb->GetLayout().GetElements().size(), "VertexBuffer has no layout");

		glBindVertexArray(m_rendererID);
		vb->Bind();

		const auto& layout = vb->GetLayout();
		for (const auto& item : layout) {
			switch (item.type) {
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
					glEnableVertexAttribArray(m_vertexBufferInd);
					glVertexAttribPointer(m_vertexBufferInd,
						item.GetComponentCount(),
						ShaderDataTypeToOpenGLType(item.type),
						item.normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						reinterpret_cast<const void*>(item.offset));
					m_vertexBufferInd++;
					break;
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
					glEnableVertexAttribArray(m_vertexBufferInd);
					glVertexAttribIPointer(m_vertexBufferInd,
						item.GetComponentCount(),
						ShaderDataTypeToOpenGLType(item.type),
						layout.GetStride(),
						reinterpret_cast<const void*>(item.offset));
					m_vertexBufferInd++;
					break;
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4: {
					const auto count = item.GetComponentCount();
					for (std::size_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(m_vertexBufferInd);
						glVertexAttribPointer(m_vertexBufferInd,
							count,
							ShaderDataTypeToOpenGLType(item.type),
							item.normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							reinterpret_cast<const void*>(item.offset + sizeof(float) * count * i));
						glVertexAttribDivisor(m_vertexBufferInd, 1);
						m_vertexBufferInd++;
					}
					break;
				}
				default: {
					VT_CORE_ASSERT(false, "Unknown data type");
				}
			}
		}
		m_vbs.push_back(vb);
	}

	void OpenGLVertexArray::AddIndexBuffer(const Ref<IndexBuffer>& ib) {
		glBindVertexArray(m_rendererID);
		ib->Bind();

		m_ib = ib;
	}

	namespace Utils {

		static GLenum TextureTarget(bool multisampled) {
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32* id, uint32 count) {
			glCreateTextures(TextureTarget(multisampled), count, id);
		}

		static void BindTexture(bool multisampled, uint32 id) {
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(uint32 id, int samples, GLenum internalFormat, GLenum format, uint32 width, uint32 height, int index) {
			bool multisampled = samples > 1;
			if (multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		static bool IsDepthFormat(FramebufferTextureFormat format) {
			switch (format)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
			}

			return false;
		}

		static GLenum FramebufferTextureFormatToGL(FramebufferTextureFormat format) {
			switch (format) {
			case FramebufferTextureFormat::RGBA8:
				return GL_RGBA8;
			case FramebufferTextureFormat::RED_INTEGER:
				return GL_RED_INTEGER;
			}
		}

	}

	static const uint32 s_maxFrameBuffer = 8192;

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpec& spec)
	: m_rendererID(0), m_depthAttachment(0), m_spec(spec),
	m_depthAttachmentSpecification(FramebufferTextureFormat::None) {
		for (auto& spec : m_spec.attachments.Attachments) {
			if (!Utils::IsDepthFormat(spec.TextureFormat)) {
				m_colorAttachmentSpecifications.emplace_back(spec);
			}
			else {
				m_depthAttachmentSpecification = spec;
			}
		}

		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer() {
		glDeleteFramebuffers(1, &m_rendererID);
		glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
		glDeleteTextures(1, &m_depthAttachment);
	}

	void OpenGLFrameBuffer::Invalidate() {
		if (m_rendererID) {
			glDeleteFramebuffers(1, &m_rendererID);
			glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
			glDeleteTextures(1, &m_depthAttachment);
		}

		glCreateFramebuffers(1, &m_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

		bool multisample = m_spec.samples > 1;

		if (m_colorAttachmentSpecifications.size()) {
			m_colorAttachments.resize(m_colorAttachmentSpecifications.size());
			Utils::CreateTextures(multisample, m_colorAttachments.data(), m_colorAttachments.size());

			for (std::size_t i = 0; i < m_colorAttachments.size(); ++i) {
				Utils::BindTexture(multisample, m_colorAttachments[i]);

				switch (m_colorAttachmentSpecifications[i].TextureFormat) {
				case FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_colorAttachments[i], m_spec.samples, GL_RGBA8, GL_RGBA, m_spec.width, m_spec.height, i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(m_colorAttachments[i], m_spec.samples, GL_R32I, GL_RED_INTEGER, m_spec.width, m_spec.height, i);
					break;
				}
			}
		}

		if (m_depthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None) {
			Utils::CreateTextures(multisample, &m_depthAttachment, 1);
			Utils::BindTexture(multisample, m_depthAttachment);

			switch (m_depthAttachmentSpecification.TextureFormat) {
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_depthAttachment, m_spec.samples, GL_DEPTH24_STENCIL8, 
					GL_DEPTH_STENCIL_ATTACHMENT, m_spec.width, m_spec.height);
				break;
			}
		}

		if (m_colorAttachments.size() > 1) {
			VT_CORE_ASSERT(m_colorAttachments.size() <= 4, "Not enough color attachments");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_colorAttachments.size(), buffers);
		}
		else if (m_colorAttachments.empty()) {
			glDrawBuffer(GL_NONE);
		}

		VT_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		glViewport(0, 0, m_spec.width, m_spec.height);
	}

	void OpenGLFrameBuffer::Unbind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32 width, uint32 height) {
		if (width == 0 || height == 0 || width > s_maxFrameBuffer || height > s_maxFrameBuffer) {
			VT_CORE_WARN("Attempted to resize window to {0}, {1}", width, height);
			return;
		}
		m_spec.width = width;
		m_spec.height = height;

		Invalidate();
	}
	int OpenGLFrameBuffer::ReadPixel(uint32 attachmentIndex, int x, int y) const {
		VT_CORE_ASSERT(attachmentIndex < m_colorAttachments.size(), "OpenGLFrameBuffer::ReadPixel attachmentIndex out of bounds");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}
	void OpenGLFrameBuffer::ClearAttachment(uint32 attachmentIndex, int v) {
		VT_CORE_ASSERT(attachmentIndex < m_colorAttachments.size(), "OpenGLFrameBuffer::ClearAttachment attachmentIndex out of bounds");

		auto& spec = m_colorAttachmentSpecifications[attachmentIndex];
		glClearTexImage(m_colorAttachments[attachmentIndex], 0, Utils::FramebufferTextureFormatToGL(spec.TextureFormat), GL_INT, &v);
	}

	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32 size, uint32 binding) : m_rendererID(0) {
		glCreateBuffers(1, &m_rendererID);
		glNamedBufferData(m_rendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_rendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer() {
		glDeleteBuffers(1, &m_rendererID);
	}

	void OpenGLUniformBuffer::SetData(const void* data, uint32 size, uint32 offset) {
		glNamedBufferSubData(m_rendererID, offset, size, data);
	}
}

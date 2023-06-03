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
		for (const auto item : layout) {
			switch (item.type) {
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool: {
					glEnableVertexAttribArray(m_vertexBufferInd);
					glVertexAttribPointer(m_vertexBufferInd,
						item.GetComponentCount(),
						ShaderDataTypeToOpenGLType(item.type),
						item.normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						reinterpret_cast<const void*>(item.offset));
					m_vertexBufferInd++;
					break;
				}
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
							reinterpret_cast<const void*>(sizeof(float) * count * i));
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

	static const uint32 s_maxFrameBuffer = 8192;

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpec& spec)
	: m_rendererID(0), m_colorAttachment(0), m_depthAttachment(0), m_spec(spec) {
		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer() {
		glDeleteFramebuffers(1, &m_rendererID);
		glDeleteTextures(1, &m_colorAttachment);
		glDeleteTextures(1, &m_depthAttachment);
	}

	void OpenGLFrameBuffer::Invalidate() {
		if (m_rendererID) {
			glDeleteFramebuffers(1, &m_rendererID);
			glDeleteTextures(1, &m_colorAttachment);
			glDeleteTextures(1, &m_depthAttachment);
		}

		glCreateFramebuffers(1, &m_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_spec.width, m_spec.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_spec.width, m_spec.height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

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
}

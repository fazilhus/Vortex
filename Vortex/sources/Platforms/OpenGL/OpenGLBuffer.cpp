#include "vtpch.hpp"
#include "Platforms/OpenGL/OpenGLBuffer.hpp"

#include <glad/glad.h>

namespace Vortex {

	// VERTEX BUFFER

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint size) {
		VT_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		VT_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_rendererID);
	}

	void OpenGLVertexBuffer::Bind() const {
		VT_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	}

	void OpenGLVertexBuffer::Unbind() const {
		VT_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// INDEXBUFFER

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint* indices, uint count) 
		: m_count(count) {
		VT_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		VT_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_rendererID);
	}

	void OpenGLIndexBuffer::Bind() const {
		VT_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
	}

	void OpenGLIndexBuffer::Unbind() const {
		VT_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// VERTEX ARRAY

	static uint ShaderDataTypeToOpenGLType(ShaderDataType type) {
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
		: m_vertexBufferInd(0) {
		VT_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &m_rendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		VT_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &m_rendererID);
	}

	void OpenGLVertexArray::Bind() const {
		VT_PROFILE_FUNCTION();
		glBindVertexArray(m_rendererID);
	}

	void OpenGLVertexArray::Unbind() const {
		VT_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vb) {
		VT_PROFILE_FUNCTION();
		VT_CORE_ASSERT(vb->GetLayout().GetElements().size(), "VertexBuffer has no layout");

		glBindVertexArray(m_rendererID);
		vb->Bind();

		const auto& layout = vb->GetLayout();
		for (const auto item : layout) {
			glEnableVertexAttribArray(m_vertexBufferInd);
			glVertexAttribPointer(m_vertexBufferInd, 
				item.GetComponentCount(),
				ShaderDataTypeToOpenGLType(item.type),
				item.normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(intptr_t)item.offset);
			m_vertexBufferInd++;
		}
		m_vbs.push_back(vb);
	}

	void OpenGLVertexArray::AddIndexBuffer(const Ref<IndexBuffer>& ib) {
		VT_PROFILE_FUNCTION();
		glBindVertexArray(m_rendererID);
		ib->Bind();

		m_ib = ib;
	}

}
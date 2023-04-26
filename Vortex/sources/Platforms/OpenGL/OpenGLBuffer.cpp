#include "vtpch.hpp"
#include "Platforms/OpenGL/OpenGLBuffer.hpp"

#include <glad/glad.h>

namespace Vortex {

	// VERTEX BUFFER

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint size) {
		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
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

	// INDEXBUFFER

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint* indices, uint count) 
		: m_count(count) {
		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), indices, GL_STATIC_DRAW);
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

	OpenGLVertexArray::OpenGLVertexArray() {
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

		uint ind = 0;
		const auto& layout = vb->GetLayout();
		for (const auto item : layout) {
			glEnableVertexAttribArray(ind);
			glVertexAttribPointer(ind, 
				item.GetComponentCount(),
				ShaderDataTypeToOpenGLType(item.type),
				item.normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)item.offset);
			ind++;
		}
		m_vbs.push_back(vb);
	}

	void OpenGLVertexArray::AddIndexBuffer(const Ref<IndexBuffer>& ib) {
		glBindVertexArray(m_rendererID);
		ib->Bind();

		m_ib = ib;
	}

}
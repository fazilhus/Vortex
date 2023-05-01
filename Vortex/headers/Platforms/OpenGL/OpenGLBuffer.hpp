#pragma once
#include "vtpch.hpp"

namespace Vortex {

	class OpenGLVertexBuffer : public VertexBuffer {
	private:
		uint m_rendererID;
		BufferLayout m_layout;

	public:
		OpenGLVertexBuffer(float* vertices, uint size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_layout = layout; }
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	private:
		uint m_rendererID;
		uint m_count;

	public:
		OpenGLIndexBuffer(uint* indices, uint count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		inline virtual uint GetCount() const override { return m_count; }
	};

	class OpenGLVertexArray : public VertexArray {
	private:
		uint m_rendererID;
		uint m_vertexBufferInd;
		std::vector<Ref<VertexBuffer>> m_vbs;
		Ref<IndexBuffer> m_ib;

	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vb) override;
		virtual void AddIndexBuffer(const Ref<IndexBuffer>& ib) override;

		inline virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_vbs; }
		inline virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_ib; }
	};

}
#pragma once
#include <vtpch.hpp>

namespace Vortex {

	class OpenGLVertexBuffer : public VertexBuffer {
	private:
		uint4 m_rendererID;
		BufferLayout m_layout;

	public:
		OpenGLVertexBuffer(const float* vertices, uint4 size);
		OpenGLVertexBuffer(uint32 size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_layout = layout; }

		virtual void SetData(const void* data, uint32 size) override;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	private:
		uint4 m_rendererID;
		uint4 m_count;

	public:
		OpenGLIndexBuffer(const uint32* indices, uint32 count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		inline virtual uint4 GetCount() const override { return m_count; }
	};

	class OpenGLVertexArray : public VertexArray {
	private:
		uint4 m_rendererID;
		uint4 m_vertexBufferInd;
		Vector<Ref<VertexBuffer>> m_vbs;
		Ref<IndexBuffer> m_ib;

	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vb) override;
		virtual void AddIndexBuffer(const Ref<IndexBuffer>& ib) override;

		inline virtual const Vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_vbs; }
		inline virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_ib; }
	};

	class OpenGLFrameBuffer : public FrameBuffer {
	private:
		uint32 m_rendererID;
		uint32 m_colorAttachment, m_depthAttachment;
		FrameBufferSpec m_spec;

	public:
		OpenGLFrameBuffer(const FrameBufferSpec& spec);
		virtual ~OpenGLFrameBuffer() override;

		void Invalidate();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Resize(uint32 x, uint32 y) override;

		virtual uint32 GetColorAttachmentID() const override { return m_colorAttachment; }
		virtual const FrameBufferSpec& GetSpec() const override { return m_spec; }
	};

}
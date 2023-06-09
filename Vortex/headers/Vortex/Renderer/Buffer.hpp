#pragma once

namespace Vortex {

	enum class ShaderDataType {
		None = 0,
		Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint4 ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Float2:   return 4 * 2;
			case ShaderDataType::Float3:   return 4 * 3;
			case ShaderDataType::Float4:   return 4 * 4;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:     return 4 * 2;
			case ShaderDataType::Int3:     return 4 * 3;
			case ShaderDataType::Int4:     return 4 * 4;
			case ShaderDataType::Bool:     return 1;
			default: {
				VT_CORE_ASSERT(false, "Unknown data type");
				return 0;
			}
		}
	}

	struct BufferElement {
		std::string name;
		ShaderDataType type;
		uint4 size;
		uint4 offset;
		bool normalized;

		BufferElement() 
			: name(""), type(), size(0), offset(0), normalized(false) {}
		BufferElement(const std::string& name, ShaderDataType type, bool normalized = false) 
			: name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized) {}

		uint4 GetComponentCount() const {
			switch (type) {
				case ShaderDataType::Float:    return 1;
				case ShaderDataType::Float2:   return 2;
				case ShaderDataType::Float3:   return 3;
				case ShaderDataType::Float4:   return 4;
				case ShaderDataType::Mat3:     return 3;
				case ShaderDataType::Mat4:     return 4;
				case ShaderDataType::Int:      return 1;
				case ShaderDataType::Int2:     return 2;
				case ShaderDataType::Int3:     return 3;
				case ShaderDataType::Int4:     return 4;
				case ShaderDataType::Bool:     return 1;
				default: {
					VT_CORE_ASSERT(false, "Unknown data type");
					return 0;
				}
			}
		}
	};

	class BufferLayout {
	private:
		Vector<BufferElement> m_elements;
		uint4 m_stride;

	public:
		BufferLayout()
			: m_elements(Vector<BufferElement>(0)), m_stride(0) {}
		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_elements(elements) {
			CalcOffsetAndStride();
		}
		BufferLayout(const BufferLayout& other) 
			: m_elements(other.GetElements()), m_stride(other.GetStride()) {
			VT_CORE_INFO("BufferLayout copy constructor {0} {1}", m_elements.size(), m_stride);
		}

		inline uint4 GetStride() const { return m_stride; }
		inline const Vector<BufferElement>& GetElements() const { return m_elements; }

		Vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		Vector<BufferElement>::iterator end() { return m_elements.end(); }
		Vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		Vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

	private:
		void CalcOffsetAndStride() {
			uint4 offset = 0;
			m_stride = 0;
			for (auto& element : m_elements) {
				element.offset = offset;
				offset += element.size;
				m_stride += element.size;
			}
		}
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		virtual void SetData(const void* data, uint32 size) = 0;

		static Ref<VertexBuffer> Create(float* vertices, uint4 size);
		static Ref<VertexBuffer> Create(uint32 size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint4 GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32* indices, uint32 count);
	};

	class VertexArray {
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vb) = 0;
		virtual void AddIndexBuffer(const Ref<IndexBuffer>& ib) = 0;

		virtual const Vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};

	enum class FramebufferTextureFormat {
		None = 0,
		RGBA8,
		RED_INTEGER,
		DEPTH24STENCIL8,
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpec {
		FramebufferTextureFormat TextureFormat;

		FramebufferTextureSpec() = default;
		FramebufferTextureSpec(FramebufferTextureFormat format) : TextureFormat(format) {}
		~FramebufferTextureSpec() = default;
	};

	struct FramebufferAttachmentSpec {
		Vector<FramebufferTextureSpec> Attachments;

		FramebufferAttachmentSpec() = default;
		FramebufferAttachmentSpec(std::initializer_list<FramebufferTextureSpec> attachments) : Attachments(attachments) {}
		~FramebufferAttachmentSpec() = default;
	};

	struct FrameBufferSpec {
		uint32 width = 0, height = 0;
		uint32 samples = 1;
		FramebufferAttachmentSpec attachments;

		bool swapChainTarget = false;
	};

	class FrameBuffer {
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32 x, uint32 y) = 0;
		virtual int ReadPixel(uint32 attachmentIndex, int x, int y) const = 0;

		virtual void ClearAttachment(uint32 attachmentIndex, int v) = 0;

		virtual const FrameBufferSpec& GetSpec() const = 0;
		virtual uint32 GetColorAttachmentRendererID(uint32 index = 0) const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpec& spec);
	};

	class UniformBuffer {
	public:
		virtual ~UniformBuffer() = default;

		virtual void SetData(const void* data, uint32 size, uint32 offset = 0) = 0;

		static Ref<UniformBuffer> Create(uint32 size, uint32 binding);
	};

}

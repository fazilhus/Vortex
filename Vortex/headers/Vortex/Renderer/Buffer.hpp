#pragma once

namespace Vortex {

	enum class ShaderDataType {
		None = 0,
		Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint ShaderDataTypeSize(ShaderDataType type) {
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
		uint size;
		uint offset;
		bool normalized;

		BufferElement() 
			: name(""), type(), size(0), offset(0), normalized(false) {}
		BufferElement(const std::string& name, ShaderDataType type, bool normalized = false) 
			: name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized) {}

		uint GetComponentCount() const {
			switch (type) {
				case ShaderDataType::Float:    return 1;
				case ShaderDataType::Float2:   return 2;
				case ShaderDataType::Float3:   return 3;
				case ShaderDataType::Float4:   return 4;
				case ShaderDataType::Mat3:     return 3 * 3;
				case ShaderDataType::Mat4:     return 4 * 4;
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
		std::vector<BufferElement> m_elements;
		uint m_stride;

	public:
		BufferLayout()
			: m_elements(std::vector<BufferElement>(0)), m_stride(0) {}
		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_elements(elements) {
			CalcOffsetAndStride();
		}
		BufferLayout(const BufferLayout& other) 
			: m_elements(other.GetElements()), m_stride(other.GetStride()) {
			VT_CORE_INFO("BufferLayout copy constructor {0} {1}", m_elements.size(), m_stride);
		}

		inline uint GetStride() const { return m_stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_elements; }

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

	private:
		void CalcOffsetAndStride() {
			uint offset = 0;
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
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* Create(float* vertices, uint size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint GetCount() const = 0;

		static IndexBuffer* Create(uint* indices, uint count);
	};

}
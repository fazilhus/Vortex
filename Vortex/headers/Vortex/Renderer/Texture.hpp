#pragma once

namespace Vortex {

	class Texture {
	public:
		virtual ~Texture() {}

		virtual uint GetWidth() const = 0;
		virtual uint GetHeight() const = 0;

		virtual void Bind(uint slot = 0) const = 0;

		virtual void SetData(void* data, uint size) = 0;
	};

	class Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(uint width, uint height);
	};

}
#pragma once

namespace Vortex {

	class Texture {
	public:
		virtual ~Texture() {}

		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;
		virtual const std::string& GetPath() const = 0;
		virtual uint32 GetID() const = 0;

		virtual void Bind(uint32 slot = 0) const = 0;

		virtual void SetData(void* data, uint32 size) = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(uint32 width, uint32 height);
	};

}
#pragma once

namespace Vortex {

	enum class RendererAPI {
		NONE = 0,
		OPENGL = 1
	};

	class Renderer {
	private:
		static RendererAPI s_rendererAPI;

	public:
		inline static RendererAPI GetAPI() { return s_rendererAPI; }
	};

}
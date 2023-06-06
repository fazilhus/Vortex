#pragma once
#include <vtpch.hpp>

namespace Vortex {

	class OpenGLContext : public GraphicsContext {
	private:
		GLFWwindow* m_windowHandle;
	
	public:
		OpenGLContext(GLFWwindow* window);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	};

}
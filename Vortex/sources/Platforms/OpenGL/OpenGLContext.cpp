#include <vtpch.hpp>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Platforms/OpenGL/OpenGLContext.hpp"

namespace Vortex {

	OpenGLContext::OpenGLContext(GLFWwindow* window) 
		: m_windowHandle(window) {
		VT_CORE_ASSERT(m_windowHandle, "Window handle is null");
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		VT_CORE_ASSERT(status, "Failed to init glad");

		VT_CORE_INFO("OpenGL Info:");
		std::string s(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		VT_CORE_INFO("  Vendor: {0}", s);
		s = std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		VT_CORE_INFO("  Renderer: {0}", s);
		s = std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
		VT_CORE_INFO("  Version: {0}", s);

		int major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		if (major <= 4 && (major != 4 || minor < 5))
		{
			VT_CORE_CRITICAL("Vortex requires at least OpenGL version 4.5");
			VT_CORE_ASSERT(false, "Vortex requires OpenGL version 4.5 or newer");
		}
	}

	void OpenGLContext::SwapBuffers() {
		VT_PROFILE_FUNC();
		glfwSwapBuffers(m_windowHandle);
	}

}
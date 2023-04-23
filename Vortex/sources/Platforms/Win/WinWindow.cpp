#include "vtpch.hpp"

#include "Platforms/Win/WinWindow.hpp"

namespace Vortex {

	static bool s_isGLFWInitialized = false;

	WinWindow* WinWindow::Create(const WindowProps& props) {
		return new WinWindow(props);
	}

	WinWindow::WinWindow(const WindowProps& props) {
		Init(props);
	}

	WinWindow::~WinWindow() {
		Shutdown();
	}

	void WinWindow::Init(const WindowProps& props) {
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;

		VT_CORE_INFO("Creating a window {0} ({1}, {2})", m_data.title, m_data.width, m_data.height);

		if (!s_isGLFWInitialized) {
			int success = glfwInit();
			VT_CORE_ASSERT(success, "Could not init GLFW");

			s_isGLFWInitialized = true;
		}

		m_window = glfwCreateWindow((int)m_data.width, (int)m_data.height, m_data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_data);
		SetVSync(true);
	}

	void WinWindow::Shutdown() {
		glfwDestroyWindow(m_window);
	}

	void WinWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void WinWindow::SetVSync(bool enabled) {
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_data.vsync = enabled;
	}

	
	bool WinWindow::IsVSync() const {
		return m_data.vsync;
	}

}
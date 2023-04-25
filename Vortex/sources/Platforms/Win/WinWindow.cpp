#include "vtpch.hpp"

#include "Platforms/Win/WinWindow.hpp"

// Events
#include "Vortex/Events/AppEvent.hpp"
#include "Vortex/Events/KeyEvent.hpp"
#include "Vortex/Events/MouseEvent.hpp"

#include "Platforms/OpenGL/OpenGLContext.hpp"

namespace Vortex {

	static bool s_isGLFWInitialized = false;

	static void GLFWErrorCallback(int code, const char* desc) {
		VT_CORE_ERROR("GLFW Error: {0} {1}", code, desc);
	}

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

			glfwSetErrorCallback(GLFWErrorCallback);
			s_isGLFWInitialized = true;
		}

		m_window = glfwCreateWindow((int)m_data.width, (int)m_data.height, m_data.title.c_str(), nullptr, nullptr);
		m_context = new OpenGLContext(m_window);
		m_context->Init();

		glfwSetWindowUserPointer(m_window, &m_data);
		SetVSync(true);

		// Setting GLFW event callbacks
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data->width = width;
			data->height = height;

			WindowResizeEvent event(width, height);
			data->eventCallback(event);
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowCloseEvent event;
			data->eventCallback(event);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data->eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data->eventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data->eventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, uint keycode) {
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			KeyTypedEvent event(keycode);
			data->eventCallback(event);
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data->eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data->eventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data->eventCallback(event);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseMovedEvent event((float)xPos, (float)yPos);
			data->eventCallback(event);
		});
	}

	void WinWindow::Shutdown() {
		glfwDestroyWindow(m_window);
	}

	void WinWindow::OnUpdate() {
		glfwPollEvents();
		m_context->SwapBuffers();
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
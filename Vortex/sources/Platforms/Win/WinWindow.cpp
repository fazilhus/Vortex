#include <vtpch.hpp>

#include "Platforms/Win/WinWindow.hpp"

// Events
#include "Vortex/Events/AppEvent.hpp"
#include "Vortex/Events/KeyEvent.hpp"
#include "Vortex/Events/MouseEvent.hpp"

#include "Platforms/OpenGL/OpenGLContext.hpp"

namespace Vortex {

	static uint4 s_GLFWwindowCount = 0;

	static void GLFWErrorCallback(int code, const char* desc) {
		VT_CORE_ERROR("GLFW Error: {0} {1}", code, desc);
	}

	Scope<Window> Window::Create(const WindowProps& props) {
		return CreateScope<WinWindow>(props);
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

		if (s_GLFWwindowCount == 0) {
			{
				("WinWindow::Init - GLFW init");
				VT_CORE_INFO("Initializing GLFW");
				int success = glfwInit();
				VT_CORE_ASSERT(success, "Could not init GLFW");

				glfwSetErrorCallback(GLFWErrorCallback);
			}
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		{
			("WinWindow::Init - GLFW create window");

#ifdef VT_DEBUG
			if (Renderer::GetAPI() == RendererAPI::API::OPENGL) {
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			}
#endif

			m_window = glfwCreateWindow((int)m_data.width, (int)m_data.height, m_data.title.c_str(), nullptr, nullptr);
			s_GLFWwindowCount++;
		}

		m_context = OpenGLContext::Create(m_window);
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
				KeyPressedEvent event(static_cast<KeyCode>(key), false);
				data->eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(static_cast<KeyCode>(key));
				data->eventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(static_cast<KeyCode>(key), true);
				data->eventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, uint4 keycode) {
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			KeyTypedEvent event(static_cast<KeyCode>(keycode));
			data->eventCallback(event);
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(static_cast<MouseCode>(button));
				data->eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
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

		if (--s_GLFWwindowCount == 0) {
			VT_CORE_INFO("Terminating GLFW");
			glfwTerminate();
		}
	}

	void WinWindow::OnUpdate(Timestep ts) {
		VT_PROFILE_FUNC();
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

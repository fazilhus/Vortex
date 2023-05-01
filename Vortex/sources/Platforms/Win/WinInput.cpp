#include "vtpch.hpp"
#include "Platforms/Win/WinInput.hpp"

#include "Vortex/Core/Application.hpp"

namespace Vortex {

    Input* Input::s_instance = new WinInput();

    bool WinInput::IsKeyPressedImpl(int keycode) {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        if (!window) return false;

        auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WinInput::IsMouseButtonPressedImpl(int button) {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        if (!window) return false;

        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> WinInput::GetMousePosImpl() {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        if (!window) return {0.0f, 0.0f};

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return {(float)xpos, (float)ypos};
    }

    float WinInput::GetMouseXImpl() {
        auto [x, y] = GetMousePosImpl();
        return x;
    }

    float WinInput::GetMouseYImpl() {
        auto [x, y] = GetMousePosImpl();
        return y;
    }

}
#include "vtpch.hpp"
#include "Platforms/Win/WinInput.hpp"

#include "Vortex/Core/Application.hpp"

namespace Vortex {

    Input* Input::s_instance = new WinInput();

    bool WinInput::IsKeyPressedImpl(KeyCode keycode) {
        VT_PROFILE_FUNCTION();
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        if (!window) return false;

        auto state = glfwGetKey(window, static_cast<int32>(keycode));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WinInput::IsMouseButtonPressedImpl(MouseCode button) {
        VT_PROFILE_FUNCTION();
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        if (!window) return false;

        auto state = glfwGetMouseButton(window, static_cast<int32>(button));
        return state == GLFW_PRESS;
    }

    std::pair<float, float> WinInput::GetMousePosImpl() {
        VT_PROFILE_FUNCTION();
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        if (!window) return {0.0f, 0.0f};

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return {(float)xpos, (float)ypos};
    }

    float WinInput::GetMouseXImpl() {
        VT_PROFILE_FUNCTION();
        auto [x, y] = GetMousePosImpl();
        return x;
    }

    float WinInput::GetMouseYImpl() {
        VT_PROFILE_FUNCTION();
        auto [x, y] = GetMousePosImpl();
        return y;
    }

}
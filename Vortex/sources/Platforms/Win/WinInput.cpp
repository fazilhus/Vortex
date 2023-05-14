#include <vtpch.hpp>

#include "Vortex/Core/Application.hpp"

namespace Vortex {

    bool Input::IsKeyPressed(KeyCode keycode) {
        const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        if (!window) return false;

        const auto state = glfwGetKey(window, static_cast<int32>(keycode));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(MouseCode button) {
        const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        if (!window) return false;

        const auto state = glfwGetMouseButton(window, static_cast<int32>(button));
        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::GetMousePos() {
        const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        if (!window) return {0.0f, 0.0f};

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return {static_cast<float>(xpos), static_cast<float>(ypos)};
    }

    float Input::GetMouseX() {
        auto [x, y] = GetMousePos();
        return x;
    }

    float Input::GetMouseY() {
        auto [x, y] = GetMousePos();
        return y;
    }

}
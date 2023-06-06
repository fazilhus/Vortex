//#include <vtpch.hpp>

#include "Vortex/Core/Input.hpp"

#include <glm/glm.hpp>
#include "Vortex/Core/Application.hpp"

namespace Vortex {

    bool Input::IsKeyPressed(const KeyCode keycode) {
        const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        if (!window) return false;

        const auto state = glfwGetKey(window, static_cast<int32>(keycode));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(const MouseCode button) {
        const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        if (!window) return false;

        const auto state = glfwGetMouseButton(window, static_cast<int32>(button));
        return state == GLFW_PRESS;
    }

    glm::vec2 Input::GetMousePos() {
        const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        if (!window) return {0.0f, 0.0f};

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return {static_cast<float>(xpos), static_cast<float>(ypos)};
    }

    float Input::GetMouseX() {
        auto pos = GetMousePos();
        return pos.x;
    }

    float Input::GetMouseY() {
        auto pos = GetMousePos();
        return pos.y;
    }

}
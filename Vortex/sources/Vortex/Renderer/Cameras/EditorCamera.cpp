#include <vtpch.hpp>
#include "Vortex/Renderer/Cameras/EditorCamera.hpp"

#include "Vortex/Core/Input.hpp"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_QUAT_DATA_WXYZ
#include <glm/gtx/quaternion.hpp>

namespace Vortex {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_fov(fov), m_aspectRatio(aspectRatio), m_nearClip(nearClip), m_farClip(farClip),
	Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)) {
		UpdateView();
	}

	void EditorCamera::OnUpdate(Timestep ts) {
		if (Input::IsKeyPressed(Key::LeftAlt)) {
			const glm::vec2& mouse{ Input::GetMousePos() };
			glm::vec2 delta = (mouse - m_mousePos) * 0.003f;
			m_mousePos = mouse;

			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle)) {
				MousePan(delta);
			}
			if (Input::IsMouseButtonPressed(Mouse::ButtonLeft)) {
				MouseRotate(delta);
			}
			if (Input::IsMouseButtonPressed(Mouse::ButtonRight)) {
				MouseZoom(delta.y);
			}
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e) {
		EventDispatcher dispatcher{ e };
		dispatcher.Dispatch<MouseScrolledEvent>(VT_BIND_EVENT_FN(OnMouseScroll));
	}

	glm::vec3 EditorCamera::GetUpDirection() const {
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const {
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const {
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::GetOrientation() const {
		return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
	}

	void EditorCamera::UpdateProjection() {
		m_aspectRatio = m_viewportWidth / m_viewportHeight;
		m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
	}

	void EditorCamera::UpdateView() {
		m_pos = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_viewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), m_pos) * glm::toMat4(orientation));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e) {
		float delta = e.GetOffsetY() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta) {
		auto [xSpeed, ySpeed] = PanSpeed();
		m_focalP += -GetRightDirection() * delta.x * xSpeed * m_dist;
		m_focalP += GetUpDirection() * delta.y * ySpeed * m_dist;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta) {
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_yaw += yawSign * delta.x * RotationSpeed();
		m_pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta) {
		m_dist -= delta * ZoomSpeed();
		if (m_dist < 1.0f) {
			m_focalP += GetForwardDirection();
			m_dist = 1.0f;
		}
	}

	glm::vec3 EditorCamera::CalculatePosition() const {
		return m_focalP - GetForwardDirection() * m_dist;
	}

	std::pair<float, float> EditorCamera::PanSpeed() const {
		float x = std::min(m_viewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_viewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const {
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const {
		float distance = m_dist * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

}

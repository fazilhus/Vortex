#pragma once
#include "Vortex/Renderer/Cameras/Camera.hpp"
#include "Vortex/Core/Timestep.hpp"
#include "Vortex/Events/Event.hpp"
#include "Vortex/Events/MouseEvent.hpp"

#include <glm/glm.hpp>

namespace Vortex {

	class EditorCamera : public Camera {
	private:
		float m_fov = 45.0f, m_aspectRatio = 1.778f, m_nearClip = 0.01f, m_farClip = 1000.0f;

		glm::mat4 m_viewMatrix{ 1.0f };
		glm::vec3 m_pos{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_focalP{ 0.0f, 0.0f, 0.0f };
		glm::vec2 m_mousePos{ 0.0f, 0.0f };
		
		float m_dist = 10.0f;
		float m_pitch = 0.0f, m_yaw = 0.0f;

		float m_viewportWidth = 1280, m_viewportHeight = 720;

	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);
		~EditorCamera() = default;

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline float GetDistance() const { return m_dist; }
		inline void SetDistance(float distance) { m_dist = distance; }

		inline void SetViewportSize(float width, float height) { m_viewportWidth = width; m_viewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
		glm::mat4 GetViewProjection() const { return m_projection * m_viewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_pos; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_pitch; }
		float GetYaw() const { return m_yaw; }

	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	};

}
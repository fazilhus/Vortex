#pragma once
#include "Vortex/Core/Core.hpp"
#include "Vortex/Renderer/Cameras/Camera.hpp"

namespace Vortex {

	class SceneCamera : public Camera {
	public:
		enum class ProjectionType {
			Perspective = 0,
			Orthographic = 1
		};

	private:
		ProjectionType m_projectionType;

		float m_orthographicSize, m_orthographicNear, m_orthographicFar;

		float m_perspectiveFOV, m_perspectiveNear, m_perspectiveFar;

		float m_aspectRatio;

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFov, float nearClip, float farClip);

		void SetViewportSize(uint32 width, uint32 height);

		float GetPerspectiveVerticalFOV() const { return m_perspectiveFOV; }
		void SetPerspectiveVerticalFOV(float verticalFov) {
			m_perspectiveFOV = verticalFov;
			RecalculateProjection();
		}
		float GetPerspectiveNearClip() const { return m_perspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) {
			m_perspectiveNear = nearClip;
			RecalculateProjection();
		}
		float GetPerspectiveFarClip() const { return m_perspectiveFar; }
		void SetPerspectiveFarClip(float farClip) {
			m_perspectiveFar = farClip;
			RecalculateProjection();
		}

		float GetOrthographicSize() const { return m_orthographicSize; }
		void SetOrthographicSize(float size) {
			m_orthographicSize = size;
			RecalculateProjection();
		}
		float GetOrthographicNearClip() const { return m_orthographicNear; }
		void SetOrthographicNearClip(float nearClip) {
			m_orthographicNear = nearClip;
			RecalculateProjection();
		}
		float GetOrthographicFarClip() const { return m_orthographicFar; }
		void SetOrthographicFarClip(float farClip) {
			m_orthographicFar = farClip;
			RecalculateProjection();
		}

		ProjectionType GetProjectionType() const { return m_projectionType; }
		void SetProjectionType(ProjectionType type) {
			m_projectionType = type;
			RecalculateProjection();
		}

	private:
		void RecalculateProjection();
	};

}

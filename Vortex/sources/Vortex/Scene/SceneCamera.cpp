#include <vtpch.hpp>
#include "Vortex/Scene/SceneCamera.hpp"

#include <glm/glm.hpp>

namespace Vortex {

	SceneCamera::SceneCamera() : 
		m_projectionType(ProjectionType::Orthographic),
		m_orthographicSize(10.0f), m_orthographicNear(-1.0f), m_orthographicFar(1.0f),
		m_perspectiveFOV(glm::radians(45.0f)), m_perspectiveNear(0.01f), m_perspectiveFar(1000.f),
		m_aspectRatio(0.0f) {
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
		m_projectionType = ProjectionType::Orthographic;
		m_orthographicSize = size;
		m_orthographicNear = nearClip;
		m_orthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFov, float nearClip, float farClip) {
		m_projectionType = ProjectionType::Perspective;
		m_perspectiveFOV = verticalFov;
		m_perspectiveNear = nearClip;
		m_perspectiveFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32 width, uint32 height) {
		VT_CORE_ASSERT(width > 0 && height > 0, "SceneCamera::SetViewportSize negative viewport dimesions");
		m_aspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection() {
		switch (m_projectionType) {
			case ProjectionType::Orthographic: {
				float orthoLeft = -m_orthographicSize * m_aspectRatio * 0.5f;
				float orthoRight = m_orthographicSize * m_aspectRatio * 0.5f;
				float orthoBottom = -m_orthographicSize * 0.5f;
				float orthoTop = m_orthographicSize * 0.5f;

				m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_orthographicNear, m_orthographicFar);
				break;
			}
			case ProjectionType::Perspective: {
				m_projection = glm::perspective(m_perspectiveFOV, m_aspectRatio, m_perspectiveNear, m_perspectiveFar);
				break;
			}
		}
		
	}

}

#include <vtpch.hpp>
#include "Vortex/Scene/SceneCamera.hpp"

#include <glm/glm.hpp>

namespace Vortex {

	void SceneCamera::SetOrtho(float size, float orthoNear, float orthoFar) {
		m_orthoSize = size;
		m_orthoNear = orthoNear;
		m_orthoFar = orthoFar;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32 width, uint32 height) {
		m_aspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection() {
		float orthoLeft = -m_orthoSize * m_aspectRatio * 0.5f;
		float orthoRight = m_orthoSize * m_aspectRatio * 0.5f;
		float orthoBottom = -m_orthoSize * 0.5f;
		float orthoTop = m_orthoSize * 0.5f;

		m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_orthoNear, m_orthoFar);
	}

}
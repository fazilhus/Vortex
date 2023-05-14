#include <vtpch.hpp>
#include "Vortex/Renderer/Cameras/OrthoCamera.hpp"

namespace Vortex{

	OrthoCamera::OrthoCamera(float left, float right, float bottom, float top) 
		: m_proj(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_view(glm::mat4(1.0f)), m_vp(glm::mat4()), 
		m_pos(glm::vec3()), m_rot(0.0f) {
		m_vp = m_proj * m_view;
	}

	void OrthoCamera::SetProjection(float left, float right, float bottom, float top) {
		m_proj = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_vp = m_proj * m_view;
	}

	void OrthoCamera::CalcViewMat() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_pos) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rot), glm::vec3(0, 0, 1));

		m_view = glm::inverse(transform);
		m_vp = m_proj * m_view;
	}

}
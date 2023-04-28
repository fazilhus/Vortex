#pragma once
#include "vtpch.hpp"
#include <glm/glm.hpp>

namespace Vortex {

	class OrthoCamera {
	private:
		glm::mat4 m_proj;
		glm::mat4 m_view;
		glm::mat4 m_vp; // dot product of view and proj

		glm::vec3 m_pos;
		float m_rot;

	public:
		OrthoCamera(float left, float right, float bottom, float top);

		inline const glm::vec3& GetPos() const { return m_pos; }
		void SetPos(const glm::vec3& pos) { 
			m_pos = pos;
			CalcViewMat();
		}

		inline float GetRot() const { return m_rot; }
		void SetRot(float rot) {
			m_rot = rot;
			CalcViewMat();
		}

		inline const glm::mat4& GetProjMat() const { return m_proj; }
		inline const glm::mat4& GetViewMat() const { return m_view; }
		inline const glm::mat4& GetViewProjMat() const { return m_vp; }

	private:
		void CalcViewMat();
	};

}
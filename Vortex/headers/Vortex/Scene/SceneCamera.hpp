#pragma once
#include "Vortex/Core/Core.hpp"
#include "Vortex/Renderer/Cameras/Camera.hpp"

namespace Vortex {

	class SceneCamera : public Camera {
	private:
		float m_orthoSize, m_orthoNear, m_orthoFar;
		float m_aspectRatio;

	public:
		SceneCamera() : m_orthoSize(10.0f), m_orthoNear(-1.0f), m_orthoFar(1.0f), m_aspectRatio(0.0f) {
			RecalculateProjection();
		}
		virtual ~SceneCamera() = default;

		void SetOrtho(float size, float orthoNear, float orhtoFar);
		void SetViewportSize(uint32 width, uint32 height);

		float GetOrthoSize() const { return m_orthoSize; }
		void SetOrthoSize(float size) { 
			m_orthoSize = size;
			RecalculateProjection();
		}

	private:
		void RecalculateProjection();
	};

}

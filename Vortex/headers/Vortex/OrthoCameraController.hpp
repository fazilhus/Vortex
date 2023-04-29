#pragma once

#include "vtpch.hpp"
#include "Vortex/Events/MouseEvent.hpp"
#include "Vortex/Events/AppEvent.hpp"

namespace Vortex {

	class OrthoCameraController {
	private:
		float m_aspectRatio;
		float m_zoomLevel = 1.0f;
		OrthoCamera m_camera;

		bool m_rot;

		glm::vec3 m_cameraPos = { 0.0f, 0.0f, 0.0f };
		float m_cameraRot = 0.0f;

		float m_cameraPosSpeed = 5.0f;
		float m_cameraRotSpeed = 180.0f;

	public:
		OrthoCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthoCamera& GetCamera() { return m_camera; }
		const OrthoCamera& GetCamera() const { return m_camera; }
		float GetZoomLevel() const { return m_zoomLevel; }
		void SetZoomLevel(float zoomLevel) { m_zoomLevel = zoomLevel; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	};

}
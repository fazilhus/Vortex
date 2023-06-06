#include <vtpch.hpp>
#include "Vortex/Controllers/OrthoCameraController.hpp"

namespace Vortex {

	OrthoCameraController::OrthoCameraController(float aspectRatio, bool rotation)
	: m_aspectRatio(aspectRatio),
	m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel),
	m_rot(rotation) {
	}

	void OrthoCameraController::OnUpdate(Timestep ts) {
		if (Vortex::Input::IsKeyPressed(Key::D) || Vortex::Input::IsKeyPressed(Key::Right)) {
			m_cameraPos.x -= cos(glm::radians(m_cameraRot)) * m_cameraPosSpeed * ts;
			m_cameraPos.y -= sin(glm::radians(m_cameraRot)) * m_cameraPosSpeed * ts;
		}
		else if (Vortex::Input::IsKeyPressed(Key::A) || Vortex::Input::IsKeyPressed(Key::Left)) {
			m_cameraPos.x += cos(glm::radians(m_cameraRot)) * m_cameraPosSpeed * ts;
			m_cameraPos.y += sin(glm::radians(m_cameraRot)) * m_cameraPosSpeed * ts;
		}
		if (Vortex::Input::IsKeyPressed(Key::W) || Vortex::Input::IsKeyPressed(Key::Up)) {
			m_cameraPos.x += -sin(glm::radians(m_cameraRot)) * m_cameraPosSpeed * ts;
			m_cameraPos.y += cos(glm::radians(m_cameraRot)) * m_cameraPosSpeed * ts;
		}
		else if (Vortex::Input::IsKeyPressed(Key::S) || Vortex::Input::IsKeyPressed(Key::Down)) {
			m_cameraPos.x -= -sin(glm::radians(m_cameraRot)) * m_cameraPosSpeed * ts;
			m_cameraPos.y -= cos(glm::radians(m_cameraRot)) * m_cameraPosSpeed * ts;
		}

		m_camera.SetPos(m_cameraPos);

		if (m_rot) {
			if (Vortex::Input::IsKeyPressed(Key::Q)) {
				m_cameraRot -= m_cameraRotSpeed * ts;
			}
			else if (Vortex::Input::IsKeyPressed(Key::E)) {
				m_cameraRot += m_cameraRotSpeed * ts;
			}

			if (m_cameraRot > 180.0f) {
				m_cameraRot -= 360.0f;
			}
			else if (m_cameraRot < - 180.0f) {
				m_cameraRot += 360.0f;
			}

			m_camera.SetRot(m_cameraRot);
		}

		m_cameraPosSpeed = m_zoomLevel;
	}

	void OrthoCameraController::OnEvent(Event& e) {
		VT_CORE_TRACE("OrthoCameraController::OnEvent {0}", e.GetName());
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(VT_BIND_EVENT_FN(OrthoCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(VT_BIND_EVENT_FN(OrthoCameraController::OnWindowResized));
	}

	void OrthoCameraController::OnResize(float x, float y) {
		m_aspectRatio = x / y;
		m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
	}

	bool OrthoCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		m_zoomLevel -= e.GetOffsetY() * 0.25f;
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		VT_CORE_TRACE("OrthoCameraController::OnEvent dispatched Mouse scroll event");
		return false;
	}

	bool OrthoCameraController::OnWindowResized(WindowResizeEvent& e) {
		OnResize(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
		VT_CORE_TRACE("Window resize event");
		return false;
	}
}

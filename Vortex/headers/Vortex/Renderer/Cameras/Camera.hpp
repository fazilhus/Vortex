#pragma once
#include <glm/glm.hpp>

namespace Vortex {

	class Camera {
	protected:
		glm::mat4 m_projection;

	public:
		Camera() : m_projection(glm::mat4{1.0f}) {}
		Camera(const glm::mat4& projection) : m_projection(projection) {}
		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_projection; }
	};

}
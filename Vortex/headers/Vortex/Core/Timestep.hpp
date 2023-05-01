#pragma once

namespace Vortex {

	class Timestep {
	private:
		float m_time;

	public:
		Timestep(float time = 0.0f) : m_time(time) {}
		~Timestep() = default;

		operator float() const { return m_time; }

		float GetSeconds() const { return m_time; }
		float GetMilliSeconds() const { return m_time * 1000.0f; }
	};

}
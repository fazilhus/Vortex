#pragma once
#include <entt/entt.hpp>

namespace Vortex {

	class Entity;

	class Scene {
	private:
		entt::registry m_registry;
		uint32 m_viewportWidth, m_viewportHeight;

		friend class Entity;

	public:
		Scene() : m_viewportWidth(0), m_viewportHeight(0) {}
		~Scene() = default;

		Entity CreateEntity(const std::string& tag);

		// temporary
		entt::registry& GetRegistry() { return m_registry; }

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32 width, uint32 height);
	};

}
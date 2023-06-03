#pragma once
#include <entt/entt.hpp>

namespace Vortex {

	class Entity;

	class Scene {
	private:
		entt::registry m_registry;
		friend class Entity;

	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& tag);

		// temporary
		entt::registry& GetRegistry() { return m_registry; }

		void OnUpdate(Timestep ts);
	};

}
#pragma once
#include <entt/entt.hpp>

namespace Vortex {

	class Scene {
	private:
		entt::registry m_registry;

	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		// temporary
		entt::registry& GetRegistry() { return m_registry; }

		void OnUpdate(Timestep ts);
	};

}
#pragma once
#include <entt/entt.hpp>
#include "Vortex/Core/Timestep.hpp"

namespace Vortex {

	class Entity;

	class Scene {
	private:
		entt::registry m_registry;
		uint32 m_viewportWidth, m_viewportHeight;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

	public:
		Scene() : m_viewportWidth(0), m_viewportHeight(0) {}
		~Scene() = default;

		Entity CreateEntity(const std::string& tag);
		void DestroyEntity(Entity entity);

		// temporary
		entt::registry& GetRegistry() { return m_registry; }

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32 width, uint32 height);

	private:
		template <typename T>
		void OnComponentAdded(Entity entity, T& component);
	};

}
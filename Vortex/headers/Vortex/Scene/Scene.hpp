#pragma once
#include <entt/entt.hpp>
#include "Vortex/Core/Timestep.hpp"
#include "Vortex/Renderer/Cameras/EditorCamera.hpp"

class b2World;

namespace Vortex {

	class Entity;

	class Scene {
	private:
		entt::registry m_registry;
		uint32 m_viewportWidth, m_viewportHeight;
		b2World* m_physicsWorld;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

	public:
		Scene() : m_viewportWidth(0), m_viewportHeight(0), m_physicsWorld(nullptr) {}
		~Scene() = default;

		Entity CreateEntity(const std::string& tag);
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32 width, uint32 height);

		void OnRuntimStart();
		void OnRuntimeStop();

		Entity GetPrimaryCamera();

	private:
		template <typename T>
		void OnComponentAdded(Entity entity, T& component);
	};

}

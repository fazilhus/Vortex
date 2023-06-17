#pragma once
#include <entt/entt.hpp>
#include "Vortex/Core/Timestep.hpp"
#include "Vortex/Renderer/Cameras/EditorCamera.hpp"
#include "Vortex/Core/UUID.hpp"

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

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& tag);
		Entity CreateEntityWithUUID(UUID id, const std::string& tag);
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32 width, uint32 height);

		void OnRuntimStart();
		void OnRuntimeResume();
		void OnRuntimePause();
		void OnRuntimeStop();

		void DuplicateEntity(Entity entity);

		Entity GetPrimaryCamera();

		template <typename ...Components>
		auto GetAllEntitiesWith() {
			return m_registry.view<Components...>();
		}

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		template<typename T>
		static void CopyComponent(entt::registry& dst, entt::registry& src, const HashMap<UUID, entt::entity> enttMap);

		template<typename T>
		static void CopyComponentIfExists(Entity dst, Entity src);
	};

}

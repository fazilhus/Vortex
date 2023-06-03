#include <entt/entt.hpp>
#include "Vortex/Scene/Scene.hpp"

namespace Vortex {

	class Entity {
	private:
		entt::entity m_entityHandle;
		Scene* m_scene;

	public:
		Entity() : m_entityHandle(entt::null), m_scene(nullptr) {}
		Entity(entt::entity handle, Scene* scene) : m_entityHandle(handle), m_scene(scene) {}
		Entity(const Entity& other) = default;
		~Entity() = default;

		Entity& operator=(const Entity& other) = default;

		template <typename T, typename... Args>
		T& AddComponent(Args&& ...args) {
			VT_CORE_ASSERT(!HasComponent<T>(), "Entity already has such component");
			return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		}

		template <typename T>
		bool RemoveComponent() {
			if (!HasComponent<T>()) {
				VT_CORE_WARN("Entity does not have such component");
				return false;
			}
			m_scene->m_registry.remove<T>(m_entityHandle);
			return true;
		}

		template <typename T>
		bool HasComponent() {
			return m_scene->m_registry.all_of<T>(m_entityHandle);
		}

		operator bool() const { return m_entityHandle != entt::null; }

		template <typename T>
		T& GetComponent() {
			VT_CORE_ASSERT(!HasComponent<T>(), "Entity already has such component");
			return m_scene->m_registry.get<T>(m_entityHandle);
		}
	};

}
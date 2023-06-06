#pragma once
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
			VT_CORE_INFO("Added {0} to entity {1}", typeid(T).name(), (uint32)m_entityHandle);
			VT_CORE_ASSERT(!HasComponent<T>(), "Entity already has such component");
			return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		}

		template <typename T>
		bool RemoveComponent() {
			if (!HasComponent<T>()) {
				VT_CORE_WARN("Entity {0} does not have {1} component", (uint32)m_entityHandle, typeid(T).name());
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
		operator uint32() const { return static_cast<uint32>(m_entityHandle); }

		bool operator==(const Entity& other) { return m_entityHandle == other.m_entityHandle && m_scene == other.m_scene; }
		bool operator!=(const Entity& other) { return !(*this == other); }

		template <typename T>
		T& GetComponent() {
			if (!HasComponent<T>()) {
				VT_CORE_WARN("Entity {0} does not have {1} component", (uint32)m_entityHandle, typeid(T).name());
				VT_CORE_ASSERT(!HasComponent<T>(), "");
			}
			return m_scene->m_registry.get<T>(m_entityHandle);
		}
	};

}
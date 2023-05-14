#pragma once
#include "Vortex/ECS/Config.hpp"
#include "Vortex/ECS/Component.hpp"
#include "Vortex/ECS/System.hpp"

namespace Vortex {

	namespace ecs {

		class ECSManager {
		private:
			Vector<std::pair<EntityID, Vector<std::pair<ComponentID, uint32>>>*> m_entities;
			HashMap<ComponentID, Vector<uint8>> m_components;
			Vector<BaseSystem*> m_systems;

		public:
			ECSManager() = default;
			ECSManager(const ECSManager& other) = delete;
			ECSManager(ECSManager&&) = delete;
			~ECSManager() = default;

			ECSManager& operator=(const ECSManager& other) = delete;
			ECSManager& operator=(ECSManager&&) = delete;

			// Entity
			EntityHandle CreateEntity(const BaseComponent* components, const ComponentID* compIDs, std::size_t num);
			void DeleteEntity(EntityHandle e);

			// Component
			template <typename Component>
			void AddComponent(EntityHandle e, const Component* comp);

			template <typename Component>
			void RemoveComponent(EntityHandle e);

			template <typename Component>
			Component* GetComponent(EntityHandle e);

			template <typename Component>
			const Component* GetComponent(EntityHandle e) const;

			// System
			inline void AddSystem(BaseSystem& s) {
				m_systems.push_back(&s);
			}

			void RemoveSystem(BaseSystem& system);
			void UpdateSystems(Timestep ts);

		private:
			inline std::pair<EntityID, Vector<std::pair<ComponentID, uint32>>>* HandleToRaw(EntityHandle e) {
				return (std::pair<EntityID, Vector<std::pair<ComponentID, uint32>>>*)e;
			}

			inline EntityID HandleToEntityID(EntityHandle e) {
				return HandleToRaw(e)->first;
			}

			inline Vector<std::pair<ComponentID, uint32>>& HandleToEntity(EntityHandle e) {
				return HandleToRaw(e)->second;
			}
		};

	}// namespace ecs

}// namespace Vortex
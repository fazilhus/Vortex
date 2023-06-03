#pragma once
#include "Vortex/ECS/Config.hpp"
#include "Vortex/ECS/Component.hpp"
#include "Vortex/ECS/System.hpp"

namespace Vortex {

	namespace ecs {

		class Manager {
		private:
			Vector<std::pair<EntityID, Vector<std::pair<ComponentID, uint32>>>*> m_entities;
			HashMap<ComponentID, Vector<uint8>> m_components;
			SystemsList m_systems;

		public:
			Manager() = default;
			Manager(const Manager& other) = delete;
			Manager(Manager&&) = delete;
			~Manager();
			Manager& operator=(const Manager& other) = delete;
			Manager& operator=(Manager&&) = delete;

			// Entity
			EntityHandle CreateEntity();
			EntityHandle CreateEntity(BaseComponent** components, const ComponentID* compIDs, std::size_t num);

			template <class ...Comp>
			EntityHandle CreateEntity(Comp& ...comp) {
				BaseComponent* comps[] = { &(comp)... };
				ComponentID compIDs[] = { std::remove_reference_t<Comp>::ID... };
				return CreateEntity(comps, compIDs, sizeof...(Comp));
			}

			void DeleteEntity(EntityHandle e);

			// Component
			template <typename Component>
			void AddComponent(EntityHandle handle, const Component* comp) {
				AddComponentInternal(handle, HandleToEntity(handle), Component::ID, (BaseComponent*)comp);
			}

			template <typename Component>
			bool RemoveComponent(EntityHandle handle) {
				return RemoveComponentInternal(handle, Component::ID);
			}

			template <typename Component>
			Component* GetComponent(EntityHandle handle) {
				return (Component*)GetComponentInternal(HandleToEntity(handle), m_components[Component::ID], Component::ID);
			}

			// System
			bool AddSystem(BaseSystem& s) {
				// TODO memory leak
				auto s1 = new BaseSystem(s);
				return m_systems.AddSystem(*s1);
			}
			bool RemoveSystem(BaseSystem& s) {
				return m_systems.RemoveSystem(s);
			}

			void OnUpdate(Timestep ts);

			void UpdateSystems(Timestep ts, SystemsList& systems);

		private:
			std::pair<EntityID, Vector<std::pair<ComponentID, uint32>>>* HandleToRaw(EntityHandle e) {
				return (std::pair<EntityID, Vector<std::pair<ComponentID, uint32>>>*)e;
			}

			EntityID HandleToEntityID(EntityHandle e) {
				return HandleToRaw(e)->first;
			}

			Vector<std::pair<ComponentID, uint32>>& HandleToEntity(EntityHandle e) {
				return HandleToRaw(e)->second;
			}

			template <typename Head>
			void AddToVector(std::vector<BaseComponent*> comps, std::vector<ComponentID> compIDs, Head head) {
				comps.push_back(&head);
				compIDs.push_back(Head::ID);
			}

			void DeleteComponent(ComponentID id, uint32 index);

			void AddComponentInternal(EntityHandle handle, Vector<std::pair<ComponentID, uint32>>& entity, ComponentID id, BaseComponent* comp);
			bool RemoveComponentInternal(EntityHandle handle, ComponentID id);
			BaseComponent* GetComponentInternal(Vector<std::pair<ComponentID, uint32>>& entity, Vector<uint8>& v, ComponentID id);

			ComponentID FindLeastCommonComponent(const Vector<ComponentID>& componentTypes, const Vector<ComponentFlag>& componentFlags);

			void UpdateSystemWithMultipleComponents(uint32 index, Timestep ts, SystemsList& systems, const Vector<ComponentID>& compTypes,
				Vector<BaseComponent*>& componentParam, Vector<Vector<uint8>*>& componentVectors);
		};

	}// namespace ecs

}// namespace Vortex
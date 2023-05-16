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
			~ECSManager() {
				for (auto it = m_components.begin(); it != m_components.end(); ++it) {
					auto size = BaseComponent::GetTypeSize(it->first);
					ComponentDeleteFunction delfn = BaseComponent::GetTypeDeleteFunction(it->first);
					for (uint32 i = 0; i < it->second.size(); ++i) {
						delfn((BaseComponent*)&it->second[i]);
					}
				}

				for (uint32 i = 0; i < m_entities.size(); ++i) {
					delete m_entities[i];
				}
			}

			ECSManager& operator=(const ECSManager& other) = delete;
			ECSManager& operator=(ECSManager&&) = delete;

			// Entity
			EntityHandle CreateEntity(BaseComponent* components, const ComponentID* compIDs, std::size_t num) {
				auto newEntity = new std::pair<EntityID, Vector<std::pair<ComponentID, uint32>>>;
				EntityHandle handle = (EntityHandle)newEntity;

				for (uint32 i = 0; i < num; ++i) {
					if (!BaseComponent::IsTypeValid(compIDs[i])) {
						VT_CORE_ERROR("ECSManager::CreateEntity Could not create entity: %u is not a valid ComponentID", compIDs[i]);
						delete newEntity;
						return nullptr;
					}

					AddComponentInternal(handle, newEntity->second, compIDs[i], &components[i]);
				}

				newEntity->first = m_entities.size();
				m_entities.push_back(newEntity);
				return handle;
			}

			void DeleteEntity(EntityHandle e) {
				auto& entity = HandleToEntity(e);

				for (uint32 i = 0; i < entity.size(); ++i) {
					DeleteComponent(entity[i].first, entity[i].second);
				}

				EntityID destInd = HandleToEntityID(e);
				EntityID srcInd = m_entities.size() - 1;
				delete m_entities[destInd];
				m_entities[destInd] = m_entities[srcInd];
				m_entities.pop_back();
			}

			// Component
			template <typename Component>
			void AddComponent(EntityHandle handle, const Component* comp) {
				AddComponentInternal(handle, HandleToEntity(handle), Component::ID, comp);
			}

			template <typename Component>
			bool RemoveComponent(EntityHandle handle) {
				return RemoveComponentInternal(handle, Component::ID);
			}

			template <typename Component>
			Component* GetComponent(EntityHandle handle) {
				return GetComponentInternal(HandleToEntity(handle), m_components[Component::ID], Component::ID);
			}

			// System
			inline void AddSystem(BaseSystem& s) {
				m_systems.push_back(&s);
			}

			inline bool RemoveSystem(BaseSystem& system) {
				for (uint32 i = 0; i < m_systems.size(); ++i) {
					if (&system == m_systems[i]) {
						m_systems.erase(m_systems.begin() + i); // erase because we need to have the same order of systems after removal
						return true;
					}
				}
				return false;
			}

			inline void UpdateSystems(Timestep ts) {
				Vector<BaseComponent*> componentParam;
				Vector<Vector<uint8>*> componentVectors;

				for (uint32 i = 0; i < m_systems.size(); ++i) {
					const Vector<ComponentID>& compTypes = m_systems[i]->GetComponentTypes();
					if (compTypes.size() == 1) {
						std::size_t size = BaseComponent::GetTypeSize(compTypes[0]);
						Vector<uint8>& v = m_components[compTypes[0]];

						for (uint32 j = 0; j < v.size(); ++j) {
							auto comp = (BaseComponent*)&v[j];
							m_systems[i]->OnUpdate(ts, &comp);
						}
					}
					else {
						UpdateSystemWithMultipleComponents(i, ts, compTypes, componentParam, componentVectors);
					}
				}
			}

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

			inline void DeleteComponent(ComponentID id, uint32 index) {
				Vector<uint8>& v = m_components[id];
				ComponentDeleteFunction delfn = BaseComponent::GetTypeDeleteFunction(id);
				std::size_t size = BaseComponent::GetTypeSize(id);
				uint32 srcInd = v.size() - size;

				auto dest = (BaseComponent*)&v[index];
				auto src = (BaseComponent*)&v[srcInd];
				delfn(dest);

				if (index == srcInd) {
					v.resize(srcInd);
					return;
				}

				memcpy(dest, src, size);
				Vector<std::pair<ComponentID, uint32>>& srcComps = HandleToEntity(src->m_entity);
				for (uint32 i = 0; i < srcComps.size(); ++i) {
					if (id == srcComps[i].first && srcInd == srcComps[i].second) {
						srcComps[i].second = index;
						break;
					}
				}
				v.resize(srcInd);
			}

			inline void AddComponentInternal(EntityHandle handle, Vector<std::pair<ComponentID, uint32>>& entity, ComponentID id, BaseComponent* comp) {
				ComponentCreateFunction createfn = BaseComponent::GetTypeCreateFunction(id);
				std::pair<ComponentID, uint32> newPair;
				newPair.first = id;
				newPair.second = createfn(m_components[id], handle, comp);
				entity.push_back(newPair);
			}
			inline bool RemoveComponentInternal(EntityHandle handle, ComponentID id) {
				Vector<std::pair<ComponentID, uint32>>& entityComps = HandleToEntity(handle);

				for (uint32 i = 0; i < entityComps.size(); ++i) {
					if (id == entityComps[i].first) {
						DeleteComponent(entityComps[i].first, entityComps[i].second);
						uint32 srcInd = entityComps.size() - 1;
						uint32 destInd = i;
						entityComps[destInd] = entityComps[srcInd];
						entityComps.pop_back();
						return true;
					}
				}
				return false;
			}
			inline BaseComponent* GetComponentInternal(Vector<std::pair<ComponentID, uint32>>& entity, Vector<uint8>& v, ComponentID id) {
				for (uint32 i = 0; i < entity.size(); ++i) {
					if (id == entity[i].first) {
						return (BaseComponent*)&v[entity[i].second];
					}
				}
				return nullptr;
			}

			inline ComponentID FindLeastCommonComponent(const Vector<ComponentID>& componentTypes) {
				uint32 minSize = m_components[componentTypes[0]].size() / BaseComponent::GetTypeSize(componentTypes[0]);
				uint32 minInd = 0;

				for (uint32 i = 0; i < componentTypes.size(); ++i) {
					uint32 size = m_components[componentTypes[i]].size() / BaseComponent::GetTypeSize(componentTypes[i]);
					if (size < minSize) {
						minSize = size;
						minInd = i;
					}
				}
				return minInd;
			}

			inline void UpdateSystemWithMultipleComponents(uint32 index, Timestep ts, const Vector<ComponentID>& compTypes,
				Vector<BaseComponent*>& componentParam, Vector<Vector<uint8>*>& componentVectors) {
				componentParam.resize(std::max(componentParam.size(), compTypes.size()));
				componentVectors.resize(std::max(componentVectors.size(), compTypes.size()));

				for (uint32 i = 0; i < compTypes.size(); ++i) {
					componentVectors[i] = &m_components[compTypes[i]];
				}

				uint32 minSizeInd = FindLeastCommonComponent(compTypes);

				std::size_t size = BaseComponent::GetTypeSize(compTypes[minSizeInd]);
				Vector<uint8>& v = *componentVectors[minSizeInd];

				for (uint32 i = 0; i < v.size(); ++i) {
					componentParam[minSizeInd] = (BaseComponent*)&v[i];
					Vector<std::pair<ComponentID, uint32>>& entity = HandleToEntity(componentParam[minSizeInd]->m_entity);

					bool isValid = true;
					for (uint32 j = 0; j < compTypes.size(); ++j) {
						if (j == minSizeInd) {
							continue;
						}

						componentParam[j] = GetComponentInternal(entity, *componentVectors[j], compTypes[j]);
						if (componentParam[j] == nullptr) {
							isValid = false;
							break;
						}
					}

					if (isValid) {
						m_systems[index]->OnUpdate(ts, &componentParam[0]);
					}
				}
			}
		};

	}// namespace ecs

}// namespace Vortex
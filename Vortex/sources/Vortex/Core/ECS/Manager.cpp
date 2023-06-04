#include <vtpch.hpp>
#include "Vortex/ECS/Manager.hpp"
#include <climits>

namespace Vortex {

	namespace ecs {

		Manager::~Manager() {
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

		EntityHandle Manager::CreateEntity() {
			auto newEntity = new std::pair<EntityID, Vector<std::pair<ComponentID, uint32>>>;
			m_entities.push_back(newEntity);
			return (EntityHandle)newEntity;
		}

		EntityHandle Manager::CreateEntity(BaseComponent** components, const ComponentID* compIDs, std::size_t num) {
			auto newEntity = new std::pair<EntityID, Vector<std::pair<ComponentID, uint32>>>;
			EntityHandle handle = (EntityHandle)newEntity;

			for (uint32 i = 0; i < num; ++i) {
				if (!BaseComponent::IsTypeValid(compIDs[i])) {
					VT_CORE_ERROR("Manager::CreateEntity Could not create entity: %u is not a valid ComponentID", compIDs[i]);
					delete newEntity;
					return nullptr;
				}

				AddComponentInternal(handle, newEntity->second, compIDs[i], components[i]);
			}

			newEntity->first = m_entities.size();
			m_entities.push_back(newEntity);
			return handle;
		}

		void Manager::DeleteEntity(EntityHandle e) {
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

		void Manager::OnUpdate(Timestep ts) {
			UpdateSystems(ts, m_systems);
		}

		void Manager::UpdateSystems(Timestep ts, SystemsList& systems) {
			Vector<BaseComponent*> componentParam;
			Vector<Vector<uint8>*> componentVectors;

			for (uint32 i = 0; i < systems.Size(); ++i) {
				const Vector<ComponentID>& compTypes = systems[i]->GetComponentTypes();
				if (compTypes.size() < 1) { 
					VT_CORE_ERROR("System has no registered component types");
					return;
				}
				else if (compTypes.size() == 1) {
					std::size_t size = BaseComponent::GetTypeSize(compTypes[0]);
					Vector<uint8>& v = m_components[compTypes[0]];

					for (uint32 j = 0; j < v.size(); ++j) {
						auto comp = (BaseComponent*)&v[j];
						systems[i]->OnUpdate(ts, &comp);
					}
				}
				else {
					UpdateSystemWithMultipleComponents(i, ts, systems, compTypes, componentParam, componentVectors);
				}
			}
		}

		void Manager::DeleteComponent(ComponentID id, uint32 index) {
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

		void Manager::AddComponentInternal(EntityHandle handle, Vector<std::pair<ComponentID, uint32>>& entity, ComponentID id, BaseComponent* comp) {
			ComponentCreateFunction createfn = BaseComponent::GetTypeCreateFunction(id);
			std::pair<ComponentID, uint32> newPair;
			newPair.first = id;
			newPair.second = createfn(m_components[id], handle, comp);
			entity.push_back(newPair);
		}

		bool Manager::RemoveComponentInternal(EntityHandle handle, ComponentID id) {
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

		BaseComponent* Manager::GetComponentInternal(Vector<std::pair<ComponentID, uint32>>& entity, Vector<uint8>& v, ComponentID id) {
			for (uint32 i = 0; i < entity.size(); ++i) {
				if (id == entity[i].first) {
					return (BaseComponent*)&v[entity[i].second];
				}
			}
			return nullptr;
		}

		ComponentID Manager::FindLeastCommonComponent(const Vector<ComponentID>& componentTypes, const Vector<ComponentFlag>& componentFlags) {
			/*uint32 minSize = m_components[componentTypes[0]].size() / BaseComponent::GetTypeSize(componentTypes[0]);
			uint32 minInd = 0;*/
			uint32 minSize = UINT32_MAX;
			uint32 minInd = UINT32_MAX;

			for (uint32 i = 0; i < componentTypes.size(); ++i) {
				VT_CORE_TRACE("Manager::FindLeastCommonComponent Component {0} with flag {1}", componentTypes[i], componentFlags[i]);
				if ((componentFlags[i] & ComponentFlag::FLAG_OPTIONAL) != 0) continue;

				uint32 size = m_components[componentTypes[i]].size() / BaseComponent::GetTypeSize(componentTypes[i]);
				if (size < minSize) {
					minSize = size;
					minInd = i;
				}
			}
			VT_CORE_INFO("Manager::FindLeastCommonComponent result {0}", minInd);
			return minInd;
		}

		void Manager::UpdateSystemWithMultipleComponents(uint32 index, Timestep ts, SystemsList& systems, 
			const Vector<ComponentID>& compTypes, Vector<BaseComponent*>& componentParam, Vector<Vector<uint8>*>& componentVectors) {
			componentParam.resize(std::max(componentParam.size(), compTypes.size()));
			componentVectors.resize(std::max(componentVectors.size(), compTypes.size()));

			const Vector<ComponentFlag>& compFlags = systems[0]->GetComponentFlags();
			VT_CORE_TRACE("Manager::UpdateSystemWithMultipleComponents flags size {0}", compFlags.size());

			for (uint32 i = 0; i < compTypes.size(); ++i) {
				componentVectors[i] = &m_components[compTypes[i]];
			}

			uint32 minSizeInd = FindLeastCommonComponent(compTypes, compFlags);

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
					if (componentParam[j] == nullptr && (compFlags[j] & ComponentFlag::FLAG_OPTIONAL) == 0) {
						isValid = false;
						break;
					}
				}

				if (isValid) {
					systems[index]->OnUpdate(ts, &componentParam[0]);
				}
			}
		}
	
	}

}
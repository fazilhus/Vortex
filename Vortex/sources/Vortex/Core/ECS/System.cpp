#include <vtpch.hpp>
#include "Vortex/ECS/System.hpp"

namespace Vortex {

	namespace ecs {

		bool BaseSystem::IsValid() {
			for (uint32 i = 0; i < m_componentFlags.size(); ++i) {
				if ((m_componentFlags[i] & ComponentFlag::FLAG_OPTIONAL) == 0) {
					return true;
				}
			}
			return false;
		}

		void BaseSystem::AddComponentType(ComponentID id, ComponentFlag flag) {
			VT_CORE_INFO("Base::System set a flag {0} for component {1}", flag, id);
			m_componentTypes.push_back(id);
			m_componentFlags.push_back(flag);
		}

		bool SystemsList::AddSystem(BaseSystem& s) {
			if (!s.IsValid()) {
				return false;
			}

			VT_CORE_INFO("SystemsList::AddSystem system comps size {0} and flags size {1}", s.GetComponentTypes().size(), s.GetComponentFlags().size());
			m_systems.push_back(&s);
			return true;
		}

		bool SystemsList::RemoveSystem(BaseSystem& s) {
			for (uint32 i = 0; i < m_systems.size(); i++) {
				if (&s == m_systems[i]) {
					m_systems.erase(m_systems.begin() + i);
					return true;
				}
			}
			return false;
		}

	}

}
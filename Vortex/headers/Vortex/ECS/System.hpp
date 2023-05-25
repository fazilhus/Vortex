#pragma once
#include "Vortex/ECS/Config.hpp"
#include "Vortex/ECS/Component.hpp"

namespace Vortex {

	namespace ecs {

		enum ComponentFlag {
			NONE = 0,
			FLAG_OPTIONAL = 1
		};

		class BaseSystem {
		private:
			Vector<ComponentID> m_componentTypes;
			Vector<uint32> m_componentFlags;

		public:
			BaseSystem(const Vector<ComponentID>& componentTypes) : m_componentTypes(componentTypes) {}
			virtual ~BaseSystem() = default;

			virtual void OnUpdate(Timestep ts, BaseComponent** components) {}

			const Vector<ComponentID>& GetComponentTypes() const {
				return m_componentTypes;
			}

			const Vector<uint32>& GetComponentFlags() const {
				return m_componentFlags;
			}

			bool IsValid() {
				for (uint32 i = 0; i < m_componentFlags.size(); ++i) {
					if ((m_componentFlags[i] & ComponentFlag::FLAG_OPTIONAL) == 0) {
						return true;
					}
				}
				return false;
			}

		protected:
			void AddComponentType(ComponentID id, ComponentFlag flag = ComponentFlag::NONE) {
				m_componentTypes.push_back(id);
				m_componentFlags.push_back(flag);
			}
		};

		class SystemsList {
		private:
			Vector<BaseSystem*> m_systems;

		public:
			SystemsList() = default;
			SystemsList(const SystemsList& other) = delete;
			SystemsList(SystemsList&&) = delete;
			~SystemsList() = default;

			SystemsList& operator=(const SystemsList& other) = delete;
			SystemsList& operator=(SystemsList&&) = delete;

			bool AddSystem(BaseSystem& s) {
				if (!s.IsValid()) {
					return false;
				}

				m_systems.push_back(&s);
				return true;
			}

			bool RemoveSystem(BaseSystem& s) {
				for (uint32 i = 0; i < m_systems.size(); i++) {
					if (&s == m_systems[i]) {
						m_systems.erase(m_systems.begin() + i);
						return true;
					}
				}
				return false;
			}

			std::size_t Size() const { return m_systems.size(); }
			BaseSystem* operator[](uint32 i) { return m_systems[i]; }
		};

	}// namespace ecs

}// namespace Vortex
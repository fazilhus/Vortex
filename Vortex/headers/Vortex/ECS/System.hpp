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
		protected:
			Vector<ComponentID> m_componentTypes;
			Vector<ComponentFlag> m_componentFlags;

		public:
			BaseSystem() : m_componentTypes(), m_componentFlags() {}
			BaseSystem(const BaseSystem& other) : m_componentTypes(other.GetComponentTypes()), m_componentFlags(other.GetComponentFlags()) {}
			virtual ~BaseSystem() = default;

			//virtual void OnUpdate(Timestep ts, BaseComponent* components) {}
			virtual void OnUpdate(Timestep ts, BaseComponent** components) {}

			const Vector<ComponentID>& GetComponentTypes() const { 
				return m_componentTypes;
			}
			const Vector<ComponentFlag>& GetComponentFlags() const { return m_componentFlags; }

			bool IsValid();

		protected:
			void AddComponentType(ComponentID id, ComponentFlag flag = ComponentFlag::NONE);
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

			bool AddSystem(BaseSystem& s);
			bool RemoveSystem(BaseSystem& s);

			std::size_t Size() const { return m_systems.size(); }
			BaseSystem* operator[](uint32 i) { return m_systems[i]; }
		};

	}// namespace ecs

}// namespace Vortex
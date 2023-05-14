#pragma once
#include "Vortex/ECS/Config.hpp"
#include "Vortex/ECS/Component.hpp"

namespace Vortex {

	namespace ecs {

		class BaseSystem {
		private:
			Vector<ComponentID> m_componentTypes;

		public:
			BaseSystem(const Vector<ComponentID>& componentTypes) : m_componentTypes(componentTypes) {}
			virtual ~BaseSystem() = default;

			virtual void OnUpdate(Timestep ts, BaseComponent** components) {}

			const Vector<ComponentID>& GetComponentTypes() const {
				return m_componentTypes;
			}
		};

	}// namespace ecs

}// namespace Vortex
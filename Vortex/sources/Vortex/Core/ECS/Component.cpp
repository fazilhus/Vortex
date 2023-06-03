#include <vtpch.hpp>
#include "Vortex/ECS/Component.hpp"

namespace Vortex {
	
	namespace ecs {

		Vector<std::tuple<ComponentCreateFunction, ComponentDeleteFunction, size_t>>* BaseComponent::s_componentTypes;

	}

}
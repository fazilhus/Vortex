#pragma once
#include "Vortex/ECS/Config.hpp"

namespace Vortex {

	namespace ecs {

		struct BaseComponent;
		typedef void* EntityHandle;
		typedef uint32 (*ComponentCreateFunction)(Vector<uint8>& memory, EntityHandle entity, BaseComponent* comp);
		typedef void (*ComponentDeleteFunction)(BaseComponent* comp);

		struct BaseComponent {
		public:
			EntityHandle m_entity;

			BaseComponent() : m_entity(nullptr) {
				s_componentTypes = Vector<std::tuple<ComponentCreateFunction, ComponentDeleteFunction, size_t>>{};
			}


			static uint32 RegisterComponentType(ComponentCreateFunction createfn, ComponentDeleteFunction delfn, size_t size) {
				ComponentID id = s_componentTypes.size();
				s_componentTypes.push_back(std::tuple<ComponentCreateFunction, ComponentDeleteFunction, std::size_t>(createfn, delfn, size));
				return id;
			}

			inline static ComponentCreateFunction GetTypeCreateFunction(ComponentID id) {
				return std::get<0>(s_componentTypes[id]);
			}

			inline static ComponentDeleteFunction GetTypeDeleteFunction(ComponentID id) {
				return std::get<1>(s_componentTypes[id]);
			}

			inline static size_t GetTypeSize(ComponentID id) {
				return std::get<2>(s_componentTypes[id]);
			}

			inline static bool IsTypeValid(ComponentID id) {
				return id < s_componentTypes.size();
			}
		private:
			static Vector<std::tuple<ComponentCreateFunction, ComponentDeleteFunction, size_t>> s_componentTypes;
		};

		template<typename T>
		struct Component : public BaseComponent {
			static const ComponentCreateFunction CREATE_FUNCTION;
			static const ComponentDeleteFunction DELETE_FUNCTION;
			static const ComponentID ID;
			static const size_t SIZE;
		};

		template<typename Component>
		ComponentID ComponentCreate(Vector<uint8>& memory, EntityHandle entity, BaseComponent* comp) {
			uint32 index = memory.size();
			memory.resize(index + Component::SIZE);
			Component* component = new(&memory[index])Component(*(Component*)comp);
			component->entity = entity;
			return index;
		}

		template<typename Component>
		void ComponentDelete(BaseComponent* comp) {
			Component* component = (Component*)comp;
			component->~Component();
		}

		template<typename T>
		const uint32 Component<T>::ID(BaseComponent::RegisterComponentType(ComponentCreate<T>, ComponentDelete<T>, sizeof(T)));

		template<typename T>
		const size_t Component<T>::SIZE(sizeof(T));

		template<typename T>
		const ComponentCreateFunction Component<T>::CREATE_FUNCTION(ComponentCreate<T>);

		template<typename T>
		const ComponentDeleteFunction Component<T>::DELETE_FUNCTION(ComponentDelete<T>);

	}// namespace ecs

}// namespace Vortex
#pragma once
#include "Vortex/ECS/Config.hpp"

namespace Vortex {

	namespace ecs {

		static ComponentID compID = 0;

		struct BaseComponent {
		private:
			EntityHandle m_entity;

		public:
			BaseComponent() : m_entity(nullptr) {}
			virtual ~BaseComponent() {}

			static ComponentID GenerateID() {
				return compID++;
			}

			const EntityHandle& GetEntity() const { return m_entity; }
			void SetEntity(const EntityHandle& e) { m_entity = e; }
		};


		template <typename Comp>
		struct Component : public BaseComponent {
		private:
			ComponentID m_id;
			std::size_t m_size;

		public:
			Component() {
				m_id = BaseComponent::GenerateID();
				m_size = sizeof(Comp);
			}
			virtual ~Component() = default;

			static const ComponentID Create(Vector<uint8>& memory, EntityHandle e, BaseComponent* comp) {
				uint32 i = memory.size();
				memory.resize(i + Component::m_size);
				Component* component = new(&memory[i]) Component(*(Component*)comp);
				component->SetEntity(e);
				return i;
			}

			static const void Delete(BaseComponent* comp) {
				Component* component = (Component*)comp;
				component->~Component();
			}

			inline ComponentID GetID() const { return m_id; }
			inline std::size_t GetSize() const { return m_size; }
		};

	}// namespace ecs

}// namespace Vortex
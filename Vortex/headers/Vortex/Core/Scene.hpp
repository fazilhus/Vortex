#pragma once
#include "Vortex/ECS/ECS.hpp"
#include "Vortex/Core/Components.hpp"

namespace Vortex {

	class RenderSystem : public ecs::BaseSystem {
	public:
		RenderSystem(const Vector<ecs::ComponentID>& compTypes) {
			for (const auto item : compTypes) {
				AddComponentType(item, ecs::ComponentFlag::NONE);
			}
			VT_CORE_INFO("RenderSystem compFlags after init {0} {1}", m_componentTypes.size(), m_componentFlags.size());
		}
		virtual ~RenderSystem() = default;

		//virtual void OnUpdate(Timestep ts, ecs::BaseComponent* comps) override {
		//	auto transform = (TransformComponent*)comps;
		//	//auto sprite = (SpriteComponent*)comps[1];
		//	VT_CORE_INFO("RenderSystem::OnUpdate called");
		//	Renderer2D::DrawQuad(transform->Transform, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
		//}
		virtual void OnUpdate(Timestep ts, ecs::BaseComponent** comps) override {
			auto transform = (TransformComponent*)comps[0];
			//auto sprite = (SpriteComponent*)comps[1];
			VT_CORE_INFO("RenderSystem::OnUpdate called");
			Renderer2D::DrawQuad(transform->Transform, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
		}
	};

	class Scene {
	private:
		ecs::Manager m_manager;
		ecs::SystemsList m_systems;

	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep ts);

		template <typename ...Comps>
		ecs::EntityHandle CreateEntity(Comps& ...comps) {
			return m_manager.CreateEntity(comps...);
		}

		ecs::Manager& GetManager() { return m_manager; }
	};

}
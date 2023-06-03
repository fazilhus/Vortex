#include <vtpch.hpp>
#include "Vortex/Core/Scene.hpp"
#include "Vortex/Core/Components.hpp"

namespace Vortex {

	Scene::Scene() {
		
	}

	Scene::~Scene() {

	}

	entt::entity Scene::CreateEntity()
	{
		return m_registry.create();
	}

	void Scene::OnUpdate(Timestep ts) {
		auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
		for (auto entity : group) {
			auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
			Renderer2D::DrawQuad(transform.Transform, sprite.Color);
		}
	}

}
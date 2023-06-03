#include <vtpch.hpp>
#include "Vortex/Scene/Entity.hpp"

namespace Vortex {

	Scene::Scene() {
		
	}

	Scene::~Scene() {

	}

	Entity Scene::CreateEntity(const std::string& tag) {
		Entity entity = { m_registry.create(), this };
		entity.AddComponent<TagComponent>((tag.empty() ? "Entity" : tag));
		entity.AddComponent<TransformComponent>();
		return entity;
	}	

	void Scene::OnUpdate(Timestep ts) {
		auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
		for (auto entity : group) {
			auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
			Renderer2D::DrawQuad(transform.Transform, sprite.Color);
		}
	}

}
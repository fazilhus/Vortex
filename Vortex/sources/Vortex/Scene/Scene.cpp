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
		Scope<Camera> mainCamera = nullptr;
		Scope<glm::mat4> cameraTransform = nullptr;

		auto group = m_registry.view<TransformComponent, CameraComponent>();
		for (auto entity : group) {
			auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

			if (camera.Primary) {
				mainCamera.reset(&camera.Camera);
				cameraTransform.reset(&transform.Transform);
				break;
			}
		}

		if (mainCamera) {
			Renderer2D::BeginScene(*mainCamera, *cameraTransform);

			auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto entity : group) {
				auto [transform, sprite] = m_registry.get<TransformComponent, SpriteComponent>(entity);
				Renderer2D::DrawQuad(transform.Transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}
		mainCamera.release();
		cameraTransform.release();
	}

}
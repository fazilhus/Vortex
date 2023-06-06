#include <vtpch.hpp>
#include "Vortex/Scene/Scene.hpp"
//#include "Vortex/Scene/Entity.hpp"

namespace Vortex {

	Entity Scene::CreateEntity(const std::string& tag) {
		Entity entity{ m_registry.create(), this };
		entity.AddComponent<TagComponent>((tag.empty() ? "Entity" : tag));
		entity.AddComponent<TransformComponent>();
		return entity;
	}	

	void Scene::OnUpdate(Timestep ts) {

		m_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
			if (!nsc.instance) {
				nsc.instance = nsc.InstantiateFunction();
				nsc.instance->m_entity = Entity{ entity, this };
				nsc.instance->OnCreate();
			}

			nsc.instance->OnUpdate(ts);
		});


		Scope<Camera> mainCamera = nullptr;
		Scope<glm::mat4> cameraTransform = nullptr;

		auto view = m_registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view) {
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

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
				VT_CORE_TRACE("Scene::OnUpdate drawquad call pos {0} {1} {2} {3}", transform.Transform[3][0], transform.Transform[3][1], transform.Transform[3][2], transform.Transform[3][3]);
				Renderer2D::DrawQuad(transform.Transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}
		mainCamera.release();
		cameraTransform.release();
	}

	void Scene::OnViewportResize(uint32 width, uint32 height) {
		m_viewportWidth = width;
		m_viewportHeight = height;

		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComp = view.get<CameraComponent>(entity);
			if (!cameraComp.FixedAspectRatio) {
				cameraComp.Camera.SetViewportSize(width, height);
			}
		}
	}

}
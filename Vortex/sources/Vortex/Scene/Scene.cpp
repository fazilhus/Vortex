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

	void Scene::DestroyEntity(Entity entity) {
		m_registry.destroy(entity);
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
		glm::mat4 cameraTransform;

		auto view = m_registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view) {
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

			if (camera.Primary) {
				mainCamera.reset(&camera.Camera);
				cameraTransform = transform.GetTransform();
				break;
			}
		}

		if (mainCamera) {
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto entity : group) {
				auto [transform, sprite] = m_registry.get<TransformComponent, SpriteComponent>(entity);
				VT_CORE_TRACE("Scene::OnUpdate drawquad call pos {0} {1} {2}", transform.Translation.x, transform.Translation.y, transform.Translation.z);
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (int)entity);
			}

			Renderer2D::EndScene();
		}
		mainCamera.release();
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera) {
		Renderer2D::BeginScene(camera);

		auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		Renderer2D::EndScene();
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

	Entity Scene::GetPrimaryCamera() {
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view) {
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary) {
				return Entity{ entity, this };
			}
		}
		return {entt::null, nullptr};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
		//static_assert(false);
	}

	template <>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {

	}

	template <>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {

	}

	template <>
	void Scene::OnComponentAdded<SpriteComponent>(Entity entity, SpriteComponent& component) {

	}

	template <>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
		if (m_viewportWidth > 0 && m_viewportHeight > 0) {
			component.Camera.SetViewportSize(m_viewportWidth, m_viewportHeight);
		}
	}

	template <>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {

	}

}

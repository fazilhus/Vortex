#include <vtpch.hpp>
#include "Vortex/Scene/Scene.hpp"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

namespace Vortex {

	static b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType) {
		switch (bodyType) {
		case Rigidbody2DComponent::BodyType::Static:    return b2_staticBody;
		case Rigidbody2DComponent::BodyType::Dynamic:   return b2_dynamicBody;
		case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		default:
			VT_CORE_ASSERT(false, "Unknown body type");
			return b2_staticBody;
		}
	}

	Scene::~Scene() {
		delete m_physicsWorld;
	}

	Ref<Scene> Scene::Copy(Ref<Scene> other) {
		auto newScene = CreateRef<Scene>();
		newScene->m_viewportWidth = other->m_viewportWidth;
		newScene->m_viewportHeight = other->m_viewportHeight;

		auto& srcRegistry = other->m_registry;
		auto& dstRegistry = newScene->m_registry;
		HashMap<UUID, entt::entity> enttMap;
		auto idview = srcRegistry.view<IDComponent>();
		for (auto e : idview) {
			UUID id = srcRegistry.get<IDComponent>(e).ID;
			const auto& name = srcRegistry.get<TagComponent>(e).Tag;
			auto newEntity = newScene->CreateEntityWithUUID(id, name);
			enttMap[id] = static_cast<entt::entity>(newEntity);
		}
		CopyComponent(AllComponents{}, dstRegistry, srcRegistry, enttMap);

		return newScene;
	}

	Entity Scene::CreateEntity(const std::string& tag) {
		return CreateEntityWithUUID(UUID(), tag);
	}

	Entity Scene::CreateEntityWithUUID(UUID id, const std::string& tag) {
		Entity entity{ m_registry.create(), this };
		entity.AddComponent<IDComponent>(id);
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

		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			m_physicsWorld->Step(ts, velocityIterations, positionIterations);

			// Retrieve transform from Box2D
			auto view = m_registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				const auto& position = body->GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = body->GetAngle();
			}
		}


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

			{
				auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
				for (auto entity : group) {
					auto [transform, sprite] = m_registry.get<TransformComponent, SpriteComponent>(entity);
					VT_CORE_TRACE("Scene::OnUpdate drawquad call pos {0} {1} {2}", transform.Translation.x, transform.Translation.y, transform.Translation.z);
					Renderer2D::DrawSprite(transform.GetTransform(), sprite, static_cast<int>(entity));
				}
			}

			{
				auto view = m_registry.view<TransformComponent, CircleRendererComponent>();
				for (auto entity : view) {
					auto [transform, circle] = m_registry.get<TransformComponent, CircleRendererComponent>(entity);
					VT_CORE_TRACE("Scene::OnUpdate drawcircle call pos {0} {1} {2}", transform.Translation.x, transform.Translation.y, transform.Translation.z);
					Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, static_cast<int>(entity));
				}
			}

			Renderer2D::EndScene();
		}
		mainCamera.release();
	}

	void Scene::OnUpdateSimulation(Timestep ts, EditorCamera& camera) {
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		m_physicsWorld->Step(ts, velocityIterations, positionIterations);

		// Retrieve transform from Box2D
		auto view = m_registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			const auto& position = body->GetPosition();
			transform.Translation.x = position.x;
			transform.Translation.y = position.y;
			transform.Rotation.z = body->GetAngle();
		}

		RenderScene(camera);
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera) {
		RenderScene(camera);
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

	void Scene::OnRuntimStart() {
		OnPhysics2DStart();
	}

	void Scene::OnRuntimeResume() {
		OnPhysics2DResume();
	}

	void Scene::OnRuntimePause() {
		OnPhysics2DPause();
	}

	void Scene::OnRuntimeStop() {
		OnPhysics2DStop();
	}

	void Scene::OnSimulateStart() {
		OnPhysics2DStart();
	}

	void Scene::OnSimulateResume() {
		OnPhysics2DResume();
	}

	void Scene::OnSimulatePause() {
		OnPhysics2DPause();
	}

	void Scene::OnSimulateStop() {
		OnPhysics2DStop();
	}

	void Scene::DuplicateEntity(Entity entity) {
		auto name = entity.GetName();
		auto newEntity = CreateEntity(name);

		CopyComponentIfExists(AllComponents{}, newEntity, entity);
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

	void Scene::OnPhysics2DStart() {
		m_physicsWorld = new b2World({ 0.0f, -9.8f });

		auto view = m_registry.view<Rigidbody2DComponent>();
		for (auto e : view) {
			Entity entity{ e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			b2Body* body = m_physicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}

			if (entity.HasComponent<CircleCollider2DComponent>()) {
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
				circleShape.m_radius = transform.Scale.x * cc2d.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnPhysics2DResume() {
		m_physicsWorld->SetAllowSleeping(false);
	}

	void Scene::OnPhysics2DPause() {
		m_physicsWorld->SetAllowSleeping(true);

		auto bodies = m_physicsWorld->GetBodyList();
		for (b2Body* b = bodies; b; b = b->GetNext()) {
			b->SetAwake(false);
		}
	}

	void Scene::OnPhysics2DStop() {
		delete m_physicsWorld;
		m_physicsWorld = nullptr;
	}

	void Scene::RenderScene(EditorCamera& camera) {
		Renderer2D::BeginScene(camera);

		// Draw sprites
		{
			auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto entity : group) {
				auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

				Renderer2D::DrawSprite(transform.GetTransform(), sprite, static_cast<int>(entity));
			}
		}

		// Draw circles
		{
			auto view = m_registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : view) {
				auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

				Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, static_cast<int>(entity));
			}
		}

		Renderer2D::EndScene();
	}

	template<typename ...T>
	void Scene::CopyComponent(entt::registry& dst, entt::registry& src, const HashMap<UUID, entt::entity> enttMap) {
		([&]() {
			auto view = src.view<T>();
			for (auto e : view) {
				UUID id = src.get<IDComponent>(e).ID;
				VT_CORE_ASSERT(enttMap.contains(id), "Entity {0} does not exist", id);
				entt::entity dstEntt = enttMap.at(id);
				auto& component = src.get<T>(e);
				dst.emplace_or_replace<T>(dstEntt, component);
			}
		}(), ...);
	}

	template<typename ...T>
	void Scene::CopyComponent(ComponentGroup<T...>, entt::registry& dst, entt::registry& src, const HashMap<UUID, entt::entity> enttMap) {
		CopyComponent<T...>(dst, src, enttMap);
	}

	template<typename ...T>
	void Scene::CopyComponentIfExists(Entity dst, Entity src) {
		([&]() {
			if (src.HasComponent<T>()) {
				dst.AddOrReplaceComponent<T>(src.GetComponent<T>());
			}
		}(), ...);
	}

	template<typename ...T>
	void Scene::CopyComponentIfExists(ComponentGroup<T...>, Entity dst, Entity src) {
		CopyComponentIfExists<T...>(dst, src);
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
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component) {

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

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component) {

	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component) {

	}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component) {

	}

}

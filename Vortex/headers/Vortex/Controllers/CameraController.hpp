#pragma once
#include "Vortex/Core/Timestep.hpp"
#include "Vortex/Core/Input.hpp"
#include "Vortex/Scene/ScriptableEntity.hpp"
#include "Vortex/Scene/Components.hpp"

namespace Vortex {

	class CameraController : public ScriptableEntity {
	public:
		virtual void OnCreate() override {
			auto& transform = GetComponent<TransformComponent>().Transform;
			transform[3][0] = rand() % 10 - 5.0f;
		}
		virtual void OnDestroy() override {}
		virtual void OnUpdate(Timestep ts) override {
			auto& transform = GetComponent<TransformComponent>().Transform;
			float speed = 5.0f;

			if (Input::IsKeyPressed(KeyCode::A)) transform[3][0] += speed * ts;
			if (Input::IsKeyPressed(KeyCode::D)) transform[3][0] -= speed * ts;
			if (Input::IsKeyPressed(KeyCode::W)) transform[3][1] -= speed * ts;
			if (Input::IsKeyPressed(KeyCode::S)) transform[3][1] += speed * ts;
		}
	};

}
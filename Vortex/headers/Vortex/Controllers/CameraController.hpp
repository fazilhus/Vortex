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

			if (Input::IsKeyPressed(Key::A) || Input::IsKeyPressed(Key::Left)) transform[3][0] += speed * ts;
			if (Input::IsKeyPressed(Key::D) || Input::IsKeyPressed(Key::Right)) transform[3][0] -= speed * ts;
			if (Input::IsKeyPressed(Key::W) || Input::IsKeyPressed(Key::Up)) transform[3][1] -= speed * ts;
			if (Input::IsKeyPressed(Key::S) || Input::IsKeyPressed(Key::Down)) transform[3][1] += speed * ts;
		}
	};

}
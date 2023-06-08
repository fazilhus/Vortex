#pragma once
#include "Vortex/Core/Timestep.hpp"
#include "Vortex/Core/Input.hpp"
#include "Vortex/Scene/ScriptableEntity.hpp"
#include "Vortex/Scene/Components.hpp"

namespace Vortex {

	class CameraController : public ScriptableEntity {
	public:
		virtual void OnCreate() override {
			auto& translation = GetComponent<TransformComponent>().Translation;
			translation.x = rand() % 10 - 5.0f;
		}
		virtual void OnDestroy() override {}
		virtual void OnUpdate(Timestep ts) override {
			auto& translation = GetComponent<TransformComponent>().Translation;
			float speed = 5.0f;

			if (Input::IsKeyPressed(Key::A) || Input::IsKeyPressed(Key::Left)) translation.x += speed * ts;
			if (Input::IsKeyPressed(Key::D) || Input::IsKeyPressed(Key::Right)) translation.x -= speed * ts;
			if (Input::IsKeyPressed(Key::W) || Input::IsKeyPressed(Key::Up)) translation.y -= speed * ts;
			if (Input::IsKeyPressed(Key::S) || Input::IsKeyPressed(Key::Down)) translation.y += speed * ts;
		}
	};

}
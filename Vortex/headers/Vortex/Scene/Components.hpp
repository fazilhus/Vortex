#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Vortex/Scene/SceneCamera.hpp"
#include "Vortex/Scene/ScriptableEntity.hpp"

namespace Vortex {

	struct TagComponent {
		std::string Tag;

		TagComponent() = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
		TagComponent(const TagComponent&) = default;
	};

	struct TransformComponent {
		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const glm::vec3& translation) : Translation(translation) {}
		TransformComponent(const TransformComponent&) = default;

		glm::mat4 GetTransform() const {
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, {1, 0, 0}) *
				glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 }) *
				glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), Translation) *
				rotation *
				glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteComponent {
		glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};

		SpriteComponent() = default;
		SpriteComponent(const glm::vec4& color) : Color(color) {}
		SpriteComponent(const SpriteComponent&) = default;
	};

	struct CameraComponent {
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent {
		ScriptableEntity* instance = nullptr;

		ScriptableEntity* (*InstantiateFunction)();
		void (*DestroyInstanceFunction)(NativeScriptComponent*);

		template <typename T>
		void Bind() {
			InstantiateFunction = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyInstanceFunction = [](NativeScriptComponent* nsc) { 
				delete nsc->instance;
				nsc->instance = nullptr;
			};
		}
	};

}
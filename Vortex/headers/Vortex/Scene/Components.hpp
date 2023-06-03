#pragma once
#include <glm/glm.hpp>
#include "Vortex/Renderer/Cameras/Camera.hpp"

namespace Vortex {

	struct TagComponent {
		std::string Tag;

		TagComponent() = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
		TagComponent(const TagComponent&) = default;
	};

	struct TransformComponent {
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const glm::mat4& transform) : Transform(transform) {}
		TransformComponent(const TransformComponent&) = default;
	};

	struct SpriteComponent {
		glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};

		SpriteComponent() = default;
		SpriteComponent(const glm::vec4& color) : Color(color) {}
		SpriteComponent(const SpriteComponent&) = default;
	};

	struct CameraComponent {
		Camera Camera;
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const glm::mat4 proj, bool primary) : Camera(proj), Primary(primary) {}
		CameraComponent(const CameraComponent&) = default;
	};

}
#pragma once
#include <vtpch.hpp>
#include "Vortex/ECS/Config.hpp"
#include "Vortex/ECS/Component.hpp"

namespace Vortex {

	struct TagComponent : public ecs::Component<TagComponent> {
		std::string Tag;

		TagComponent() = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
		TagComponent(const TagComponent&) = default;
	};

	struct TransformComponent : public ecs::Component<TransformComponent> {
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const glm::mat4& transform) : Transform(transform) {}
		TransformComponent(const TransformComponent&) = default;
	};



	struct SpriteComponent : public ecs::Component<SpriteComponent> {
		glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};

		SpriteComponent() = default;
		SpriteComponent(const glm::vec4& color) : Color(color) {}
		SpriteComponent(const SpriteComponent&) = default;
	};

}
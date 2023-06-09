#pragma once
#include "Vortex/Scene/Scene.hpp"

namespace Vortex {

	class SceneSerializer {
	private:
		Ref<Scene> m_scene;

	public:
		SceneSerializer(const Ref<Scene>& scene) : m_scene(scene) {}

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);

	private:
		void SerializeEntity(YAML::Emitter& out, Entity entity);
	};

}
#include <vtpch.hpp>
#include "Vortex/Core/Scene.hpp"
#include "Vortex/Core/Components.hpp"

namespace Vortex {

	Scene::Scene() {
		RenderSystem render{ {TransformComponent::ID} };
		VT_CORE_INFO("Scene comp types and flags {0} {1}", render.GetComponentTypes().size(), render.GetComponentFlags().size());
		if (!m_systems.AddSystem(render)) {
			VT_CORE_WARN("Scene::Scene could not add render system");
		}
		VT_CORE_INFO("Scene amount of systems {0}", m_systems.Size());
	}

	Scene::~Scene() {

	}

	void Scene::OnUpdate(Timestep ts) {
		m_manager.OnUpdate(ts);
		//m_manager.UpdateSystems(ts, m_systems);
	}

}
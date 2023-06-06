#pragma once
#include "Vortex/Scene/Entity.hpp"

namespace Vortex {

	class Scene;

	class ScriptableEntity {
	private:
		Entity m_entity;
		friend class Scene;

	public:
		//ScriptableEntity() = default;
		virtual ~ScriptableEntity() = default;

		template <typename T>
		T& GetComponent() { return m_entity.GetComponent<T>(); }

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
	};

}
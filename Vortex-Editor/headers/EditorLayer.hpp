#pragma once

#include "Vortex.hpp"

struct Transform {
	float x, y, z;

	void SetTranslation(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct TransformComponent : public Vortex::ecs::Component<TransformComponent> {
	Transform transform;
};

class EditorLayer : public Vortex::Layer {
private:
	Vortex::OrthoCameraController m_cameraController;

	Vortex::Ref<Vortex::VertexArray> m_vao;
	Vortex::Ref<Vortex::Shader> m_shader;
	Vortex::Ref<Vortex::Texture2D> m_texture1;
	Vortex::Ref<Vortex::Texture2D> m_texture2;
	Vortex::Ref<Vortex::FrameBuffer> m_frameBuffer;

	glm::vec2 m_viewportPanelSize;

	bool m_viewportFocused, m_viewportHovered;

	Vortex::ecs::Manager m_ecs;
	TransformComponent m_trcomp;
	Vortex::ecs::EntityHandle m_entity;


public:
	EditorLayer();
	virtual ~EditorLayer() override = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Vortex::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Vortex::Event& e) override;
};
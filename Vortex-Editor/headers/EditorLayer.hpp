#pragma once

#include "Vortex.hpp"
#include "Vortex/ECS/Component.hpp"

struct PositionComponent : public Vortex::ecs::Component<PositionComponent> {
	float x, y, z;

	PositionComponent(float x, float y, float z) : x(x), y(y), z(z) {}

	PositionComponent& operator=(const PositionComponent& other) {
		x = other.x;
		y = other.y;
		z = other.z;
	}
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

	PositionComponent m_comp;

public:
	EditorLayer();
	virtual ~EditorLayer() override = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Vortex::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Vortex::Event& e) override;
};
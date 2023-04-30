#pragma once

#include "Vortex.hpp"

class Sandbox2D : public Vortex::Layer {
private:
	Vortex::OrthoCameraController m_cameraController;

	Vortex::Ref<Vortex::VertexArray> m_vao;
	Vortex::Ref<Vortex::Shader> m_shader;
	Vortex::Ref<Vortex::Texture2D> m_texture;

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Vortex::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Vortex::Event& e) override;
};
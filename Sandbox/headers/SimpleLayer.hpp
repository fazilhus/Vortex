#pragma once
#include "Vortex/Core/EntryPoint.hpp"

#include "Platforms/OpenGL/OpenGLShader.hpp"

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.hpp"


class SimpleLayer : public Vortex::Layer {
private:
	Vortex::ShaderLib m_shaderLib;

	Vortex::Ref<Vortex::VertexArray> m_vao1;
	Vortex::Ref<Vortex::VertexArray> m_vao2;

	Vortex::Ref<Vortex::Texture2D> m_tex;

	Vortex::OrthoCameraController m_cameraController;

	glm::vec3 camPos{ 0.0f, 0.0f, 0.0f };
	float camPosVel = 1.0f;
	float camRot{ 0.0f };
	float camRotVel = 180.0f;

public:
	SimpleLayer();
	virtual ~SimpleLayer() = default;

	virtual void OnAttach() override;

	virtual void OnDetach() override;

	virtual void OnUpdate(Vortex::Timestep ts) override;

	virtual void OnImGuiRender() override;

	virtual void OnEvent(Vortex::Event& e) override;
};
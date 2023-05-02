#include "SimpleLayer.hpp"

SimpleLayer::SimpleLayer() : Layer("Simple"), m_cameraController(1600.0f / 900.0f, true) {}

void SimpleLayer::OnAttach() {
	Layer::OnAttach();

	{
		m_vao2 = Vortex::VertexArray::Create();

		float vertices[4 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,	0.0f, 0.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Vortex::VertexBuffer> vbo;
		vbo = Vortex::VertexBuffer::Create(vertices, sizeof(vertices));
		Vortex::BufferLayout layout{
			{"a_position", Vortex::ShaderDataType::Float3},
			{"a_color", Vortex::ShaderDataType::Float4}
		};
		vbo->SetLayout(layout);
		m_vao2->AddVertexBuffer(vbo);

		uint4 indices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Vortex::IndexBuffer> ibo;
		ibo = Vortex::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint4));
		m_vao2->AddIndexBuffer(ibo);

		auto shader = m_shaderLib.Load("res/shaders/boxShader.glsl");
		VT_CL_TRACE("box shader created");
	}

	{
		m_vao1 = Vortex::VertexArray::Create();

		float vertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Vortex::VertexBuffer> vbo;
		vbo = Vortex::VertexBuffer::Create(vertices, sizeof(vertices));
		Vortex::BufferLayout layout{
			{"a_position", Vortex::ShaderDataType::Float3},
			{"a_texPos", Vortex::ShaderDataType::Float2}
		};
		vbo->SetLayout(layout);
		m_vao1->AddVertexBuffer(vbo);

		uint4 indices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Vortex::IndexBuffer> ibo;
		ibo = Vortex::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint4));
		m_vao1->AddIndexBuffer(ibo);

		auto texShader = m_shaderLib.Load("res/shaders/textureShader.glsl");
		VT_CL_TRACE("texture shader created");

		m_tex = Vortex::Texture2D::Create("res/textures/img3.png");

		texShader->Bind();
		texShader->SetInt("u_texture", 0);
	}
}

void SimpleLayer::OnDetach() {
	Layer::OnDetach();
}

void SimpleLayer::OnUpdate(Vortex::Timestep ts) {
	Layer::OnUpdate(ts);

	m_cameraController.OnUpdate(ts);

	Vortex::Render::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Vortex::Render::Clear();

	Vortex::Renderer::BeginScene(m_cameraController.GetCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.15f));

	auto shader = m_shaderLib.Get("boxShader");
	auto textureShader = m_shaderLib.Get("textureShader");

	for (int y = -4; y < 5; ++y) {
		for (int x = -4; x < 5; ++x) {
			glm::vec3 pos(x * 0.16f, y * 0.16f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Vortex::Renderer::Submit(shader, m_vao2, transform);
		}
	}

	m_tex->Bind(0);
	Vortex::Renderer::Submit(textureShader, m_vao1);

	Vortex::Renderer::EndScene();
}

void SimpleLayer::OnImGuiRender() {
	Layer::OnImGuiRender();

	ImGui::DragFloat3("Pos", &camPos.x, 0.005f, -1.0f, 1.0f);
	ImGui::SameLine();
	if (ImGui::Button("Reset pos")) { camPos = glm::vec3(0.0f); }
	ImGui::DragFloat("Rot", &camRot, 0.1f, -90.0f, 90.0f);
	ImGui::SameLine();
	if (ImGui::Button("Reset rot")) { camRot = 0.0f; }
}

void SimpleLayer::OnEvent(Vortex::Event& e) {
	Layer::OnEvent(e);

	m_cameraController.OnEvent(e);
}

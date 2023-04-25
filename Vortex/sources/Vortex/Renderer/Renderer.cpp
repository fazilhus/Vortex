#include "vtpch.hpp"
#include "Vortex/Renderer/Renderer.hpp"


namespace Vortex {

	void Renderer::BeginScene() {
	}

	void Renderer::EndScene() {
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& va) {
		va->Bind();
		Render::DrawIndexed(va);
	}

}
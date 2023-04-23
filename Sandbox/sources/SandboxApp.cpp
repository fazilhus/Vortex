#include <Vortex.hpp>

#include "Vortex/Layers/ImGuiLayer.hpp"

class SimpleLayer : public Vortex::Layer {
public:
	SimpleLayer()
		: Layer("Simple") {};
	~SimpleLayer() {};

	void OnAttach() override {
		VT_CL_TRACE("Layer {0} attached", GetName());
	}

	void OnDetach() override {
		VT_CL_TRACE("Layer {0} detached", GetName());
	}

	void OnUpdate() override {
		VT_CL_TRACE("{0} layer update", GetName());
	}

	void OnEvent(Vortex::Event& e) override {
		VT_CL_TRACE("Event {0}", e);
	}
};

class Sandbox : public Vortex::Application {
public:
	Sandbox() {
		PushLayer(new SimpleLayer());
		PushOverlay(new Vortex::ImGuiLayer());
	}
	~Sandbox() {}
};

Vortex::Application* Vortex::CreateApplication() {
	return new Sandbox();
}
#include <Vortex.hpp>

class SimpleLayer : public Vortex::Layer {
public:
	SimpleLayer()
		: Layer("Simple") {};
	~SimpleLayer() {};

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
	}
	~Sandbox() {}
};

Vortex::Application* Vortex::CreateApplication() {
	return new Sandbox();
}
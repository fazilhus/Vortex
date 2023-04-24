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

		if (Vortex::Input::IsKeyPressed(VT_KEY_TAB)) {
			VT_CL_TRACE("Tab is pressed (poll)");
		}
	}

	void OnEvent(Vortex::Event& e) override {
		VT_CL_TRACE("Event {0}", e);

		if (e.GetEventType() == Vortex::EventType::KeyPressed) {
			Vortex::KeyPressedEvent& e1 = (Vortex::KeyPressedEvent&)e;
			if (e1.GetKeyCode() == VT_KEY_TAB) {
				VT_CL_TRACE("Tab is pressed (event)");
			}
			VT_CL_TRACE("{0}", (char)e1.GetKeyCode());
		}
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
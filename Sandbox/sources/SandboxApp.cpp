#include <iostream>

#include <Vortex.hpp>

class Sandbox : public Vortex::Application {
public:
	Sandbox() {}
	~Sandbox() {}
};

Vortex::Application* Vortex::CreateApplication() {
	return new Sandbox();
}
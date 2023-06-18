#include <Vortex.hpp>
#include "Vortex/Core/EntryPoint.hpp"
#include "Vortex/Core/Application.hpp"

#include "Sandbox2D.hpp"

class Sandbox : public Vortex::Application {
public:
	Sandbox(const Vortex::AppSpec& specification)
		: Vortex::Application(specification) {
		PushLayer(Vortex::CreateRef<Sandbox2D>());
	}

	virtual ~Sandbox() = default;
};

Vortex::Application* Vortex::CreateApplication(Vortex::AppCommandlineArgs args) {
	AppSpec spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../Vortex-Editor";
	spec.CommandlineArgs = args;

	return new Sandbox(spec);
}

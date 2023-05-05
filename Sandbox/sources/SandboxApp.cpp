#include <Vortex.hpp>

#include "Sandbox2D.hpp"
#include "SimpleLayer.hpp"


class Sandbox : public Vortex::Application {
public:
	Sandbox() {
		//Vortex::Ref<Vortex::Layer> sl = Vortex::CreateRef<SimpleLayer>();
		//PushLayer(sl);
		Vortex::Ref<Vortex::Layer> sl2d = Vortex::CreateRef<Sandbox2D>();
		PushLayer(sl2d);
	}
	~Sandbox() {}
};

//Vortex::Application* Vortex::CreateApplication() {
//	return new Sandbox();
//}
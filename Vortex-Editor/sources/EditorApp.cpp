#include <Vortex.hpp>

#include "EditorLayer.hpp"

#include "Vortex/Core/EntryPoint.hpp"

namespace Vortex {

	class EditorApp : public Application {
	public:
		EditorApp() {
			//Ref<Layer> sl = CreateRef<SimpleLayer>();
			//PushLayer(sl);
			Ref<Layer> sl2d = CreateRef<EditorLayer>();
			PushLayer(sl2d);
		}
		virtual ~EditorApp() override = default;
	};

	Application* CreateApplication() {
		return new EditorApp();
	}

}
#include <Vortex.hpp>

#include "EditorLayer.hpp"

#include "Vortex/Core/EntryPoint.hpp"

namespace Vortex {

	class EditorApp : public Application {
	public:
		EditorApp(AppCommandlineArgs args) : Application("Vortex Editor", args) {
			Ref<Layer> sl2d = CreateRef<EditorLayer>();
			PushLayer(sl2d);
		}
		virtual ~EditorApp() override = default;
	};

	Application* CreateApplication(AppCommandlineArgs args) {
		return new EditorApp(args);
	}

}

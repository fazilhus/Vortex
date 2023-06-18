#include <Vortex.hpp>

#include "EditorLayer.hpp"

#include "Vortex/Core/EntryPoint.hpp"

namespace Vortex {

	class EditorApp : public Application {
	public:
		EditorApp(AppSpec spec) : Application(spec) {
			Ref<Layer> sl2d = CreateRef<EditorLayer>();
			PushLayer(sl2d);
		}
		virtual ~EditorApp() override = default;
	};

	Application* CreateApplication(AppCommandlineArgs args) {
		AppSpec spec;
		spec.Name = "Vortex-Editor";
		spec.WorkingDirectory = "../Vortex-Editor";
		spec.CommandlineArgs = args;
		return new EditorApp(spec);
	}

}

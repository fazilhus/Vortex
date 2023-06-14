#pragma once
#include "Vortex/Core/Application.hpp"

#ifdef VT_PLATFORM_WIN

extern Vortex::Application* Vortex::CreateApplication(Vortex::AppCommandlineArgs args);

int main(int argc, char** argv) {
	Vortex::Log::Init();

	auto app = Vortex::CreateApplication({ argc, argv });

	VT_PROFILE_BEGIN_SESSION("App", "VortexProfile-App.json");
	app->Run();
	VT_PROFILE_END_SESSION();

	delete app;
	return 0;
}

#endif

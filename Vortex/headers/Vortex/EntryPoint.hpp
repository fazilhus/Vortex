#pragma once

#ifdef VT_PLATFORM_WIN

extern Vortex::Application* Vortex::CreateApplication();

int main(int argc, char** argv) {
	auto app = Vortex::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif
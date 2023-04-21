#pragma once

#ifdef VT_PLATFORM_WIN

extern Vortex::Application* Vortex::CreateApplication();

int main(int argc, char** argv) {
	Vortex::Log::Init();
	int a = 10;
	VT_INFO("Var: {0}", a);

	auto app = Vortex::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif
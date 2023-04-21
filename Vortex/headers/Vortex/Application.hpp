#pragma once

#include "Core.hpp"

namespace Vortex {

	class VERTEX_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in the CLIENT
	Application* CreateApplication();

}
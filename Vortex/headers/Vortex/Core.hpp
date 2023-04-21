#pragma once

#ifdef VT_PLATFORM_WIN
	#ifdef VT_BUILD_DLL
		#define VERTEX_API __declspec(dllexport)
	#else
		#define VERTEX_API __declspec(dllimport)
	#endif
#else
	#error Vortex only supports Win
#endif
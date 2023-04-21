#pragma once

#ifdef VT_PLATFORM_WIN
	#ifdef VT_BUILD_DLL
		#define VORTEX_API __declspec(dllexport)
	#else
		#define VORTEX_API __declspec(dllimport)
	#endif
#else
	#error Vortex only supports Win
#endif
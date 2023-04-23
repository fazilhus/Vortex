#pragma once

#ifdef VT_PLATFORM_WIN
	#ifdef VT_BUILD_DLL
		#define VORTEX_API __declspec(dllexport)
	#else
		#define VORTEX_API __declspec(dllimport)
	#endif

	#ifdef VT_ENABLE_ASSERTS
		#define VT_CORE_ASSERT(x, ...) { if(!(x)) { VT_CORE_ERROR("Assertion failed {0}", __VA_ARGS__); __debugbreak(); } }
		#define VT_CL_ASSERT(x, ...) { if(!(x)) { VT_CL_ERROR("Assertion failed {0}", __VA_ARGS__); __debugbreak(); } }
	#else
		#define VT_CORE_ASSERT(x, ...)
		#define VT_CL_ASSERT(x, ...)
	#endif
#else
	#error Vortex only supports Win
#endif


#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

using uint = unsigned int;
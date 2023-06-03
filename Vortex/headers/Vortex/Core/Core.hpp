#pragma once

#include <memory>

#ifdef _WIN32
/* Windows x64/x86 */
	#ifdef _WIN64
			/* Windows x64  */
		#define VT_PLATFORM_WIN
	#else
			/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif

#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define VT_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define VT_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#define VORTEX_API __declspec(dllimport)
		#error "Unknown Apple platform!"
	#endif
	 /* We also have to check __ANDROID__ before __linux__
	  * since android is based on the linux kernel
	  * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define VT_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define VT_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#define VORTEX_API
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif

// DLL support
#ifdef VT_PLATFORM_WIN
	#ifdef VT_DYNAMIC_LINK
		#ifdef VT_BUILD_DLL
			#define VORTEX_API __declspec(dllexport)
		#else
			#define VORTEX_API __declspec(dllimport)
		#endif
	#else
		#define VORTEX_API
	#endif
#else
	#error Vortex only supports Windows!
#endif

#ifdef VT_PLATFORM_WIN
	#define VT_DEBUGBREAK() __debugbreak()
#else
	#define VT_DEBUGBREAK()
	#error Vortex only supports Win
#endif

#ifdef VT_ENABLE_ASSERTS
	#define VT_CORE_ASSERT(x, ...) { if(!(x)) { VT_CORE_ERROR("Assertion failed {0}", __VA_ARGS__); VT_DEBUGBREAK(); } }
	#define VT_CL_ASSERT(x, ...) { if(!(x)) { VT_CL_ERROR("Assertion failed {0}", __VA_ARGS__); VT_DEBUGBREAK(); } }
#else
	#define VT_CORE_ASSERT(x, ...)
	#define VT_CL_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define VT_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

using uint4 = unsigned int;
using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;

using int4 = int;
using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;

namespace Vortex {

	template <typename T>
	using Scope = std::unique_ptr<T>;
	template <typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}
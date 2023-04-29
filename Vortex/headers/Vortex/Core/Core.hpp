#pragma once

#include <memory>

#ifdef VT_PLATFORM_WIN
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

#define VT_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

using uint = unsigned int;

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
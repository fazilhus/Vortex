#pragma once

#include "vtpch.hpp"

#include "Core.hpp"

namespace Vortex {

	class VORTEX_API Log {
	private:
		static std::shared_ptr<spdlog::logger> s_clientLogger;
		static std::shared_ptr<spdlog::logger> s_coreLogger;

	public:
		static void Init();

		inline static std::shared_ptr <spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr <spdlog::logger>& GetClientLogger() { return s_clientLogger; }
	};

}

#ifdef VT_DEBUG
	// Core Log macros
	#define VT_CORE_CRITICAL(...) ::Vortex::Log::GetCoreLogger()->critical(__VA_ARGS__)
	#define VT_CORE_ERROR(...)    ::Vortex::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define VT_CORE_WARN(...)     ::Vortex::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define VT_CORE_INFO(...)     ::Vortex::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define VT_CORE_DEBUG(...)    ::Vortex::Log::GetCoreLogger()->debug(__VA_ARGS__)
	#define VT_CORE_TRACE(...)    ::Vortex::Log::GetCoreLogger()->trace(__VA_ARGS__)

	// Client Log macros
	#define VT_CL_CRITICAL(...)      ::Vortex::Log::GetClientLogger()->critical(__VA_ARGS__)
	#define VT_CL_ERROR(...)         ::Vortex::Log::GetClientLogger()->error(__VA_ARGS__)
	#define VT_CL_WARN(...)          ::Vortex::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define VT_CL_INFO(...)          ::Vortex::Log::GetClientLogger()->info(__VA_ARGS__)
	#define VT_CL_DEBUG(...)         ::Vortex::Log::GetClientLogger()->debug(__VA_ARGS__)
	#define VT_CL_TRACE(...)         ::Vortex::Log::GetClientLogger()->trace(__VA_ARGS__)

#else
// Disabled logs If distribution build
	#define VT_CORE_CRITICAL(...)
	#define VT_CORE_ERROR(...)
	#define VT_CORE_WARN(...)
	#define VT_CORE_INFO(...)
	#define VT_CORE_DEBUG(...)
	#define VT_CORE_TRACE(...)

	#define VT_CL_CRITICAL(...)
	#define VT_CL_ERROR(...)
	#define VT_CL_WARN(...)
	#define VT_CL_INFO(...)
	#define VT_CL_DEBUG(...)
	#define VT_CL_TRACE(...)
#endif
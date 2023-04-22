#include "vtpch.hpp"

#include "Vortex/Log.hpp"

namespace Vortex {

	std::shared_ptr<spdlog::logger> Log::s_coreLogger;
	std::shared_ptr<spdlog::logger> Log::s_clientLogger;

	void Log::Init() {
		// Log pattern
		spdlog::set_pattern("%^[%T %e] %n-> %v%$");

		s_coreLogger = spdlog::stdout_color_mt("VORTEX");
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stdout_color_mt("APP");
		s_clientLogger->set_level(spdlog::level::trace);
	 }
}
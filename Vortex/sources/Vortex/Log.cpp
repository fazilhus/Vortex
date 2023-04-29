#include "vtpch.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/fmt/ostr.h"

namespace Vortex {

	Ref<spdlog::logger> Log::s_coreLogger;
	Ref<spdlog::logger> Log::s_clientLogger;

	void Log::Init() {
		// Log pattern
		const auto pattern = "%^[%T %e] %n-> %v%$";

		const auto coreConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		coreConsoleSink->set_level(spdlog::level::trace);
		coreConsoleSink->set_pattern(pattern);

		const auto coreFileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("../SandBox/logs/log.txt", 1024 * 1024, 5, true);
		coreFileSink->set_level(spdlog::level::trace);
		coreFileSink->set_pattern(pattern);

		const spdlog::sinks_init_list coreSinkList{coreConsoleSink, coreFileSink};
		s_coreLogger = std::make_shared<spdlog::logger>("CORE", coreSinkList.begin(), coreSinkList.end());

		const auto appConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		appConsoleSink->set_level(spdlog::level::trace);
		appConsoleSink->set_pattern(pattern);

		const auto appFileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("../SandBox/logs/log.txt", 1024 * 1024, 5, true);
		appFileSink->set_level(spdlog::level::trace);
		appFileSink->set_pattern(pattern);

		const spdlog::sinks_init_list appSinkList{ appConsoleSink, appFileSink };
		s_clientLogger = std::make_shared<spdlog::logger>("APP", appSinkList.begin(), appSinkList.end());
	 }
}
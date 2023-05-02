#include "vtpch.hpp"
#include "Vortex/Core/Profiling/Profiler.hpp"

namespace Vortex {

	Profiler::Profiler()
	: m_currentSession(nullptr), m_profileCount(0) {}

	void Profiler::BeginSession(const std::string& name, const std::string& filepath = "results.json") {
		VT_CORE_INFO("Profiler session {0} start to file {1}", name, filepath);
		m_output.open(filepath, std::ios::end | std::ios::trunc);
		WriteHeader();
		m_currentSession = CreateScope<ProfilerSession>(name);
	}

	void Profiler::EndSession() {
		VT_CORE_INFO("Profiler session {0} end", m_currentSession->name);
		WriteFooter();
		m_output.close();
		m_currentSession.release();
		m_profileCount = 0;
	}

	void Profiler::WriteProfile(const ProfilerResult& res) {
		if (m_profileCount++ > 0) {
			m_output << ",";
		}

		std::string name = res.name;
		std::replace(name.begin(), name.end(), '"', '\'');

		m_output << "	{\n";
		m_output << "		\"cat\":\"function\",\n";
		m_output << "		\"dur\":" << (res.end - res.start) << ",\n";
		m_output << "		\"name\":\"" << name << "\",\n";
		m_output << "		\"ph\":\"X\",\n";
		m_output << "		\"pid\":0,\n";
		m_output << "		\"tid\":" << res.threadID << ",\n";
		m_output << "		\"ts\":" << res.start << "\n";
		m_output << "	}";

		m_output.flush();
	}

	Profiler& Profiler::Get() {
		static Profiler instance;
		return instance;
	}

	void Profiler::WriteHeader() {
		m_output << "{\"otherData\": {},\n\"traceEvents\":\n[\n";
		m_output.flush();
	}

	void Profiler::WriteFooter() {
		m_output << "]}";
		m_output.flush();
	}

	Timer::Timer(const char* name)
	: m_name(name), m_stopped(false) {
		m_startPoint = std::chrono::high_resolution_clock::now();
	}

	Timer::~Timer() {
		if (!m_stopped) {
			Stop();
		}
	}

	void Timer::Stop() {
		auto endPoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startPoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();

		uint4 threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Profiler::Get().WriteProfile({ m_name, start, end, threadID });

		m_stopped = true;
	}

}

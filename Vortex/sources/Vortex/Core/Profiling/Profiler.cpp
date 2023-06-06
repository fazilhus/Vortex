#include <vtpch.hpp>
#include "Vortex/Core/Profiling/Profiler.hpp"

namespace Vortex {

	Profiler::Profiler()
	: m_currentSession(nullptr), m_profileCount(0) {}

	void Profiler::BeginSession(const std::string& name, const std::string& filepath = "results.json") {
		std::lock_guard lock(m_mutex);
		if (m_currentSession) {
			if(Log::GetCoreLogger()) {
				VT_CORE_ERROR("Profiler::BeginSession('{0}') was called when session {1} already open", name, m_currentSession->name);
			}
			InternalEndSession();
		}
		VT_CORE_INFO("Profiler session {0} start to file {1}", name, filepath);
		m_output.open(filepath, std::ios::end | std::ios::trunc);

		if (!m_output.is_open()) {
			if (Log::GetCoreLogger()) {
				VT_CORE_ERROR("Profiler could not open file '{0}'", filepath);
			}
			return;
		}

		WriteHeader();
		m_currentSession = CreateScope<ProfilerSession>(name);
	}

	void Profiler::EndSession() {
		std::lock_guard lock(m_mutex);
		VT_CORE_INFO("Profiler session {0} end", m_currentSession->name);
		InternalEndSession();
	}

	void Profiler::WriteProfile(const ProfilerResult& res) {
		std::stringstream ss;

		if (m_profileCount++ > 0) {
			ss << ",\n";
		}

		std::string name = res.name;
		std::replace(name.begin(), name.end(), '"', '\'');

		ss << "	{\n";
		ss << "		\"cat\":\"function\",\n";
		ss << "		\"dur\":" << (res.end - res.start) << ",\n";
		ss << "		\"name\":\"" << name << "\",\n";
		ss << "		\"ph\":\"X\",\n";
		ss << "		\"pid\":0,\n";
		ss << "		\"tid\":" << res.threadID << ",\n";
		ss << "		\"ts\":" << res.start << "\n";
		ss << "	}";

		std::lock_guard lock(m_mutex);
		if (m_currentSession) {
			m_output << ss.str();
			m_output.flush();
		}
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

	void Profiler::InternalEndSession() {
		if (m_currentSession) {
			WriteFooter();
			m_output.close();
			m_currentSession.release();
			m_currentSession = nullptr;
			m_profileCount = 0;
		}
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

		Profiler::Get().WriteProfile({ m_name, start, end, std::this_thread::get_id() });

		m_stopped = true;
	}

}

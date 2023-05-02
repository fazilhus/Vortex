#pragma once
#include "vtpch.hpp"

namespace Vortex {

	struct ProfilerResult {
		std::string name;
		long long start, end;
		std::thread::id threadID;
	};

	struct ProfilerSession {
		std::string name;
	};

	class Profiler {
	private:
		Scope<ProfilerSession> m_currentSession;
		std::ofstream m_output;
		int m_profileCount;

		std::mutex m_mutex;

	public:
		Profiler();
		~Profiler() = default;

		static void OnImGuiRender();

		void BeginSession(const std::string& name, const std::string& filepath);
		void EndSession();

		void WriteProfile(const ProfilerResult& res);

		static Profiler& Get();

	private:
		void WriteHeader();
		void WriteFooter();

		void InternalEndSession();
	};

	class Timer {
	private:
		const char* m_name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startPoint;
		bool m_stopped;

	public:
		Timer(const char* name);
		~Timer();

		void Stop();
	};

}

#ifdef VT_PROFILE
	#define VT_PROFILE_BEGIN_SESSION(name, filepath) ::Vortex::Profiler::Get().BeginSession(name, filepath)
	#define VT_PROFILE_END_SESSION()                 ::Vortex::Profiler::Get().EndSession()
	#define VT_PROFILE_SCOPE(name)                   ::Vortex::Timer timer##__LINE__(name)
	#define VT_PROFILE_FUNCTION()                    VT_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define VT_PROFILE_BEGIN_SESSION(name, filepath)
	#define VT_PROFILE_END_SESSION()
	#define VT_PROFILE_SCOPE(name)
	#define VT_PROFILE_FUNCTION()
#endif
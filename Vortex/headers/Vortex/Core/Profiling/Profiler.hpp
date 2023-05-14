#pragma once
#include <vtpch.hpp>

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
	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define VT_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define VT_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__FUNCSIG__)
		#define VT_FUNC_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define VT_FUNC_SIG __FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define VT_FUNC_SIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define VT_FUNC_SIG __func__
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define VT_FUNC_SIG __func__
	#else
		#define VT_FUNC_SIG "VT_FUNC_SIG unknown!"
	#endif

	#define VT_PROFILE_BEGIN_SESSION(name, filepath) ::Vortex::Profiler::Get().BeginSession(name, filepath)
	#define VT_PROFILE_END_SESSION()                 ::Vortex::Profiler::Get().EndSession()
	#define VT_PROFILE_SCOPE(name)                   ::Vortex::Timer timer##__LINE__(name)
	#define VT_PROFILE_FUNC()                        VT_PROFILE_SCOPE(VT_FUNC_SIG)
#else
	#define VT_PROFILE_BEGIN_SESSION(name, filepath)
	#define VT_PROFILE_END_SESSION()
	#define VT_PROFILE_SCOPE(name)
	#define VT_PROFILE_FUNC()
#endif
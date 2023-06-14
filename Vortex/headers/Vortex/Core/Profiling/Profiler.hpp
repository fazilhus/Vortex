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

	class ProfilerTimer {
	private:
		const char* m_name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startPoint;
		bool m_stopped;

	public:
		ProfilerTimer(const char* name);
		~ProfilerTimer();

		void Stop();
	};

	namespace Utils {

		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}

	}

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
	#define VT_PROFILE_SCOPE_LINE2(name, line)       constexpr auto fixedName##line = ::Vortex::Utils::CleanupOutputString(name, "__cdecl ");\
												     ::Vortex::ProfilerTimer timer##line(fixedName##line.Data)
	#define VT_PROFILE_SCOPE_LINE(name, line)        VT_PROFILE_SCOPE_LINE2(name, line)
	#define VT_PROFILE_SCOPE(name)                   VT_PROFILE_SCOPE_LINE(name, __LINE__)
	#define VT_PROFILE_FUNC()                        VT_PROFILE_SCOPE(VT_FUNC_SIG)
#else
	#define VT_PROFILE_BEGIN_SESSION(name, filepath)
	#define VT_PROFILE_END_SESSION()
	#define VT_PROFILE_SCOPE(name)
	#define VT_PROFILE_FUNC()
#endif

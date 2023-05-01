#include "vtpch.hpp"
#include "Platforms/Win/WinPlatformAPI.hpp"

namespace Vortex {

	float WinPlatformAPI::GetTimeSec() {
		VT_PROFILE_FUNCTION();
		LARGE_INTEGER t, fr;
		QueryPerformanceCounter(&t);
		QueryPerformanceFrequency(&fr);
		return t.QuadPart / (float)fr.QuadPart;
	}

	float WinPlatformAPI::GetTimeMil() {
		VT_PROFILE_FUNCTION();
		LARGE_INTEGER t, fr;
		QueryPerformanceCounter(&t);
		QueryPerformanceFrequency(&fr);
		return (1000.0f * t.QuadPart) / (float)fr.QuadPart;
	}

}
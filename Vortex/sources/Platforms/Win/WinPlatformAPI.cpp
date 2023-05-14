#include <vtpch.hpp>
#include "Platforms/Win/WinPlatformAPI.hpp"

namespace Vortex {

	float WinPlatformAPI::GetTimeSec() {
		LARGE_INTEGER t, fr;
		QueryPerformanceCounter(&t);
		QueryPerformanceFrequency(&fr);
		return t.QuadPart / (float)fr.QuadPart;
	}

	float WinPlatformAPI::GetTimeMil() {
		LARGE_INTEGER t, fr;
		QueryPerformanceCounter(&t);
		QueryPerformanceFrequency(&fr);
		return (1000.0f * t.QuadPart) / (float)fr.QuadPart;
	}

}
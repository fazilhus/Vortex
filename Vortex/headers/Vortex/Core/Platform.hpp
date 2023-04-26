#pragma once
#include "vtpch.hpp"
#include "Vortex/Core/PlatformAPI.hpp"

namespace Vortex {

	class Platform {
	private:
		static PlatformAPI* s_platformAPI;

	public:
		inline static float GetTimeSec() {
			return s_platformAPI->GetTimeSec();
		}

		inline static float GetTimeMil() {
			return s_platformAPI->GetTimeMil();
		}
	};

}
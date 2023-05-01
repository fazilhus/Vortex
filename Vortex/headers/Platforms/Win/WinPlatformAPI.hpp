#pragma once
#include "Vortex/Core/PlatformAPI.hpp"

namespace Vortex {

	class WinPlatformAPI : public PlatformAPI {
	public:
		virtual float GetTimeSec() override;
		virtual float GetTimeMil() override;
	};

}
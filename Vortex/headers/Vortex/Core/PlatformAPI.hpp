#pragma once

namespace Vortex {

	class PlatformAPI {
	public:
		enum class OS {
			NONE = 0,
			WIN = 1
		};

	private:
		static OS s_os;

	public:
		virtual float GetTimeSec() = 0;
		virtual float GetTimeMil() = 0;
	};

}
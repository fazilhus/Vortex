#include <vtpch.hpp>
#include "Vortex/Core/Platform.hpp"
#include "Platforms/Win/WinPlatformAPI.hpp"

namespace Vortex {

	PlatformAPI* Platform::s_platformAPI = new WinPlatformAPI();

}
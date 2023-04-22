#pragma once

// STL
#include <iostream>

#include <algorithm>
#include <utility>
#include <functional>

#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

#include <memory>

#include <string>
#include <sstream>

// SPDLOG
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

#ifdef VT_PLATFORM_WIN
	#include <Windows.h>
#endif
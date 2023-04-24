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

// Vortex
#include "Vortex/Core.hpp"
#include "Vortex/Log.hpp"
#include "Vortex/Layers./Layer.hpp"
#include "Vortex/Window.hpp"
#include "Vortex/Input.hpp"
#include "Vortex/Codes.hpp"

// GLFW
#include <GLFW/glfw3.h>

// ImGui
#include <imgui.h>

// GLM
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

#ifdef VT_PLATFORM_WIN
	#include <Windows.h>
#endif
#pragma once

// STL
#include <iostream>
#include <fstream>
#include <algorithm>
#include <utility>
#include <functional>
#include <array>
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

// Vortex
#include "Vortex/Core.hpp"
#include "Vortex/Log.hpp"
#include "Vortex/Layers/Layer.hpp"
#include "Vortex/Layers/LayerStack.hpp"

#include "Vortex/Window.hpp"
#include "Vortex/Input.hpp"
#include "Vortex/Codes.hpp"
#include "Vortex/Events/Event.hpp"

#include "Vortex/Renderer/GraphicsContext.hpp"
#include "Vortex/Renderer/Shader.hpp"
#include "Vortex/Renderer/Buffer.hpp"
#include "Vortex/Renderer/Render.hpp"
#include "Vortex/Renderer/Renderer.hpp"
#include "Vortex/Renderer/RendererAPI.hpp"
#include "Vortex/Renderer/Texture.hpp"

#include "Vortex/Core/Platform.hpp"
#include "Vortex/Core/PlatformAPI.hpp"
#include "Vortex/Core/Timestep.hpp"

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

// STB
#include <stb_image.h>

#ifdef VT_PLATFORM_WIN
	#include <Windows.h>
#endif
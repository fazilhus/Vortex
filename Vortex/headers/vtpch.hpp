#pragma once

// STL
#include <iostream>
#include <fstream>
#include <algorithm>
#include <utility>
#include <functional>
#include <tuple>
#include <memory>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <climits>

// SPDLOG
#include "spdlog/spdlog.h"

// Vortex
#include "Vortex/Core/Core.hpp"
#include "Vortex/Core/Log.hpp"
#include "Vortex/Core/Window.hpp"
#include "Vortex/Core/Input.hpp"
#include "Vortex/Core/Codes.hpp"
#include "Vortex/Core/Platform.hpp"
#include "Vortex/Core/PlatformAPI.hpp"
#include "Vortex/Core/Timestep.hpp"

#include "Vortex/Core/Containers/ContainerWrappers.hpp"

#include "Vortex/Core/Profiling/Profiler.hpp"

#include "Vortex/Layers/Layer.hpp"
#include "Vortex/Layers/LayerStack.hpp"

#include "Vortex/Events/Event.hpp"

#include "Vortex/Renderer/GraphicsContext.hpp"
#include "Vortex/Renderer/Shader.hpp"
#include "Vortex/Renderer/Buffer.hpp"
#include "Vortex/Renderer/Render.hpp"
#include "Vortex/Renderer/Renderer.hpp"
#include "Vortex/Renderer/Renderer2D.hpp"
#include "Vortex/Renderer/RendererAPI.hpp"
#include "Vortex/Renderer/Texture.hpp"

#include "Vortex/Renderer/Cameras/Camera.hpp"
#include "Vortex/Renderer/Cameras/OrthoCamera.hpp"

#include "Vortex/Scene/Scene.hpp"
#include "Vortex/Scene/Components.hpp"

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
#include <glm/gtc/type_ptr.hpp>

// STB
#include <stb_image.h>

// ENTT
#include <entt/entt.hpp>

// YAML-CPP
#include <yaml-cpp/yaml.h>

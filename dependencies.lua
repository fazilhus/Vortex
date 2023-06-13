vulkanSDK = os.getenv("VULKAN_SDK")

includedir = {}
includedir["all"] = "%{wks.location}/Vortex/vendor"
includedir["spdlog"] = "%{wks.location}/Vortex/vendor/spdlog/include"
includedir["glfw"] = "%{wks.location}/Vortex/vendor/glfw/include"
includedir["glad"] = "%{wks.location}/Vortex/vendor/glad/include"
includedir["imgui"] = "%{wks.location}/Vortex/vendor/imgui"
includedir["glm"] = "%{wks.location}/Vortex/vendor/glm"
includedir["stb"] = "%{wks.location}/Vortex/vendor/stb"
includedir["entt"] = "%{wks.location}/Vortex/vendor/entt/single_include"
includedir["yaml"] = "%{wks.location}/Vortex/vendor/yaml-cpp/include"
includedir["imguizmo"] = "%{wks.location}/Vortex/vendor/ImGuizmo"
includedir["shaderc"] = "%{wks.location}/Vortex/vendor/shaderc/include"
includedir["SPIRV_Cross"] = "%{wks.location}/Vortex/vendor/spirv-cross/include"
includedir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

librarydir = {}

librarydir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
librarydir["VulkanSDK_Debug"] = "%{wks.location}/Vortex/vendor/VulkanSDK/Lib"

library = {}
library["Vulkan"] = "%{librarydir.VulkanSDK}/vulkan-1.lib"
library["VulkanUtils"] = "%{librarydir.VulkanSDK}/VkLayer_utils.lib"

library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

library["ShaderC_Release"] = "%{librarydir.VulkanSDK}/shaderc_shared.lib"
library["SPIRV_Cross_Release"] = "%{librarydir.VulkanSDK}/spirv-cross-core.lib"
library["SPIRV_Cross_GLSL_Release"] = "%{librarydir.VulkanSDK}/spirv-cross-glsl.lib"
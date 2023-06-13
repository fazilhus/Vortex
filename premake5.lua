include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Vortex"
    architecture "x86_64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

    solution_items {
        ".editorconfig"
    }

    flags {
        "MultiProcessorCompile"
    }

    startproject "Vortex-Editor"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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

group "Dependencies"
    include "vendor/Premake"
    include "Vortex/vendor/glfw"
    include "Vortex/vendor/glad"
    include "Vortex/vendor/ImGui"
    include "Vortex/vendor/yaml-cpp"

group "Tools"
    include "Vortex-Editor"

group ""
    include "Vortex"
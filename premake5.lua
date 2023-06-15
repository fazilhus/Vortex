include "./vendor/premake/premake_customization/solution_items.lua"
include "dependencies.lua"

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

group "Dependencies"
    include "vendor/Premake"
    include "Vortex/vendor/glfw"
    include "Vortex/vendor/glad"
    include "Vortex/vendor/ImGui"
    include "Vortex/vendor/yaml-cpp"
    include "Vortex/vendor/Box2D"

group "Tools"
    include "Vortex-Editor"

group ""
    include "Vortex"
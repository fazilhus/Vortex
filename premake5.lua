workspace "Vortex"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

    startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includedir = {}
includedir["all"] = "Vortex/vendor"
includedir["spdlog"] = "Vortex/vendor/spdlog/include"
includedir["glfw"] = "Vortex/vendor/glfw/include"
includedir["glad"] = "Vortex/vendor/glad/include"
includedir["imgui"] = "Vortex/vendor/imgui"
includedir["glm"] = "Vortex/vendor/glm"

group "Dependencies"
    include "Vortex/vendor/glfw"
    include "Vortex/vendor/glad"
    include "Vortex/vendor/ImGui"

group ""

    project "Vortex"
        location "Vortex"
        kind "StaticLib"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"

        ignoredefaultlibraries {
            "libcmtd"
        }

        defines {
            "_CRT_SECURE_NO_WARNINGS"
        }

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        pchheader "vtpch.hpp"
        pchsource "Vortex/sources/vtpch.cpp"

        files {
            "%{prj.name}/headers/**.hpp",
            "%{prj.name}/sources/**.cpp",
            "%{prj.name}/vendor/glm/glm/**.hpp",
            "%{prj.name}/vendor/glm/glm/**.inl"
        }

        includedirs {
            "%{prj.name}/headers",
            "%{includedir.spdlog}",
            "%{includedir.glfw}",
            "%{includedir.glad}",
            "%{includedir.imgui}",
            "%{includedir.glm}"
        }

        links {
            "GLFW",
            "opengl32.lib",
            "GLAD",
            "ImGui"
        }

        filter "system:windows"
            systemversion "latest"

            defines {
                "VT_PLATFORM_WIN",
                "VT_BUILD_DLL",
                "GLFW_INCLUDE_NONE"
            }

    
        filter "configurations:Debug"
            defines {
                "VT_DEBUG",
                "VT_ENABLE_ASSERTS"
            }
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "VT_RELEASE"
            runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            defines "VT_DIST"
            runtime "Release"
            optimize "on"

    project "Sandbox"
        location "Sandbox"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "%{prj.name}/headers/**.hpp",
            "%{prj.name}/sources/**.cpp"
        }

        includedirs {
            "Vortex/headers",
            "%{includedir.spdlog}",
            "%{includedir.glfw}",
            "%{includedir.glad}",
            "%{includedir.imgui}",
            "%{includedir.glm}"
        }

        links {
            "Vortex"
        }

        filter "system:windows"
            systemversion "latest"

            defines {
                "VT_PLATFORM_WIN"
            }
    
        filter "configurations:Debug"
            defines "VT_DEBUG"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "VT_RELEASE"
            runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            defines "VT_DIST"
            runtime "Release"
            optimize "on"
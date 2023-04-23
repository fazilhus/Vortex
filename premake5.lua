workspace "Vortex"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includedir = {}
includedir["spdlog"] = "Vortex/vendor/spdlog/include"
includedir["glfw"] = "Vortex/vendor/glfw/include"
includedir["glad"] = "Vortex/vendor/glad/include"

include "Vortex/vendor/glfw"
include "Vortex/vendor/glad"

project "Vortex"
    location "Vortex"
    kind "SharedLib"
    language "C++"

    ignoredefaultlibraries {
		"libcmtd"
    }

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "vtpch.hpp"
    pchsource "Vortex/sources/vtpch.cpp"

    files {
        "%{prj.name}/headers/**.hpp",
        "%{prj.name}/sources/**.cpp"
    }

    includedirs {
        "%{prj.name}/headers",
        "%{includedir.spdlog}",
        "%{includedir.glfw}",
        "%{includedir.glad}"
    }

    links {
        "GLFW",
        "opengl32.lib",
        "GLAD"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines {
            "VT_PLATFORM_WIN",
            "VT_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }
    
    filter "configurations:Debug"
        defines {
            "VT_DEBUG",
            "VT_ENABLE_ASSERTS"
        }
        buildoptions "/MDd"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines "VT_RELEASE"
        buildoptions "/MD"
        symbols "On"
        optimize "On"

    filter "configurations:Dist"
        defines "VT_DIST"
        buildoptions "/MD"
        symbols "Off"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/headers/**.hpp",
        "%{prj.name}/sources/**.cpp"
    }

    includedirs {
        "%{prj.name}/headers",
        "Vortex/headers",
        "%{includedir.spdlog}",
        "%{includedir.glfw}",
        "%{includedir.glad}"
    }

    links {
        "Vortex"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines {
            "VT_PLATFORM_WIN"
        }
    
    filter "configurations:Debug"
        defines "VT_DEBUG"
        buildoptions "/MDd"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines "VT_RELEASE"
        buildoptions "/MD"
        symbols "On"
        optimize "On"

    filter "configurations:Dist"
        defines "VT_DIST"
        buildoptions "/MD"
        symbols "Off"
        optimize "On"
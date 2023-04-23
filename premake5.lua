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

include "Vortex/vendor/glfw"

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
        "%{includedir.glfw}"
    }

    links {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "VT_PLATFORM_WIN",
            "VT_BUILD_DLL" 
        }

        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }
    
    filter "configurations:Debug"
        defines {
            "VT_DEBUG",
            "VT_ENABLE_ASSERTS"
        }
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines "VT_RELEASE"
        symbols "On"
        optimize "On"

    filter "configurations:Dist"
        defines "VT_DIST"
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
        "%{includedir.glfw}"
    }

    links {
        "Vortex"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "VT_PLATFORM_WIN"
        }
    
    filter "configurations:Debug"
        defines "VT_DEBUG"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines "VT_RELEASE"
        symbols "On"
        optimize "On"

    filter "configurations:Dist"
        defines "VT_DIST"
        symbols "Off"
        optimize "On"
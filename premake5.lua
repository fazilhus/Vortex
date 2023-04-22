workspace "Vortex"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Vortex"
    location "Vortex"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/headers/**.hpp",
        "%{prj.name}/sources/**.cpp"
    }

    includedirs {
        "%{prj.name}/headers",
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++20"
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
        defines "VT_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "VT_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "VT_DIST"
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
        "Vortex/vendor/spdlog/include",
        "Vortex/headers"
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
        symbols "On"

    filter "configurations:Release"
        defines "VT_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "VT_DIST"
        symbols "On"
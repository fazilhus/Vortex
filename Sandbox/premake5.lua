project "Sandbox"
    kind "ConsoleApp"
    language (lang)
    cppdialect (cppdlct)
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/headers/**.hpp",
        "%{prj.name}/sources/**.cpp",
        "../Vortex/vendor/stb/**.h",
        "../Vortex/vendor/stb/**.cpp"
    }

    includedirs {
        "%{prj.name}/headers",
        "../Vortex/headers",
        "../%{includedir.spdlog}",
        "../%{includedir.glfw}",
        "../%{includedir.glad}",
        "../%{includedir.imgui}",
        "../%{includedir.glm}",
        "../%{includedir.stb}"
    }

    links {
        "Vortex"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines {
            "VT_DEBUG",
            "VT_PROFILE"
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
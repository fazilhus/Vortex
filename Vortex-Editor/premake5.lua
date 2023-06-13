project "Vortex-Editor"
    kind "ConsoleApp"
    language "c++"
    cppdialect "c++20"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "headers/**.hpp",
        "sources/**.cpp"
    }

    includedirs {
        "headers",
        "%{wks.location}/Vortex/headers",
        "%{wks.location}/Vortex/vendor",
        "%{includedir.spdlog}",
        "%{includedir.glfw}",
        "%{includedir.glad}",
        "%{includedir.imgui}",
        "%{includedir.glm}",
        "%{includedir.stb}",
        "%{includedir.entt}",
        "%{includedir.yaml}",
        "%{includedir.imguizmo}"
    }

    links {
        "Vortex"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines {
            "VT_DEBUG",
            "VT_ENABLE_ASSERTS",
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
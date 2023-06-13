project "Vortex"
    kind "StaticLib"
    language "c++"
    cppdialect "c++20"
    staticruntime "on"

    ignoredefaultlibraries {
        "libcmtd"
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE",
        "YAML_CPP_STATIC_DEFINE"
    }

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "vtpch.hpp"
    pchsource "sources/vtpch.cpp"

    files {
        "headers/**.hpp",
        "sources/**.cpp",
        "vendor/glm/glm/**.hpp",
        "vendor/glm/glm/**.inl",
        "vendor/stb/**.h",
        "vendor/stb/**.cpp",
        "vendor/ImGuizmo/ImGuizmo.h",
        "vendor/ImGuizmo/ImGuizmo.cpp"
    }

    includedirs {
        "headers",
        "%{includedir.spdlog}",
        "%{includedir.glfw}",
        "%{includedir.glad}",
        "%{includedir.imgui}",
        "%{includedir.glm}",
        "%{includedir.stb}",
        "%{includedir.entt}",
        "%{includedir.yaml}",
        "%{includedir.imguizmo}",
        "%{includedir.VulkanSDK}"
    }

    links {
        "GLFW",
        "opengl32.lib",
        "GLAD",
        "ImGui",
        "yaml-cpp"
    }

    filter "files:vendor/ImGuizmo/**.cpp"
        flags {
            "NoPCH"
        }

    filter "system:windows"
        systemversion "latest"

        defines {
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
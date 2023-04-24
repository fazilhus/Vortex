project "ImGui"
	kind "StaticLib"
	language "C++"
    staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"imconfig.h",
		"imgui.h",
		"imgui_internal.h",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui.cpp",
		"imgui_demo.cpp",
		"imgui_draw.cpp",
		"imgui_tables.cpp",
		"imgui_widgets.cpp"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++20"


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
        optimize "off"

	filter "configurations:Release"
		runtime "Release"
        symbols "off"
		optimize "off"

    filter "configurations:Dist"
		runtime "Release"
        symbols "off"
		optimize "on"

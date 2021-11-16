project "Base"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.c",
		"src/**.cpp",

		"vendor/lodepng/lodepng.h",
		"vendor/lodepng/lodepng.cpp",

		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",

		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp",

		"vendor/Lua/src/lua.h",
		"vendor/Lua/src/lua.c"
	}

	includedirs
	{
		"src/",
		"%{IncludeDirectories.Glfw}",
		"%{IncludeDirectories.Glad}",
		"%{IncludeDirectories.LodePng}",
		"%{IncludeDirectories.ImGui}",
		"%{IncludeDirectories.ImGuizmo}",
		"%{IncludeDirectories.yaml}",
		"%{IncludeDirectories.Box2D}",
		"%{IncludeDirectories.stb_image}",
		"%{IncludeDirectories.entt}",
		"%{IncludeDirectories.spdlog}",
		"%{IncludeDirectories.glm}",
		"%{IncludeDirectories.debugbreak}",
		"%{IncludeDirectories.Lua}"
	}

	links
	{
		"Box2D",
		"yaml-cpp",
		"Glad",
		"ImGui",
		"GLFW",
		"opengl32.lib",
		"Lua"
	}

	postbuildcommands
	{
		("{COPYDIR} \"./src/**.h\" \"./include/Base\""),
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"BASE_WINDOWS_BUILD",
			"BASE_STATIC_BUILD"
		}

	filter "configurations:Debug"
		defines "BASE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "BASE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "BASE_DIST"
		optimize "On"

	--filter { "system:windows", "configurations:Debug" }
	--	buildoptions "/MTd"
	--
	--filter { "system:windows", "configurations:Release" }
	--	buildoptions "/MT"
	--
	--filter { "system:windows", "configurations:Dist" }
	--	buildoptions "/MT"
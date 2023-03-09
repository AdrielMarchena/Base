project "Base"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource ( "src/pch.cpp" )

	files 
	{
		"src/**.h",
		"src/**.c",
		"src/**.cpp",

		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp",

		--"vendor/Lua/lua.h",
		"vendor/Lua/lua.c"
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
		"%{IncludeDirectories.Lua}",
		"%{IncludeDirectories.msdfgen}",
		"%{IncludeDirectories.msdf_atlas_gen}",
		"%{IncludeDirectories.meta}",
	}

	links
	{
		"Box2D",
		"yaml-cpp",
		"Glad",
		"ImGui",
		"GLFW",
		"opengl32.lib",
		"Lua",
		"msdfgen",
		"msdf-atlas-gen",
		"freetype.lib",
		"stb_image",
		"lodepng"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	postbuildcommands
	{
		("{COPYDIR} \"./src/**.h\" \"./include/Base\""),
	}

	filter { 'files:vendor/Lua/lua.c' }
		flags { 'NoPCH' }

	filter { 'files:vendor/Lua/lua.h' }
		flags { 'NoPCH' }

	filter { 'files:vendor/lodepng/lodepng.h' }
		flags { 'NoPCH' }

	filter { 'files:vendor/lodepng/lodepng.cpp' }
		flags { 'NoPCH' }

	filter { 'files:vendor/ImGuizmo/ImGuizmo.h' }
		flags { 'NoPCH' }

	filter { 'files:vendor/ImGuizmo/ImGuizmo.cpp' }
		flags { 'NoPCH' }

	filter { 'files:vendor/stb_image/**.h' }
		flags { 'NoPCH' }

	filter { 'files:vendor/stb_image/**.cpp' }
		flags { 'NoPCH' }

	filter { "platforms:x64", "system:Windows" }
		libdirs
		{
			"%{wks.location}/Base/vendor/msdfgen/freetype/win64"
		}
	
	filter { "platforms:x86", "system:Windows" }
		libdirs
		{
			"%{wks.location}/Base/vendor/msdfgen/freetype/win32"
		}

	filter "system:Windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"BASE_WINDOWS_BUILD",
			"BASE_STATIC_BUILD"
		}

	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"BASE_UNIX_BUILD",
			"BASE_STATIC_BUILD"
		}

		links
		{
			"pthread",
			"dl"
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
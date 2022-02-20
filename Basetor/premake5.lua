project "Basetor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "pch.h"
	pchsource "src/pch.cpp"

	files 
	{
		"src/**.h",
		"src/**.cpp",
		"src/**.c",
	}

	includedirs
	{
		"%{IncludeDirectories.Base}",
		"%{IncludeDirectories.Box2D}",
		"%{IncludeDirectories.ImGui}",
		"%{IncludeDirectories.yaml}",
		"%{IncludeDirectories.entt}",
		"%{IncludeDirectories.spdlog}",
		"%{IncludeDirectories.glm}",
		"%{IncludeDirectories.debugbreak}",
		"%{IncludeDirectories.ImGuizmo}",
		"%{IncludeDirectories.Lua}",
		"%{IncludeDirectories.msdfgen}",
		"src/"
	}

	links
	{
		"Base",
		"Box2D",
		"yaml-cpp",
		"Glad",
		"ImGui",
		"GLFW",
		--"opengl32.lib",
		"Lua",
		"msdfgen",
		"msdf-atlas-gen",
		--"freetype.lib",
		"stb_image",
		"lodepng"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	filter { "platforms:x64", "system:windows" }
		libdirs
		{
			"%{wks.location}/Base/vendor/msdfgen/freetype/win64"
		}
	
	filter { "platforms:x86", "system:windows" }
		libdirs
		{
			"%{wks.location}/Base/vendor/msdfgen/freetype/win32"
		}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"BASE_WINDOWS_BUILD"
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

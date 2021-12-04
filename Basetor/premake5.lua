project "Basetor"
	kind "ConsoleApp"
	language "C++"

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
		"Lua",
		"ImGui",
		"msdfgen",
		"freetype.lib"
	}

	filter "platforms:x64"
		libdirs
		{
			"%{wks.location}/Base/vendor/msdfgen/freetype/win64"
		}
	
	filter "platforms:x86"
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
			"BASE_WINDOWS_BUILD"
		}
	
	filter "system:Unix"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"BASE_UNIX_BUILD",
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

project "Life"
	location "Life"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Base/vendor/Box2D/include",
		"Base/vendor/stb_image",
		"Base/vendor/entt/single_include",
		"Base/vendor/libsndfile/include",
		"Base/vendor/spdlog/include",
		"Base/src",
		"Base/vendor/glm",
		"Base/vendor/debugbreak",
	}
	
	links
	{
		"Base"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"BASE_WINDOWS_BUILD"
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
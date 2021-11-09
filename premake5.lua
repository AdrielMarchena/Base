workspace "Base"
	
	startproject "Sandbox"

	platforms
	{
		"x64",
		"x86"
	}

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	filter "platforms:x64"
		architecture "x64"
	
	filter "platforms:x86"
		architecture "x86"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Base"
	location "Base"
	kind "StaticLib"
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
		"%{prj.name}/vendor/yaml-cpp/include",
		"%{prj.name}/vendor/Box2D/include",
		"%{prj.name}/vendor/stb_image",
		"%{prj.name}/vendor/entt/single_include",
		"%{prj.name}/vendor/libsndfile/include",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{prj.name}/vendor/glm",
		"%{prj.name}/vendor/debugbreak"
	}
	
	libdirs 
	{ 
		"%{prj.name}/vendor/yaml-cpp/bin/" .. outputdir .. "/yaml-cpp",
		"%{prj.name}/vendor/Box2D/bin/" .. outputdir .. "/Box2D",
	}
	links
	{
		"Box2D",
		"yaml-cpp"
	}

	postbuildcommands
	{
		("{COPYDIR} \"./src/**.h\" \"./include/\""),
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

		flags
		{
			"MultiProcessorCompile"
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

	filter { "system:windows", "configurations:Debug" }
		buildoptions "/MTd"
	
	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"

	filter { "system:windows", "configurations:Dist" }
		buildoptions "/MT"




project "Sandbox"
	location "SandBox"
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
		"Base/vendor/yaml-cpp/include",
		--"Base/vendor/Box2D/include",--
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
		
		flags
		{
			"MultiProcessorCompile"
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

	filter { "system:windows", "configurations:Debug" }
		buildoptions "/MTd"
	
	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"

	filter { "system:windows", "configurations:Dist" }
		buildoptions "/MT"



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

	filter { "system:windows", "configurations:Debug" }
		buildoptions "/MTd"
	
	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"

	filter { "system:windows", "configurations:Dist" }
		buildoptions "/MT"
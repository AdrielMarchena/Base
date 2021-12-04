project "msdfgen"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"core/**.h",
		"core/**.cpp",
		"ext/**.h",
		"ext/**.hpp",
		"include/**.h",
		"ext/*.cpp",
		"lib/*.cpp"
	}
	
	includedirs
	{
		"include",
		"core",
		"ext",
		"lib",
		"freetype/include"
	}
	
	filter "platforms:x64"
		libdirs
		{
			"./freetype/win32"
		}
	
	filter "platforms:x86"
		libdirs
		{
			"./freetype/win64"
		}
	
	links
	{
		"freetype.lib"
	}	
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		
	filter "configurations:Dist"
		runtime "Release"
		symbols "on"

	filter { "system:windows", "configurations:Debug" }
		buildoptions "/MTd"
	
	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"
	
	filter { "system:windows", "configurations:Dist" }
		buildoptions "/MT"
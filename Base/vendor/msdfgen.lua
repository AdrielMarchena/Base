project "msdfgen"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
	staticruntime "off"

	targetdir ("msdfgen/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("msdfgen/bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"msdfgen/core/**.h",
		"msdfgen/core/**.cpp",
		"msdfgen/ext/**.h",
		"msdfgen/ext/**.hpp",
		"msdfgen/include/**.h",
		"msdfgen/ext/*.cpp",
		"msdfgen/lib/*.cpp"
	}
	
	includedirs
	{
		"msdfgen/include",
		"msdfgen/core",
		"msdfgen/ext",
		"msdfgen/lib",
		"msdfgen/freetype/include"
	}
	
	filter "platforms:x64"
		libdirs
		{
			"msdfgen/freetype/win32"
		}
	
	filter "platforms:x86"
		libdirs
		{
			"msdfgen/freetype/win64"
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

	-- filter { "system:windows", "configurations:Debug" }
	-- 	buildoptions "/MTd"
	-- 
	-- filter { "system:windows", "configurations:Release" }
	-- 	buildoptions "/MT"
	-- 
	-- filter { "system:windows", "configurations:Dist" }
	-- 	buildoptions "/MT"
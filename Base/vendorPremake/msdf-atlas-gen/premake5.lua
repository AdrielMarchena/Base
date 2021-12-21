project "msdf-atlas-gen"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{

		"msdf-atlas-gen/*.h",
		"msdf-atlas-gen/*.hpp",
		"msdf-atlas-gen/*.cpp"
	}
	
	removefiles
	{
		"msdf-atlas-gen/main.cpp"
	}

	includedirs
	{
		"%{IncludeDirectories.msdfgen}",
		"msdfgen/include",
		"artery-font-format",
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
		"msdfgen",
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
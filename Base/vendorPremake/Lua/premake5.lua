project "Lua"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"**.h",
		"**.c",
		"**.hpp",
	}
	removefiles
	{
		--"src/lua.h",
		"lua.c",
		"luac.c",
	}

	includedirs
	{
		"src",
		"./"
	}

	defines
	{
		"MAKE_LIB"
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
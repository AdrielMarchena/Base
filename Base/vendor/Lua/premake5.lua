project "Lua"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.c",
		"src/**.hpp",
	}
	removefiles
	{
		--"src/lua.h",
		"src/lua.c",
		"src/luac.c",
	}

	includedirs
	{
		"src"
	}

	postbuildcommands
	{
		("{COPYDIR} \"./src/**.h\" \"./include/Lua\""),
		("{COPYDIR} \"./src/**.hpp\" \"./include/Lua\""),

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
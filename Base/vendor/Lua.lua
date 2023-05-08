project "Lua"
	kind "StaticLib"
	language "C"
	staticruntime "off"

	targetdir ("Lua/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Lua/bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		--"Lua/**.h",
		--"Lua/**.c",
		--"Lua/**.hpp",
		"Lua/onelua.c"
	}
	removefiles
	{
		--"src/lua.h",
		"Lua/lua.c",
		"Lua/luac.c",
	}

	includedirs
	{
		"Lua/src",
		"Lua/"
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
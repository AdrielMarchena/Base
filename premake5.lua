include "Dependencies.lua"
workspace "Base"
	startproject "Basetor"

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

	flags
	{
		"MultiProcessorCompile"
	}

	filter "platforms:x64"
		architecture "x64"
	
	filter "platforms:x86"
		architecture "x86"

	filter "system:Windows"
		system "windows"
		
	filter "system:Unix"
		system "linux"

	filter { "system:windows", "configurations:Debug" }
		buildoptions "/MTd"
	
	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"
	
	filter { "system:windows", "configurations:Dist" }
		buildoptions "/MT"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"

--Libs
include "Base/vendor/glfw.lua"
include "Base/vendor/imgui.lua"
include "Base/vendor/glad.lua"
include "Base/vendor/yaml-cpp.lua"
include "Base/vendor/Box2D.lua"
include "Base/vendor/Lua.lua"
include "Base/vendor/msdfgen.lua"
include "Base/vendor/msdf-atlas-gen.lua"
include "Base/vendor/stb_image.lua"
include "Base/vendor/lodepng.lua"

group "Core"
include "Base"
include "Base-ScriptCore"

group "Tools"
--Projects
include "Basetor"
--include "Sandbox"

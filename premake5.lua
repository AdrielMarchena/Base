include "Dependencies.lua"

workspace "Base"
	startproject "Basetor"

	platforms
	{
		"x64",
		"x86"
	}

	system
	(
		"Windows",
		"Unix"
	)

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

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"

--Libs
include "Base/vendor/glfw"
include "Base/vendor/imgui"
include "Base/vendor/glad"
include "Base/vendor/yaml-cpp"
include "Base/vendor/Box2D"
include "Base/vendor/Lua"
group ""

--Projects
include "Base"
include "Basetor"
--include "Sandbox"
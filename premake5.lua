include "Dependencies.lua"

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

	flags
	{
		"MultiProcessorCompile"
	}

	filter "platforms:x64"
		architecture "x64"
	
	filter "platforms:x86"
		architecture "x86"

	

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"

--Libs
include "Base/vendor/glfw"
include "Base/vendor/imgui"
include "Base/vendor/glad"
include "Base/vendor/yaml-cpp"
include "Base/vendor/Box2D"

group ""

--Projects
include "Base"
include "Sandbox"
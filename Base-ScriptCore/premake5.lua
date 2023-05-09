project "Base-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{wks.location}/Basetor/Resources/Scripts")
	objdir ("%{wks.location}/Basetor/Resources/Scripts/Intermediates")

	files
	{
		"Source/**.cs",
		"Properties/**.cs",
	}
	
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "On"
		symbols "Off"

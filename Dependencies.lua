IncludeDirectories = {}
IncludeDirectories["CoreBase"] = "%{wks.location}/Base/src/"
IncludeDirectories["Base"] = "%{wks.location}/Base/include"
IncludeDirectories["Box2D"] = "%{wks.location}/Base/vendor/Box2D/include"
IncludeDirectories["debugbreak"] = "%{wks.location}/Base/vendor/debugbreak"
IncludeDirectories["entt"] = "%{wks.location}/Base/vendor/entt/single_include"
IncludeDirectories["Glad"] = "%{wks.location}/Base/vendor/glad/include"
IncludeDirectories["Glfw"] = "%{wks.location}/Base/vendor/glfw/include"
IncludeDirectories["glm"] = "%{wks.location}/Base/vendor/glm"
IncludeDirectories["ImGui"] = "%{wks.location}/Base/vendor/imgui"
IncludeDirectories["LodePng"] = "%{wks.location}/Base/vendor/lodepng"
IncludeDirectories["spdlog"] = "%{wks.location}/Base/vendor/spdlog/include"
IncludeDirectories["stb_image"] = "%{wks.location}/Base/vendor/stb_image"
IncludeDirectories["yaml"] = "%{wks.location}/Base/vendor/yaml-cpp/include"
IncludeDirectories["ImGuizmo"] = "%{wks.location}/Base/vendor/ImGuizmo"
IncludeDirectories["Lua"] = "%{wks.location}/Base/vendor/Lua"
IncludeDirectories["msdfgen"] = "%{wks.location}/Base/vendor/msdfgen"
IncludeDirectories["msdf_atlas_gen"] = "%{wks.location}/Base/vendor/msdf-atlas-gen"
IncludeDirectories["meta"] = "%{wks.location}/Base/vendor/meta/src"
IncludeDirectories["mono"] = "%{wks.location}/Base/vendor/mono/include"
IncludeDirectories["reflect_cpp"] = "%{wks.location}/Base/vendor/reflect-cpp/include"

LibDirectories = {}
LibDirectories["Mono"] = "%{wks.location}/Base/vendor/mono/lib/%{cfg.buildcfg}"

filter "platforms:x64"
	LibDirectories["freetype"] = "%{wks.location}/Base/vendor/msdfgen/freetype/win64"
	
filter "platforms:x86"
	LibDirectories["freetype"] = "%{wks.location}/Base/vendor/msdfgen/freetype/win32"

Library = {}
Library["mono"] = "%{LibDirectories.Mono}/libmono-static-sgen.lib"
Library["monoR"] = "%{LibDirectories.Mono}/MonoPosixHelper.lib"
Library["monoP"] = "%{LibDirectories.Mono}/libmonoruntime-sgen.lib"
Library["monoG"] = "%{LibDirectories.Mono}/libgcmonosgen.lib"
Library["monoE"] = "%{LibDirectories.Mono}/eglib.lib"
Library["monoM"] = "%{LibDirectories.Mono}/eglib.lib"

-- Windows

Library["WinSock"] = "Ws2_32.lib"
Library["Winmm"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["WinOldNames"] = "OLDNAMES.lib"
Library["WinBCrypt"] = "Bcrypt.lib"

SourceDir = {}
SourceDir["reflect_cpp"] = "%{wks.location}/Base/vendor/reflect-cpp/src"
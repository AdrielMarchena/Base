# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_x64
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

RESCOMP = windres
PCH = ../Base/src/pch.h
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -I../Base/src -I../Base/vendor/Box2D/include -I../Base/vendor/imgui -I../Base/vendor/yaml-cpp/include -I../Base/vendor/entt/single_include -I../Base/vendor/spdlog/include -I../Base/vendor/glm -I../Base/vendor/debugbreak -I../Base/vendor/ImGuizmo -I../Base/vendor/Lua -I../Base/vendor/msdfgen -Isrc
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug_x64)
TARGETDIR = bin/Debug-linux-x86_64/Basetor
TARGET = $(TARGETDIR)/Basetor
OBJDIR = bin-int/Debug-linux-x86_64/Basetor
DEFINES += -DYAML_CPP_STATIC_DEFINE -DBASE_UNIX_BUILD -DBASE_STATIC_BUILD -DBASE_DEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -g -std=c++17
LIBS += ../Base/bin/Debug-linux-x86_64/Base/libBase.a ../Base/vendor/Box2D/bin/Debug-linux-x86_64/Box2D/libBox2D.a ../Base/vendor/yaml-cpp/bin/Debug-linux-x86_64/yaml-cpp/libyaml-cpp.a ../Base/vendor/glad/bin/Debug-linux-x86_64/Glad/libGlad.a ../Base/vendor/imgui/bin/Debug-linux-x86_64/ImGui/libImGui.a ../Base/vendor/glfw/bin/Debug-linux-x86_64/GLFW/libGLFW.a ../Base/vendor/Lua/bin/Debug-linux-x86_64/Lua/libLua.a ../Base/vendor/msdfgen/bin/Debug-linux-x86_64/msdfgen/libmsdfgen.a ../Base/vendor/msdf-atlas-gen/bin/Debug-linux-x86_64/msdf-atlas-gen/libmsdf-atlas-gen.a ../Base/vendor/bin/Debug-linux-x86_64/stb_image/libstb_image.a ../Base/vendor/bin/Debug-linux-x86_64/lodepng/liblodepng.a -lpthread -ldl
LDDEPS += ../Base/bin/Debug-linux-x86_64/Base/libBase.a ../Base/vendor/Box2D/bin/Debug-linux-x86_64/Box2D/libBox2D.a ../Base/vendor/yaml-cpp/bin/Debug-linux-x86_64/yaml-cpp/libyaml-cpp.a ../Base/vendor/glad/bin/Debug-linux-x86_64/Glad/libGlad.a ../Base/vendor/imgui/bin/Debug-linux-x86_64/ImGui/libImGui.a ../Base/vendor/glfw/bin/Debug-linux-x86_64/GLFW/libGLFW.a ../Base/vendor/Lua/bin/Debug-linux-x86_64/Lua/libLua.a ../Base/vendor/msdfgen/bin/Debug-linux-x86_64/msdfgen/libmsdfgen.a ../Base/vendor/msdf-atlas-gen/bin/Debug-linux-x86_64/msdf-atlas-gen/libmsdf-atlas-gen.a ../Base/vendor/bin/Debug-linux-x86_64/stb_image/libstb_image.a ../Base/vendor/bin/Debug-linux-x86_64/lodepng/liblodepng.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64

else ifeq ($(config),debug_x86)
TARGETDIR = bin/Debug-linux-x86/Basetor
TARGET = $(TARGETDIR)/Basetor
OBJDIR = bin-int/Debug-linux-x86/Basetor
DEFINES += -DYAML_CPP_STATIC_DEFINE -DBASE_UNIX_BUILD -DBASE_STATIC_BUILD -DBASE_DEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -g -std=c++17
LIBS += ../Base/bin/Debug-linux-x86/Base/libBase.a ../Base/vendor/Box2D/bin/Debug-linux-x86/Box2D/libBox2D.a ../Base/vendor/yaml-cpp/bin/Debug-linux-x86/yaml-cpp/libyaml-cpp.a ../Base/vendor/glad/bin/Debug-linux-x86/Glad/libGlad.a ../Base/vendor/imgui/bin/Debug-linux-x86/ImGui/libImGui.a ../Base/vendor/glfw/bin/Debug-linux-x86/GLFW/libGLFW.a ../Base/vendor/Lua/bin/Debug-linux-x86/Lua/libLua.a ../Base/vendor/msdfgen/bin/Debug-linux-x86/msdfgen/libmsdfgen.a ../Base/vendor/msdf-atlas-gen/bin/Debug-linux-x86/msdf-atlas-gen/libmsdf-atlas-gen.a ../Base/vendor/bin/Debug-linux-x86/stb_image/libstb_image.a ../Base/vendor/bin/Debug-linux-x86/lodepng/liblodepng.a -lpthread -ldl
LDDEPS += ../Base/bin/Debug-linux-x86/Base/libBase.a ../Base/vendor/Box2D/bin/Debug-linux-x86/Box2D/libBox2D.a ../Base/vendor/yaml-cpp/bin/Debug-linux-x86/yaml-cpp/libyaml-cpp.a ../Base/vendor/glad/bin/Debug-linux-x86/Glad/libGlad.a ../Base/vendor/imgui/bin/Debug-linux-x86/ImGui/libImGui.a ../Base/vendor/glfw/bin/Debug-linux-x86/GLFW/libGLFW.a ../Base/vendor/Lua/bin/Debug-linux-x86/Lua/libLua.a ../Base/vendor/msdfgen/bin/Debug-linux-x86/msdfgen/libmsdfgen.a ../Base/vendor/msdf-atlas-gen/bin/Debug-linux-x86/msdf-atlas-gen/libmsdf-atlas-gen.a ../Base/vendor/bin/Debug-linux-x86/stb_image/libstb_image.a ../Base/vendor/bin/Debug-linux-x86/lodepng/liblodepng.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32

else ifeq ($(config),release_x64)
TARGETDIR = bin/Release-linux-x86_64/Basetor
TARGET = $(TARGETDIR)/Basetor
OBJDIR = bin-int/Release-linux-x86_64/Basetor
DEFINES += -DYAML_CPP_STATIC_DEFINE -DBASE_UNIX_BUILD -DBASE_STATIC_BUILD -DBASE_RELEASE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O2 -std=c++17
LIBS += ../Base/bin/Release-linux-x86_64/Base/libBase.a ../Base/vendor/Box2D/bin/Release-linux-x86_64/Box2D/libBox2D.a ../Base/vendor/yaml-cpp/bin/Release-linux-x86_64/yaml-cpp/libyaml-cpp.a ../Base/vendor/glad/bin/Release-linux-x86_64/Glad/libGlad.a ../Base/vendor/imgui/bin/Release-linux-x86_64/ImGui/libImGui.a ../Base/vendor/glfw/bin/Release-linux-x86_64/GLFW/libGLFW.a ../Base/vendor/Lua/bin/Release-linux-x86_64/Lua/libLua.a ../Base/vendor/msdfgen/bin/Release-linux-x86_64/msdfgen/libmsdfgen.a ../Base/vendor/msdf-atlas-gen/bin/Release-linux-x86_64/msdf-atlas-gen/libmsdf-atlas-gen.a ../Base/vendor/bin/Release-linux-x86_64/stb_image/libstb_image.a ../Base/vendor/bin/Release-linux-x86_64/lodepng/liblodepng.a -lpthread -ldl
LDDEPS += ../Base/bin/Release-linux-x86_64/Base/libBase.a ../Base/vendor/Box2D/bin/Release-linux-x86_64/Box2D/libBox2D.a ../Base/vendor/yaml-cpp/bin/Release-linux-x86_64/yaml-cpp/libyaml-cpp.a ../Base/vendor/glad/bin/Release-linux-x86_64/Glad/libGlad.a ../Base/vendor/imgui/bin/Release-linux-x86_64/ImGui/libImGui.a ../Base/vendor/glfw/bin/Release-linux-x86_64/GLFW/libGLFW.a ../Base/vendor/Lua/bin/Release-linux-x86_64/Lua/libLua.a ../Base/vendor/msdfgen/bin/Release-linux-x86_64/msdfgen/libmsdfgen.a ../Base/vendor/msdf-atlas-gen/bin/Release-linux-x86_64/msdf-atlas-gen/libmsdf-atlas-gen.a ../Base/vendor/bin/Release-linux-x86_64/stb_image/libstb_image.a ../Base/vendor/bin/Release-linux-x86_64/lodepng/liblodepng.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -s

else ifeq ($(config),release_x86)
TARGETDIR = bin/Release-linux-x86/Basetor
TARGET = $(TARGETDIR)/Basetor
OBJDIR = bin-int/Release-linux-x86/Basetor
DEFINES += -DYAML_CPP_STATIC_DEFINE -DBASE_UNIX_BUILD -DBASE_STATIC_BUILD -DBASE_RELEASE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -O2 -std=c++17
LIBS += ../Base/bin/Release-linux-x86/Base/libBase.a ../Base/vendor/Box2D/bin/Release-linux-x86/Box2D/libBox2D.a ../Base/vendor/yaml-cpp/bin/Release-linux-x86/yaml-cpp/libyaml-cpp.a ../Base/vendor/glad/bin/Release-linux-x86/Glad/libGlad.a ../Base/vendor/imgui/bin/Release-linux-x86/ImGui/libImGui.a ../Base/vendor/glfw/bin/Release-linux-x86/GLFW/libGLFW.a ../Base/vendor/Lua/bin/Release-linux-x86/Lua/libLua.a ../Base/vendor/msdfgen/bin/Release-linux-x86/msdfgen/libmsdfgen.a ../Base/vendor/msdf-atlas-gen/bin/Release-linux-x86/msdf-atlas-gen/libmsdf-atlas-gen.a ../Base/vendor/bin/Release-linux-x86/stb_image/libstb_image.a ../Base/vendor/bin/Release-linux-x86/lodepng/liblodepng.a -lpthread -ldl
LDDEPS += ../Base/bin/Release-linux-x86/Base/libBase.a ../Base/vendor/Box2D/bin/Release-linux-x86/Box2D/libBox2D.a ../Base/vendor/yaml-cpp/bin/Release-linux-x86/yaml-cpp/libyaml-cpp.a ../Base/vendor/glad/bin/Release-linux-x86/Glad/libGlad.a ../Base/vendor/imgui/bin/Release-linux-x86/ImGui/libImGui.a ../Base/vendor/glfw/bin/Release-linux-x86/GLFW/libGLFW.a ../Base/vendor/Lua/bin/Release-linux-x86/Lua/libLua.a ../Base/vendor/msdfgen/bin/Release-linux-x86/msdfgen/libmsdfgen.a ../Base/vendor/msdf-atlas-gen/bin/Release-linux-x86/msdf-atlas-gen/libmsdf-atlas-gen.a ../Base/vendor/bin/Release-linux-x86/stb_image/libstb_image.a ../Base/vendor/bin/Release-linux-x86/lodepng/liblodepng.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32 -s

else ifeq ($(config),dist_x64)
TARGETDIR = bin/Dist-linux-x86_64/Basetor
TARGET = $(TARGETDIR)/Basetor
OBJDIR = bin-int/Dist-linux-x86_64/Basetor
DEFINES += -DYAML_CPP_STATIC_DEFINE -DBASE_UNIX_BUILD -DBASE_STATIC_BUILD -DBASE_DIST
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O2 -std=c++17
LIBS += ../Base/bin/Dist-linux-x86_64/Base/libBase.a ../Base/vendor/Box2D/bin/Dist-linux-x86_64/Box2D/libBox2D.a ../Base/vendor/yaml-cpp/bin/Dist-linux-x86_64/yaml-cpp/libyaml-cpp.a ../Base/vendor/glad/bin/Dist-linux-x86_64/Glad/libGlad.a ../Base/vendor/imgui/bin/Dist-linux-x86_64/ImGui/libImGui.a ../Base/vendor/glfw/bin/Dist-linux-x86_64/GLFW/libGLFW.a ../Base/vendor/Lua/bin/Dist-linux-x86_64/Lua/libLua.a ../Base/vendor/msdfgen/bin/Dist-linux-x86_64/msdfgen/libmsdfgen.a ../Base/vendor/msdf-atlas-gen/bin/Dist-linux-x86_64/msdf-atlas-gen/libmsdf-atlas-gen.a ../Base/vendor/bin/Dist-linux-x86_64/stb_image/libstb_image.a ../Base/vendor/bin/Dist-linux-x86_64/lodepng/liblodepng.a -lpthread -ldl
LDDEPS += ../Base/bin/Dist-linux-x86_64/Base/libBase.a ../Base/vendor/Box2D/bin/Dist-linux-x86_64/Box2D/libBox2D.a ../Base/vendor/yaml-cpp/bin/Dist-linux-x86_64/yaml-cpp/libyaml-cpp.a ../Base/vendor/glad/bin/Dist-linux-x86_64/Glad/libGlad.a ../Base/vendor/imgui/bin/Dist-linux-x86_64/ImGui/libImGui.a ../Base/vendor/glfw/bin/Dist-linux-x86_64/GLFW/libGLFW.a ../Base/vendor/Lua/bin/Dist-linux-x86_64/Lua/libLua.a ../Base/vendor/msdfgen/bin/Dist-linux-x86_64/msdfgen/libmsdfgen.a ../Base/vendor/msdf-atlas-gen/bin/Dist-linux-x86_64/msdf-atlas-gen/libmsdf-atlas-gen.a ../Base/vendor/bin/Dist-linux-x86_64/stb_image/libstb_image.a ../Base/vendor/bin/Dist-linux-x86_64/lodepng/liblodepng.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -s

else ifeq ($(config),dist_x86)
TARGETDIR = bin/Dist-linux-x86/Basetor
TARGET = $(TARGETDIR)/Basetor
OBJDIR = bin-int/Dist-linux-x86/Basetor
DEFINES += -DYAML_CPP_STATIC_DEFINE -DBASE_UNIX_BUILD -DBASE_STATIC_BUILD -DBASE_DIST
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -O2 -std=c++17
LIBS += ../Base/bin/Dist-linux-x86/Base/libBase.a ../Base/vendor/Box2D/bin/Dist-linux-x86/Box2D/libBox2D.a ../Base/vendor/yaml-cpp/bin/Dist-linux-x86/yaml-cpp/libyaml-cpp.a ../Base/vendor/glad/bin/Dist-linux-x86/Glad/libGlad.a ../Base/vendor/imgui/bin/Dist-linux-x86/ImGui/libImGui.a ../Base/vendor/glfw/bin/Dist-linux-x86/GLFW/libGLFW.a ../Base/vendor/Lua/bin/Dist-linux-x86/Lua/libLua.a ../Base/vendor/msdfgen/bin/Dist-linux-x86/msdfgen/libmsdfgen.a ../Base/vendor/msdf-atlas-gen/bin/Dist-linux-x86/msdf-atlas-gen/libmsdf-atlas-gen.a ../Base/vendor/bin/Dist-linux-x86/stb_image/libstb_image.a ../Base/vendor/bin/Dist-linux-x86/lodepng/liblodepng.a -lpthread -ldl
LDDEPS += ../Base/bin/Dist-linux-x86/Base/libBase.a ../Base/vendor/Box2D/bin/Dist-linux-x86/Box2D/libBox2D.a ../Base/vendor/yaml-cpp/bin/Dist-linux-x86/yaml-cpp/libyaml-cpp.a ../Base/vendor/glad/bin/Dist-linux-x86/Glad/libGlad.a ../Base/vendor/imgui/bin/Dist-linux-x86/ImGui/libImGui.a ../Base/vendor/glfw/bin/Dist-linux-x86/GLFW/libGLFW.a ../Base/vendor/Lua/bin/Dist-linux-x86/Lua/libLua.a ../Base/vendor/msdfgen/bin/Dist-linux-x86/msdfgen/libmsdfgen.a ../Base/vendor/msdf-atlas-gen/bin/Dist-linux-x86/msdf-atlas-gen/libmsdf-atlas-gen.a ../Base/vendor/bin/Dist-linux-x86/stb_image/libstb_image.a ../Base/vendor/bin/Dist-linux-x86/lodepng/liblodepng.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32 -s

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/Basetor.o
GENERATED += $(OBJDIR)/Editor.o
GENERATED += $(OBJDIR)/pch.o
OBJECTS += $(OBJDIR)/Basetor.o
OBJECTS += $(OBJDIR)/Editor.o
OBJECTS += $(OBJDIR)/pch.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking Basetor
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning Basetor
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) rmdir /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/Basetor.o: src/Basetor.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Editor.o: src/Editor.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pch.o: src/pch.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
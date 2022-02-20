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
DEFINES += -D_GLFW_X11
INCLUDES += -Iglfw/include
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LIBS +=
LDDEPS +=
LINKCMD = $(AR) -rcs "$@" $(OBJECTS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug_x64)
TARGETDIR = glfw/bin/Debug-linux-x86_64/GLFW
TARGET = $(TARGETDIR)/libGLFW.a
OBJDIR = glfw/bin-int/Debug-linux-x86_64/GLFW
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -fPIC -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -fPIC -g
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64

else ifeq ($(config),debug_x86)
TARGETDIR = glfw/bin/Debug-linux-x86/GLFW
TARGET = $(TARGETDIR)/libGLFW.a
OBJDIR = glfw/bin-int/Debug-linux-x86/GLFW
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -fPIC -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -fPIC -g
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32

else ifeq ($(config),release_x64)
TARGETDIR = glfw/bin/Release-linux-x86_64/GLFW
TARGET = $(TARGETDIR)/libGLFW.a
OBJDIR = glfw/bin-int/Release-linux-x86_64/GLFW
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O2 -fPIC
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O2 -fPIC
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -s

else ifeq ($(config),release_x86)
TARGETDIR = glfw/bin/Release-linux-x86/GLFW
TARGET = $(TARGETDIR)/libGLFW.a
OBJDIR = glfw/bin-int/Release-linux-x86/GLFW
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -O2 -fPIC
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -O2 -fPIC
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32 -s

else ifeq ($(config),dist_x64)
TARGETDIR = glfw/bin/Dist-linux-x86_64/GLFW
TARGET = $(TARGETDIR)/libGLFW.a
OBJDIR = glfw/bin-int/Dist-linux-x86_64/GLFW
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -fPIC -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -fPIC -g
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64

else ifeq ($(config),dist_x86)
TARGETDIR = glfw/bin/Dist-linux-x86/GLFW
TARGET = $(TARGETDIR)/libGLFW.a
OBJDIR = glfw/bin-int/Dist-linux-x86/GLFW
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -fPIC -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -fPIC -g
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/context.o
GENERATED += $(OBJDIR)/egl_context.o
GENERATED += $(OBJDIR)/glx_context.o
GENERATED += $(OBJDIR)/init.o
GENERATED += $(OBJDIR)/input.o
GENERATED += $(OBJDIR)/linux_joystick.o
GENERATED += $(OBJDIR)/monitor.o
GENERATED += $(OBJDIR)/null_init.o
GENERATED += $(OBJDIR)/null_joystick.o
GENERATED += $(OBJDIR)/null_monitor.o
GENERATED += $(OBJDIR)/null_window.o
GENERATED += $(OBJDIR)/osmesa_context.o
GENERATED += $(OBJDIR)/platform.o
GENERATED += $(OBJDIR)/posix_module.o
GENERATED += $(OBJDIR)/posix_thread.o
GENERATED += $(OBJDIR)/posix_time.o
GENERATED += $(OBJDIR)/vulkan.o
GENERATED += $(OBJDIR)/window.o
GENERATED += $(OBJDIR)/x11_init.o
GENERATED += $(OBJDIR)/x11_monitor.o
GENERATED += $(OBJDIR)/x11_window.o
GENERATED += $(OBJDIR)/xkb_unicode.o
OBJECTS += $(OBJDIR)/context.o
OBJECTS += $(OBJDIR)/egl_context.o
OBJECTS += $(OBJDIR)/glx_context.o
OBJECTS += $(OBJDIR)/init.o
OBJECTS += $(OBJDIR)/input.o
OBJECTS += $(OBJDIR)/linux_joystick.o
OBJECTS += $(OBJDIR)/monitor.o
OBJECTS += $(OBJDIR)/null_init.o
OBJECTS += $(OBJDIR)/null_joystick.o
OBJECTS += $(OBJDIR)/null_monitor.o
OBJECTS += $(OBJDIR)/null_window.o
OBJECTS += $(OBJDIR)/osmesa_context.o
OBJECTS += $(OBJDIR)/platform.o
OBJECTS += $(OBJDIR)/posix_module.o
OBJECTS += $(OBJDIR)/posix_thread.o
OBJECTS += $(OBJDIR)/posix_time.o
OBJECTS += $(OBJDIR)/vulkan.o
OBJECTS += $(OBJDIR)/window.o
OBJECTS += $(OBJDIR)/x11_init.o
OBJECTS += $(OBJDIR)/x11_monitor.o
OBJECTS += $(OBJDIR)/x11_window.o
OBJECTS += $(OBJDIR)/xkb_unicode.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking GLFW
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
	@echo Cleaning GLFW
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
	$(SILENT) $(CC) -x c-header $(ALL_CFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
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

$(OBJDIR)/context.o: glfw/src/context.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/egl_context.o: glfw/src/egl_context.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/glx_context.o: glfw/src/glx_context.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/init.o: glfw/src/init.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/input.o: glfw/src/input.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/linux_joystick.o: glfw/src/linux_joystick.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/monitor.o: glfw/src/monitor.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/null_init.o: glfw/src/null_init.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/null_joystick.o: glfw/src/null_joystick.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/null_monitor.o: glfw/src/null_monitor.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/null_window.o: glfw/src/null_window.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/osmesa_context.o: glfw/src/osmesa_context.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/platform.o: glfw/src/platform.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/posix_module.o: glfw/src/posix_module.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/posix_thread.o: glfw/src/posix_thread.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/posix_time.o: glfw/src/posix_time.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/vulkan.o: glfw/src/vulkan.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/window.o: glfw/src/window.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/x11_init.o: glfw/src/x11_init.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/x11_monitor.o: glfw/src/x11_monitor.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/x11_window.o: glfw/src/x11_window.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/xkb_unicode.o: glfw/src/xkb_unicode.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
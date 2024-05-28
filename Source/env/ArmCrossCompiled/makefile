# +=====================+
# | Module make utility |
# +=====================+

# =========== Drivers
DRIVER_SRC= Actions.cpp Driver.cpp Loader.cpp Commands.cpp
DRIVERS= Keyboard Screen File System

# =========== BIOS targets
BIOS_SRC= BIOS/BIOS.cpp BIOS/POST.cpp BIOS/SysInfo.cpp

# =========== UEFI targets
UEFI_SRC= UEFI/UEFI.cpp UEFI/Records.cpp

# =========== plugin targets
PLUGINS= Panic
PLUGINS_SRC= $(foreach plug,$(PLUGINS),./Plugins/$(plug).cpp)

# =========== System calls
SYSCALLS= Memory
SYSCALLS_SRC= $(foreach call,$(SYSCALLS),./Syscalls/$(call).cpp)

# Module information
MODULE=ENV

# Source files
DRIVERS_SRCLIST= $(foreach drv,$(DRIVERS), $(foreach fil,$(DRIVER_SRC),./Drivers/$(drv)/$(fil))) ./Drivers/Manifests.cpp
MODULE_SRC= Boot.cpp $(UEFI_SRC) $(BIOS_SRC) $(PLUGINS_SRC) $(DRIVERS_SRCLIST) $(SYSCALLS_SRC)

# Actually include the makefile
SOURCE_DIR=./../..
include ../../makefile

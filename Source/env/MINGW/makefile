# +=====================+
# | Module make utility |
# +=====================+

# =========== Drivers
DRIVER_SRC = Actions.cpp Driver.cpp Loader.cpp Commands.cpp
DRIVERS = Keyboard Screen File

# =========== BIOS targets
BIOS_SRC = BIOS/BIOS.cpp BIOS/POST.cpp

# =========== EFI targets
EFI_SRC = EFI/EFI.cpp EFI/Records.cpp

# =========== plugin targets
PLUGINS= Time Memory Input File ECMA35
PLUGINS_SRC= $(foreach plug,$(PLUGINS),./Plugins/$(plug).cpp)

# =========== Submodule information
SUBMODULES= GTerm

# =========== Module target details
MODULE= ENV
DRIVERS_SRCLIST= $(foreach drv,$(DRIVERS), $(foreach fil,$(DRIVER_SRC),./Drivers/$(drv)/$(fil))) ./Drivers/Manifests.cpp
SUBMODULE_SRC= $(foreach submod,$(SUBMODULES),$(wildcard ./Source/$(submod)/*.cpp))
MODULE_SRC= Boot.cpp $(EFI_SRC) $(BIOS_SRC) $(PLUGINS_SRC) $(DRIVERS_SRCLIST) $(SUBMODULE_SRC)


# Actually include the makefile
SOURCE_DIR=./../..
include ../../makefile

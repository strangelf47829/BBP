#########################################
#  Module make utility                  #
#                                       #
#########################################
#  Module: Environment routines         #

# =========== Platform
PLAT= generic

# =========== Drivers
DRIVER_SRC = Actions.cpp Driver.cpp Loader.cpp Commands.cpp
DRIVERS = Keyboard Screen File

# =========== BIOS targets
BIOS_SRC = BIOS/BIOS.cpp BIOS/POST.cpp

# =========== UEFI targets
UEFI_SRC = UEFI/UEFI.cpp UEFI/Records.cpp

# =========== plugin targets
PLUGINS= Time Memory Input File ECMA35
PLUGINS_SRC= $(foreach plug,$(PLUGINS),./Plugins/$(plug).cpp)

# =========== Submodule information
SUBMODULES= GTerm

# =========== Module target details
MODULE= ENV
DRIVERS_SRCLIST= $(foreach drv,$(DRIVERS), $(foreach fil,$(DRIVER_SRC),./Drivers/$(drv)/$(fil))) ./Drivers/Manifests.cpp
SUBMODULE_SRC= $(foreach submod,$(SUBMODULES),$(wildcard ./Source/$(submod)/*.cpp))
MODULE_SRC= Boot.cpp $(UEFI_SRC) $(BIOS_SRC) $(PLUGINS_SRC) $(DRIVERS_SRCLIST) $(SUBMODULE_SRC)

# ========
# END OF MODULE SETTINGS.
# ========

# =========== Platform specific stuff
CC= gcc
CXX= g++
AR= ar
RANLIB= ranlib

# =========== Compiler / Linker flags
CFLAGS= $(SYSCFLAGS) $(EXTCFLAGS)
CXXFLAGS= $(SYSCXXFLAGS) $(EXTCXXFLAGS)
ARFLAGS= $(SYSARFLAGS) $(EXTARFLAGS)
LDFLAGS= $(SYSLDFLAGS) $(EXTLDFLAGS)

# =========== Module specific flags
SYSCFLAGS= -c
SYSCXXFLAGS= -c -std=c++17 -Wall -Wextra -fdiagnostics-show-option
SYSARFLAGS= rc
SYSLDFLAGS=

# =========== Platform specific flags
EXTCFLAGS=
EXTCXXFLAGS=
EXTARFLAGS=
EXTLDFLAGS=

# ========
# END OF MODULE CONFIGURATION.
# ========
MODULE_O= $(patsubst %.cpp,%.o,$(MODULE_SRC))
MODULE_A= lib$(MODULE).a

default: generic

$(MODULE_A): $(MODULE_O)
	@echo
	@$(AR) $(ARFLAGS) libmodule.a $(MODULE_O)
	@echo "+=======================+"
	@echo "|=+= Module Compiled =+=|"
	@echo "+=======================+"

cho:
	@echo $(DRIVERS_SRCLIST)

%.o: %.cpp
	@$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)
	@echo "        $(MODULE) +=+ Compiling $^..."

%.log:
	@echo
	@echo ===========================
	@echo Compiling module $(MODULE)
	@echo "        Platform: $(PLAT)"
	@echo "        Source files: $(MODULE_SRC)"
	@echo ===========================
	@echo
	@rm -f *.log
	@rm -f *.a *.o
	@echo "Hehe" >> $@
	@make $(PLAT) --no-print-directory
	@echo 

generic: $(MODULE).$(PLAT).log $(MODULE_A)




clean:
	@echo Cleaning files for module $(MODULE)...
	@rm -f *.a *.o *.log
	@echo Done!
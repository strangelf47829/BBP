#########################################
#  Module make utility                  #
#                                       #
#########################################
#  Module: Environment routines         #

# =========== Platform
PLAT= generic

# =========== Module target details
MODULE= ENV
MODULE_SRC= Memory.cpp Start.cpp tterm.cpp File.cpp Dir.cpp Time.cpp Graphics.cpp Terminal.cpp

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
	$(AR) $(ARFLAGS) libmodule.a $(MODULE_O)
	@echo "+=======================+"
	@echo "|=+= Module Compiled =+=|"
	@echo "+=======================+"


%.o: %.cpp
	@$(CXX) $^ -o $@ $(CXXFLAGS)
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
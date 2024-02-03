#########################################
#  Module make utility                  #
#                                       #
#########################################
#  Module: Parser Utilities             #

# =========== Platform
PLAT= generic

# =========== Module target details
MODULE= ELSA
MODULE_SRC= Actions.cpp Auxiliary.cpp Backend.cpp Builder.cpp Frontend.cpp Processor.cpp Secondary.cpp EABI.cpp

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
SYSCXXFLAGS= -c -Wall -Wextra -fdiagnostics-show-option
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
	$(AR) $(ARFLAGS) $@ $(MODULE_O)
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
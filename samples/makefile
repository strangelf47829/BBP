# Makefile to build and install samples

# == CHANGE THE SETTINGS BELOW TO SUIT THE ENVIRONMENT ==

PROJDIR= /mnt/c/users/stran/documents/23IN01/Design/Software/BBP

default: all

# Where to install and build
INSTALL_BIN= $(INSTALL_TOP)/samples/$(SAMPLE)
SAMPLES_SRC= $(INSTALL_TOP)/samples/$(SAMPLE)/src
OUT_DIR= ./bin

INSTALL_TO= /usr/local/bin
INSTALL= install
INSTALL_EXEC= $(INSTALL) -m 0755

TO_BIN= l++

# source library settings
BBP_DIR= $(PROJDIR)/build
BBP_LIBDIR= $(BBP_DIR)/lib
BBP_INCDIR= $(BBP_DIR)/include
SRC_INCDIR= $(PROJDIR)/Source/include/
CLEAN_ALL= cd $(BBP_DIR); make clean_all
# == PLATS avail
PLATS= x86 arm x86Debug
SAMPLES= gui l++ debugger
# == Target variables
PLAT=
SAMPLE=
SRC_DIR=src/$(SAMPLE)_$(PLAT)
# == Platform rules
x86:
	@cd $(BBP_DIR); make x86.plat
arm:
	@cd $(BBP_DIR); make arm.plat
x86Debug:
	@cd $(BBP_DIR); make x86Debug.plat
# == Samples per platform
all_x86= x86 $(foreach samp, $(SAMPLES), $(samp).x86)
all_arm= arm $(foreach samp, $(SAMPLES), $(samp).arm)
all_x86Debug= x86Debug $(foreach samp, $(SAMPLES), $(samp).x86Debug)

everything= $(foreach pla, $(PLATS), $(pla) $(foreach samp, $(SAMPLES), $(samp).$(pla)))
all_all= $(foreach pla, $(PLATS), $(all_$(pla)))

everything: $(everything)
all:
	@echo =======================================================
	@echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	@echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	@echo =======================================================
	@echo
	@echo Compiling all targets for all platforms
	@echo Platforms are:
	@echo	$(all_all)
	@echo
	@echo =======================================================
	@echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	@echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	@echo =======================================================
	@echo
	@echo
	@$(foreach pla, $(PLATS), make --no-print-directory ALL_$(pla);)

ALL: $(all_all)

ALL_PLAT:
	@echo =======================================================
	@echo =======================================================
	@echo
	@echo Compiling all targets for platform $(PLAT)
	@echo Targets are:
	@echo	$(all_$(PLAT))
	@echo
	@echo =======================================================
	@echo =======================================================
	@echo
	@echo
	@make --no-print-directory $(all_$(PLAT))
# == Generic target rules
gui_o= $(SRC_DIR)/IO.o $(SRC_DIR)/GraphicsDriver.o $(SRC_DIR)/boot.o
debugger_o= $(SRC_DIR)/IO.o $(SRC_DIR)/GraphicsDriver.o $(SRC_DIR)/boot.o
l++_o= $(SRC_DIR)/IO.o $(SRC_DIR)/lpp.o
sample_o=

gui: $(SRC_DIR)/IO.o $(SRC_DIR)/GraphicsDriver.o $(SRC_DIR)/boot.o
debugger: $(SRC_DIR)/IO.o $(SRC_DIR)/GraphicsDriver.o $(SRC_DIR)/boot.o
l++: $(SRC_DIR)/IO.o $(SRC_DIR)/lpp.o
# == X86 Flags
x86_CXXFLAGS=
x86_CFLAGS=
x86_CXX= x86_64-w64-mingw32-g++
x86_CC= x86_64-w64-mingw32-gcc
x86_LIBS= SDL2 BBPRing0 lua ELF lpp Lexer
x86_PREFIX= x86_64-w64-mingw32-
# == ARM Flags
arm_CXXFLAGS=
arm_CFLAGS=
arm_CXX= arm-none-eabi-g++
arm_CC= arm-none-eabi-gcc
arm_LIBS= BBPRing0 lua lpp ELF Lexer
arm_PREFIX= arm-none-eabi-
# == X86Debug Flags
x86Debug_CXXFLAGS=-g
x86Debug_CFLAGS=-g
x86Debug_CXX= g++
x86Debug_CC= gcc
x86Debug_LIBS= SDL2 BBPRing0 lua ELF lpp Lexer
x86Debug_PREFIX=
# == Makefile Flags
INC= -I$(BBP_INCDIR) -I$(SRC_INCDIR)
CXXFLAGS= $(INC) $($(PLAT)_CXXFLAGS)
CFLAGS= $(INC) $($(PLAT)_CFLAGS)
CXX= $($(PLAT)_CXX)
CC= $($(PLAT)_CC)
LIBS= -Wl,--start-group $(foreach lib,$($(PLAT)_LIBS), -l$(lib)) -Wl,--end-group
PREFIX= $($(PLAT)_PREFIX)
# == Generic Target
OUTFILE=
%.sample: OUTFILE = $(OUT_DIR)/$(PREFIX)$(SAMPLE)
%.sample:
	@echo [Compiling $(SAMPLE) for $(PLAT) as: $(OUTFILE)]
	@$(CXX) -o $(OUTFILE).exe $($(SAMPLE)_o) -L$(BBP_LIBDIR) $(LIBS)
	@cp $(OUTFILE).exe $(OUTFILE)
	@rm $(OUTFILE).exe
	@echo
	@echo ===========
	@echo Compilation of sample '$(SAMPLE)' successful: Executable is at $(OUTFILE)
	@echo ===========
	@echo
	@rm $(SRC_DIR)/*.o
UNAVAILMSG=Sample does not exist
%.unavail:
	@echo
	@echo ===========
	@echo Could not compile sample '$(SAMPLE)' for $(PLAT): $(UNAVAILMSG)
	@echo ===========
	@echo
# == X86 targets
%.x86: SAMPLE = $(basename $@)
%.x86: PLAT = x86
%.x86: x86
	@echo "compiling sample '$(SAMPLE)' for platform '$(PLAT)'"
	@make $(SAMPLE) --no-print-directory SAMPLE="$(SAMPLE)" PLAT="$(PLAT)"
	@make $(SAMPLE).sample --no-print-directory SAMPLE="$(SAMPLE)" PLAT="$(PLAT)"
ALL_x86: SAMPLE = $(basename $@)
ALL_x86: PLAT = x86
ALL_x86: ALL_PLAT

# == ARM targets
%.arm: SAMPLE = $(basename $@)
%.arm: PLAT = arm
%.arm: arm
	@echo "compiling sample '$(SAMPLE)' for platform '$(PLAT)'"
	@make $(SAMPLE) --no-print-directory SAMPLE="$(SAMPLE)" PLAT="$(PLAT)"
	@make $(SAMPLE).sample --no-print-directory SAMPLE="$(SAMPLE)" PLAT="$(PLAT)"
gui.arm:
	@make $(SAMPLE).unavail --no-print-directory SAMPLE="$(SAMPLE)" PLAT="$(PLAT)"
l++.arm:
	@make $(SAMPLE).unavail --no-print-directory SAMPLE="$(SAMPLE)" PLAT="$(PLAT)"
debugger.arm:
	@make $(SAMPLE).unavail --no-print-directory SAMPLE="$(SAMPLE)" PLAT="$(PLAT)"
ALL_arm: SAMPLE = $(basename $@)
ALL_arm: PLAT = arm
ALL_arm: ALL_PLAT
# == X86 Debug targets
%.x86Debug: SAMPLE = $(basename $@)
%.x86Debug: PLAT = x86Debug
%.x86Debug: x86Debug
	@echo "compiling sample '$(SAMPLE)' for platform '$(PLAT)'"
	@make $(SAMPLE) --no-print-directory SAMPLE="$(SAMPLE)" PLAT="$(PLAT)"
	@make $(SAMPLE).sample --no-print-directory SAMPLE="$(SAMPLE)" PLAT="$(PLAT)"
x86Debug.ALL:
	@make ALL_PLAT SAMPLE="$(SAMPLE)" PLAT="$(PLAT)"
ALL_x86Debug: SAMPLE = $(basename $@)
ALL_x86Debug: PLAT = x86Debug
ALL_x86Debug: ALL_PLAT
# == Installation rules
install:
	@cd $(OUT_DIR); $(INSTALL_EXEC) $(TO_BIN) $(INSTALL_TO)

uninstall:
	@cd $(INSTALL_TO); rm $(TO_BIN)
# == Cleanup rules
clean:
	@echo == Cleaning $(OUT_DIR) ==
	@rm -r $(OUT_DIR)
	@mkdir $(OUT_DIR)

test:
	valgrind --leak-check=full --show-leak-kinds=all -s bin/l++ -e -c header.lua

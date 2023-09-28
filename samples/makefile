# Makefile to build and install samples

# == CHANGE THE SETTINGS BELOW TO SUIT THE ENVIRONMENT ==

PROJDIR= /mnt/c/users/stran/documents/23IN01/Design/Software/BBP

SAMPLE= debugger

default: all

# Where to install and build
INSTALL_BIN= $(INSTALL_TOP)/samples/$(SAMPLE)
SAMPLES_SRC= $(INSTALL_TOP)/samples/$(SAMPLE)/src
OUT_DIR= ./bin

INSTALL_TO= /usr/local/bin
INSTALL= install
INSTALL_EXEC= $(INSTALL) -m 0755

TO_BIN= x86_64-bbp-debugger x86_64-bbp-gui x86_64-bbp-win-debugger x86_64-win-gui

# source library settings
BBP_DIR= $(PROJDIR)/build
CLEAN_ALL= cd $(BBP_DIR); make clean_all
# == Platform rules
x86:
	$(CLEAN_ALL) -s; make "PLAT= x86"
arm:
	$(CLEAN_ALL) -s; make "PLAT= arm"
x86Debug:
	$(CLEAN_ALL) -s; make "PLAT= x86Debug" EXT_FLAG="-DBBP_DEBUG"
# == Samples per platform
all: ALL_x86 ALL_x86Debug ALL_ARM

ALL_x86: x86 debugger_x86 GUI_x86
ALL_ARM: arm debugger_arm GUI_arm
ALL_x86Debug: x86Debug debugger_x86Debug GUI_x86Debug
# == X86 targets
debugger_x86:
	make -f makefile.x86 SAMPLE="debugger"

GUI_x86:
	make -f makefile.x86 SAMPLE="gui"
# == ARM targets
debugger_arm:
	make -f makefile.arm SAMPLE="debugger"

GUI_arm:
	make -f makefile.arm SAMPLE="gui"
# == X86 Debug targets
debugger_x86Debug:
	make -f makefile.x86Debug SAMPLE="debugger"

GUI_x86Debug:
	make -f makefile.x86Debug SAMPLE="gui"
# == Installation rules
install:
	@cd $(OUT_DIR); $(INSTALL_EXEC) $(TO_BIN) $(INSTALL_TO)

uninstall:
	@cd $(INSTALL_TO); rm $(TO_BIN)
# == Cleanup rules
clean:
	@echo == Cleaning $(OUT_DIR) ==
	@rm -r $(OUT_DIR); mkdir $(OUT_DIR)

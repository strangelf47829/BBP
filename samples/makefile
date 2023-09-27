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

TO_BIN= bbp_x86_debugger bbp_x86_gui

# source library settings
BBP_DIR= $(PROJDIR)/build
CLEAN_ALL= cd $(BBP_DIR); make clean_all
# == Platform rules
x86:
	$(CLEAN_ALL) -s; make "PLAT= x86" -s
x86Debug:
	$(CLEAN_ALL) -s; make "PLAT= x86Debug" -s
# == Samples per platform
all: ALL_X86Debug ALL_X86

ALL_X86Debug: x86Debug debugger
ALL_X86: GUI x86
# == X86Debug targets
debugger:
	make -f makefile.x86Debug debugger
# == X86 targets
GUI:
	$(CLEAN_ALL) -s; make PLAT="x86" EXT_FLAGS="-DBBP_C_ENTRY" -s
	make -f makefile.x86 gui
# == Installation rules
install:
	@cd $(OUT_DIR); $(INSTALL_EXEC) $(TO_BIN) $(INSTALL_TO)

uninstall:
	@cd $(INSTALL_TO); rm $(TO_BIN)

#########################################
#  Module make utility                  #
#                                       #
#########################################
#  Module: Master BBP Compiler tool     #

# =========== What file is needed to guess
PLAT_GUESS=./setup.sh

# =========== Module target details
MODULE= BBP
MODULE_SRC= Applications ELF ELSA File Graphics Parser RAE stdlib System Daemons Terminal
MODULE_A= Applications ELF ELSA fileSys GLib Parser RAE stdlib System Daemons Terminal

# =========== Environment target details
ENV=$(PLAT)
ENV_DIR= ../Source/env/$(ENV)
ENV_BIN= libENV.a

# =========== OS target details
OS_LIST= BBPOSIX WINDOWSE20
OS_DIR= ../Source/OS
OS_DIRS= $(foreach os,$(OS_LIST),$(OS_DIR)/$(os))
OS_A= $(foreach os,$(OS_LIST),bin/libOS$(os).a)

# EXTNERLA
EXT_A=

# ========
# END OF MODULE SETTINGS.
# ========

# =========== Platform specific stuff
CC= gcc
CXX= g++
AR= ar
RANLIB= ranlib

# =========== Compiler / Linker flags
CFLAGS=
CXXFLAGS= -g
ARFLAGS=
LDFLAGS= -pthread

ifeq ($(PLAT),MINGW)
	LDFLAGS= -lmingw32
	EXT_A= lib/libSDL2main.a lib/libSDL2.a
endif

# ==========
# Default build action
# ==========

ifeq ($(PLAT),)
MAKEFLAGS += --no-print-directory
endif

ifeq ($(PLAT),)
default:
	@echo "No default platform specified, guessing platform..."
	@$(PLAT_GUESS)
else
default: build
endif

# ========
# END OF MODULE CONFIGURATION.
# ========

MODULE_DIRS= $(foreach var,$(MODULE_SRC),../Source/$(var))

.PHONY: $(MODULE_DIRS)



count:
	cloc --exclude-dir=SDL2 ../Source/

env:
	@echo "Compiling platform $(PLAT)/$(ENV)"
	@cd $(ENV_DIR); make EXTCXXFLAGS="$(CXXFLAGS)" --no-print-directory

$(MODULE_DIRS):
	@cd $@; make EXTCXXFLAGS="$(CXXFLAGS)" --no-print-directory

$(OS_LIST):
	@cd "$(OS_DIR)/$@"; make --no-print-directory

ALL_A= $(foreach mod,$(MODULE_A),bin/lib$(mod).a) $(OS_A) bin/$(ENV_BIN) $(EXT_A)
build: $(MODULE_DIRS) $(OS_LIST) env executable


executable:
	g++ $(LDFLAGS) -Wl,--start-group $(ALL_A) -Wl,--end-group $(LDFLAGS)
	@echo "Compiled everything"


clean: $(patsubst %,%.clean,$(notdir $(MODULE_DIRS))) $(ENV_DIR).clean $(foreach osdir,$(OS_DIRS),$(osdir).clean)
	rm -f a.out a.exe bin/*

%.clean:
	@cd ../Source/$(patsubst %.clean,%,$@); make clean --no-print-directory

elf:
	readelf -e /mnt/v/boot/ELSA.o -x 5 -x 4 -x 2 -x 8 -r -s

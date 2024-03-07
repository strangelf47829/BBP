#########################################
#  Module make utility                  #
#                                       #
#########################################
#  Module: Master BBP Compiler tool     #

# =========== Platform
PLAT= UBUNTU

# =========== Module target details
MODULE= BBP
MODULE_SRC= Applications ELF ELSA env_UBUNTU File Graphics Parser RAE stdlib

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
LDFLAGS=

# ========
# END OF MODULE CONFIGURATION.
# ========
MODULE_DIRS= $(foreach var,$(MODULE_SRC),../Source/$(var))

.PHONY: $(MODULE_DIRS)

default: build

$(MODULE_DIRS):
	@cd $@; make EXTCXXFLAGS="$(CXXFLAGS)" --no-print-directory
	@cp $(patsubst %,%/libmodule.a,$@) ./bin/lib$(notdir $@).a

build: $(MODULE_DIRS)
	@echo "Compiled everything"
	@g++ -Wl,--start-group $(patsubst %, bin/lib%.a, $(notdir $(MODULE_DIRS))) -Wl,--end-group -pthread

clean: $(patsubst %,%.clean,$(notdir $(MODULE_DIRS)))
	rm bin/*; rm a.out

%.clean:
	@cd ../Source/$(patsubst %.clean,%,$@); make clean --no-print-directory

elf:
	readelf -e /mnt/v/boot/ELSA.o -x 5 -x 4 -x 2 -x 8 -r -s

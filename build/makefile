#########################################
#  Module make utility                  #
#                                       #
#########################################
#  Module: Master BBP Compiler tool     #

# =========== Platform
PLAT=
ifeq ($(USER),stran)
	PLAT=MINGW
else
	PLAT=UBUNTU
endif

# =========== Module target details
MODULE= BBP
MODULE_SRC= Applications ELF ELSA File Graphics Parser RAE stdlib System Daemons

# =========== Environment target details
ENV=$(PLAT)
ENV_DIR= ../Source/env/$(ENV)
ENV_BIN= libmoduleenv

# =========== OS target details
OS_LIST= BBPOSIX WINDOWSE20
OS_DIR= ../Source/OS
OS_DIRS= $(foreach os,$(OS_LIST),$(OS_DIR)/$(os))

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
LDFLAGS=  -pthread

ifeq ($(PLAT),MINGW)
	LDFLAGS= -lmingw32 -lSDL2main -lSDL2 -L./lib/
endif

# ========
# END OF MODULE CONFIGURATION.
# ========
MODULE_DIRS= $(foreach var,$(MODULE_SRC),../Source/$(var))

.PHONY: $(MODULE_DIRS)

default: build

env:
	@echo "Compiling platform $(PLAT)/$(ENV)"
	@cd $(ENV_DIR); make EXTCXXFLAGS="$(CXXFLAGS)" --no-print-directory
	@cp $(ENV_DIR)/libmodule.a ./bin/libenv.a

$(MODULE_DIRS):
	@cd $@; make EXTCXXFLAGS="$(CXXFLAGS)" --no-print-directory
	@cp $(patsubst %,%/libmodule.a,$@) ./bin/lib$(notdir $@).a

$(OS_LIST): 
	@cd "$(OS_DIR)/$@"; make --no-print-directory
	@cp $(OS_DIR)/$@/libmodule.a ./bin/lib$@OS.a

build: $(MODULE_DIRS) $(OS_LIST) env
	@g++ $(LDFLAGS) -Wl,--start-group $(patsubst %, bin/lib%.a, $(notdir $(MODULE_DIRS))) bin/libenv.a $(foreach os,$(OS_LIST),bin/lib$(os)OS.a) -Wl,--end-group $(LDFLAGS)
	@echo "Compiled everything"
	

clean: $(patsubst %,%.clean,$(notdir $(MODULE_DIRS))) $(ENV_DIR).clean $(foreach osdir,$(OS_DIRS),$(osdir).clean)
	rm bin/*; rm a.out

%.clean:
	@cd ../Source/$(patsubst %.clean,%,$@); make clean --no-print-directory

elf:
	readelf -e /mnt/v/boot/ELSA.o -x 5 -x 4 -x 2 -x 8 -r -s

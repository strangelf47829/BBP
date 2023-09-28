# Makefile to build and install ring 0

# == CHANGE THE SETTINGS BELOW TO SUIT THE ENVIRONMENT ==

PLAT= x86

# Where to install
INSTALL_TOP= ../..
INSTALL_BIN= $(INSTALL_TOP)/Build/bin
INSTALL_INC= $(INSTALL_TOP)/Build/include
INSTALL_LIB= $(INSTALL_TOP)/Build/lib

CC= gcc
CXX= g++

CFLAGS= $(SYSCFLAGS) $(MYCFLAGS) $(EXT_CFLAGS)
CXXFLAGS= $(SYSCXFLAGS) $(MYCXFLAGS) $(EXT_CXFLAGS)

LDFLAGS= $(SYSLDFLAGS) $(MYLDFLAGS)
LIBS= $(SYSLIBS) $(MYLIBS)

AR= ar rcu
RANLIB= ranlib

SYSCFLAGS=-c
SYSCXFLAGS=-c
SYSLDFLAGS=
SYSLIBS=

MYCFLAGS= -Iinclude/
MYCXFLAGS= -Iinclude/
MYLDFLAGS=
MYLIBS=

EXT_CFLAGS=
EXT_CXFLAGS=

# == END OF USER SETTINGS ==

PLATS= x86 micro stdgnu x86Debug

KERNEL_A= libBBPRing0libs.a
KERNEL_O= kernel.o

CORE_O= application.o Interrupts.o
LIB_O= ROM.o R2D.o LuaLibraries.o $(EXT_O)
EXT_O=
BASE_O= $(CORE_O) $(LIB_O)

KERNEL_T= libBBPRing0.a

ALL_O= $(BASE_O) $(KERNEL_O)
ALL_T= $(KERNEL_A) $(KERNEL_T)
ALL_A= $(KERNEL_A)
ALL_H= ../include/*
#ALL_H= ../include/Kernel.h ../include/lua.h ../include/lapi.h ../include/lauxlib.h


# Targets start here
ALL= all

default: $(PLAT)

all: $(ALL_T)

o: $(ALL_O)

a: $(ALL_A)

$(KERNEL_A): $(BASE_O)
	$(AR) $@ $(BASE_O)

$(KERNEL_T): $(KERNEL_O) $(KERNEL_A)
	$(AR) $@ $(ALL_O)
	cp $(KERNEL_A) $(INSTALL_BIN)/$(KERNEL_A)
	cp $(KERNEL_T) $(INSTALL_BIN)/$(KERNEL_T)
	cp -r ../lib/* $(INSTALL_LIB)
	cp $(ALL_H) $(INSTALL_INC)

clean:
	rm -rf $(ALL_T) $(wildcard *.o)
clean_all:
	rm -rf $(ALL_T) $(ALL_O) $(INSTALL_BIN)/* $(INSTALL_LIB)/* $(INSTALL_INC)/*

generic: all

x86:
	$(MAKE) $(ALL) CC="x86_64-w64-mingw32-gcc" CXX="x86_64-w64-mingw32-g++"

x86Debug:
	echo hehehehhehehe
	make x86 EXT_O="Debug.o"
#	$(MAKE) $(ALL) CC="x86_64-w64-mingw32-gcc" CXX="x86_64-w64-mingw32-g++" 

nostdlib:
	$(MAKE) $(ALL) SYSCFLAGS="-nostdlib" SYSCXFLAGS="-nostdlib"

freestanding:
	$(MAKE) $(ALL) SYSCFLAGS="-nostdlib -ffreestanding" SYSCXFLAGS="-nostdlib -ffreestanding"

arm:
	$(MAKE) $(ALL) SYSCFLAGS="-nostdlib -ffreestanding -DSTM_32" SYSCXFLAGS="-nostdlib -ffreestanding -DSTM_32" CC="arm-none-eabi-gcc" CXX="arm-none-eabi-g++"



# Make rules
application.o: application.cpp
R2D.o: R2D.cpp
ROM.o: ROM.cpp
serices.o: services.cpp
Interrupts.o: Interrupts.cpp
kernel.o: kernel.cpp
LuaLibraries.o: LuaLibraries.cpp
Debug.o: Debug.cpp

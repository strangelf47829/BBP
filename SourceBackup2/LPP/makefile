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

SYSCFLAGS=
SYSCXFLAGS=
SYSLDFLAGS=
SYSLIBS=

INCDIR= ../include/

MYCFLAGS= -I$(INCDIR) -I$(INSTALL_INC)/
MYCXFLAGS= -I$(INCDIR) -I$(INSTALL_INC)/
MYLDFLAGS=
MYLIBS=

EXT_CFLAGS=
EXT_CXFLAGS=

# == END OF USER SETTINGS ==

PLATS= x86 micro stdgnu x86Debug

LIB_A= libLPP.a

CORE_O= CommandParser.o Services.o
LIB_O= $(EXT_O)
EXT_O=
BASE_O= $(CORE_O) $(LIB_O)

ALL_O= $(BASE_O)
ALL_A= $(ALL_O) $(LIB_A)
ALL_H= $(INCDIR)Lexer.h $(INCDIR)LuaLexer.h $(INCDIR)LuaELF.h


# Targets start here
ALL= all

default: $(PLAT)

all: $(ALL_A)

o: $(ALL_O)

a: $(ALL_A)

$(LIB_A): $(ALL_O)
	$(AR) $@ $(ALL_O)
	cp $(LIB_A) $(INSTALL_BIN)/$(LIB_A)
	cp $(LIB_A) ../lib/$(LIB_A)

clean:
	rm -rf $(ALL_A) $(wildcard *.o)
clean_all:
	rm -rf $(ALL_T) $(ALL_O) $(INSTALL_BIN)/* $(INSTALL_LIB)/* $(INSTALL_INC)/*

generic: all

x86:
	$(MAKE) $(ALL) CC="x86_64-w64-mingw32-gcc" CXX="x86_64-w64-mingw32-g++"

x86Debug:
	$(MAKE) $(ALL) CC="gcc" CXX="g++" SYSCXFLAGS="-g"

nostdlib:
	$(MAKE) $(ALL) SYSCFLAGS="-nostdlib" SYSCXFLAGS="-nostdlib"

freestanding:
	$(MAKE) $(ALL) SYSCFLAGS="-nostdlib -ffreestanding" SYSCXFLAGS="-nostdlib -ffreestanding"

arm:
	$(MAKE) $(ALL) SYSCFLAGS="-nostdlib -ffreestanding -DSTM_32" SYSCXFLAGS="-nostdlib -ffreestanding -DSTM_32" CC="arm-none-eabi-gcc" CXX="arm-none-eabi-g++"



# Make rules
CommandParser.o: CommandParser.cpp
Services.o: Services.cpp
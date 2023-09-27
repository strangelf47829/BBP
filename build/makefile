## TODO Implement stuff here


PLATS= x86 arm x86Debug
PLAT= x86Debug

TOPDIR= ..
SOURCEDIR= $(TOPDIR)/Source

LUADIR= $(SOURCEDIR)/LUA
BBPDIR= $(SOURCEDIR)/src

BBP_LIBDIR= $(SOURCEDIR)/lib

EXT_FLAGS=

all: LUA RING0

LUA:
	@echo === Compiling Lua for $(PLAT) ===
	@cd $(LUADIR); make $(PLAT) -s;
	@echo === Copying data ===
	@cp $(LUADIR)/liblua.a $(BBP_LIBDIR)/liblua.a
	@echo === LUA COMPILATION COMPLETED. ===

RING0:
	@echo === Compiling BBP RING 0 for $(PLAT) ===
	@cd $(BBPDIR); make $(PLAT) "EXT_CXFLAGS= $(EXT_FLAGS)" -s; make clean -s
	@echo === Copying data ===
	@cp $(TOPDIR)/build/bin/libBBPRing0.a $(TOPDIR)/build/lib/libBBPRing0.a
	@cp $(TOPDIR)/build/bin/libBBPRing0libs.a $(TOPDIR)/build/lib/libBBPRing0libs.a
	@rm $(TOPDIR)/build/bin/libBBPRing0.a
	@rm $(TOPDIR)/build/bin/libBBPRing0libs.a
	@echo === COMPILATION COMPLETE ===

clean_all:
	@echo === Cleaning all ===
	@cd $(LUADIR); make clean
	@cd $(BBPDIR); make clean

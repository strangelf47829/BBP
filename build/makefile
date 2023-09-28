## TODO Implement stuff here


PLATS= x86 arm
PLAT= x86

TOPDIR= ..
SOURCEDIR= $(TOPDIR)/Source

LUADIR= $(SOURCEDIR)/LUA
BBPDIR= $(SOURCEDIR)/src

BBP_LIBDIR= $(SOURCEDIR)/lib

LUA_FLAGS= "-DBBP_ARCH=$(PLAT)"
INT_FLAGS= "-DBBP_ARCH=$(PLAT)" $(EXT_FLAGS)
EXT_FLAGS=

all: LUA RING0

LUA:
	@echo === Compiling Lua for $(PLAT) ===
	@cd $(LUADIR); make $(PLAT) "EXT_FLAGS= $(LUA_FLAGS)";
	@echo === Copying data ===
	@cp $(LUADIR)/liblua.a $(BBP_LIBDIR)/liblua.a
	@echo === LUA COMPILATION COMPLETED. ===

RING0:
	@echo === Compiling BBP RING 0 for $(PLAT) ===
	@cd $(BBPDIR); make $(PLAT) "EXT_CXFLAGS= $(LUA_FLAGS)"; make clean -s
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

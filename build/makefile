## TODO Implement stuff here

PLAT= x86

TOPDIR= ..
SOURCEDIR= $(TOPDIR)/Source

LUADIR= $(SOURCEDIR)/LUA
BBPDIR= $(SOURCEDIR)/src

BBP_LIBDIR= $(SOURCEDIR)/lib

all: LUA RING0

LUA:
	@echo === Compiling Lua for $(PLAT) ===
	@cd $(LUADIR); make $(PLAT) -s;
	@echo === Copying data ===
	@cp $(LUADIR)/liblua.a $(BBP_LIBDIR)/liblua.a
	@cd $(LUADIR); make clean -s
	@echo === LUA COMPILATION COMPLETED. ===

RING0:
	@echo === Compiling BBP RING 0 for $(PLAT) ===
	@cd $(BBPDIR); make $(PLAT) -s; make clean -s
	@echo === COMPILATION COMPLETE ===

## TODO Implement stuff here


PLATS= x86 arm x86Debug
PLAT= x86Debug

TOPDIR= ..
SOURCEDIR= $(TOPDIR)/Source

INT_FLAGS= "-DBBP_ARCH_$(PLAT)" $(EXT_FLAGS)

LUADIR= $(SOURCEDIR)/LUA
LUAINSTALLDIR= $(SOURCEDIR)/LUA
LUA_FLAGS= "-DBBP_ARCH_$(PLAT)"

ELFDIR= $(SOURCEDIR)/ELF
ELFINSTALLDIR= $(TOPDIR)/build/bin/
ELF_FLAGS= $(INT_FLAGS)

LEXERDIR= $(SOURCEDIR)/Lexer
LEXERINSTALLDIR= $(TOPDIR)/build/bin/
LEXER_FLAGS= $(INT_FLAGS)

LPPDIR= $(SOURCEDIR)/LPP
LPPINSTALLDIR= $(TOPDIR)/build/bin/
LPP_FLAGS= $(INT_FLAGS)

BUFFERSDIR= $(SOURCEDIR)/Buffers
BUFFERSINSTALLDIR= $(TOPDIR)/build/bin/
BUFFERS_FLAGS= $(INT_FLAGS)

BBPDIR= $(SOURCEDIR)/src
BBPINSTALLDIR= $(TOPDIR)/build/bin/

BBPRING0DIR= $(BBPDIR)
BBPRING0INSTALLDIR= $(BBPINSTALLDIR)
BBPRING0_FLAGS= $(INT_FLAGS)

BBPRING0LIBSDIR= $(BBPDIR)
BBPRING0LIBSINSTALLDIR= $(BBPINSTALLDIR)
BBPRING0LIBS_FLAGS= $(INT_FLAGS)



EXT_FLAGS=

MODULE=
LIB=

LIBS= lib/libELF.a lib/libLPP.a lib/libLexer.a lib/liblua.a lib/libBBPRing0.a lib/libBBPRing0libs.a lib/libBuffers.a

FORCE:

%.a: MODULE = $(basename $(subst lib/lib,,$@))
%.a: CAPS = $(shell echo $(MODULE) | tr a-z A-Z)
%.a: FORCE
	@echo === Compiling $(MODULE) $(CAPS) for $(PLAT) ===
	@cd $($(CAPS)DIR); make $(PLAT) --no-print-directory "EXT_CXFLAGS= $($(CAPS)_FLAGS)";
	@cp $($(CAPS)INSTALLDIR)/lib$(MODULE).a lib/lib$(MODULE).a
	@echo === Module $(CAPS) COMPILATION COMPLETED. ===


BUILD:
	@make $(LIBS)
	$(file > $(PLAT).log,Compiled at $(shell date))

.PRECIOUS: %.log
%.log:
	@make --no-print-directory clean_all;
	

%.plat: %.log
	make BUILD "PLAT= $(basename $@)"

clean:
	@echo === Cleaning ===
	$(file > dummy.log,dummy)
	@rm -f $(LIBS) *.log

clean_all: clean
	@echo === Cleaning all ===
	@cd $(LUADIR); make clean
	@cd $(BBPDIR); make clean
	@cd $(ELFDIR); make clean
	@cd $(LEXERDIR); make clean
	@cd $(LPPDIR); make clean


.SECONDARY: $(PLAT).log
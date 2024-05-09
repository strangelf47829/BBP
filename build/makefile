# +======================================+
# | Makefile utility to create libraries |
# +======================================+

# Include configuration file
include ../Source/makefile.config

# Get a list of modules
MODULE_LIST= Applications ELF ELSA ELSAnew File Graphics Parser RAE stdlib System Daemons Terminal Builtin

# Get environment details
ENV= $(PLAT)

# Get operating system details
OS_LIST= BBPOSIX WINDOWSE20

# Create a list of targets
MODULE_A= Applications ELF ELSA fileSys Glib Parser RAE stdlib System Daemons Terminal Builtin ELSAnew
ENV_A= env
OS_A= $(foreach os,$(OS_LIST),OS$(os))
ALL_A= $(MODULE_A) $(ENV_A) $(OS_A)

# Directory information
SOURCE_DIR= ../Source

MODULE_DIR=$(SOURCE_DIR)
MODULE_DIRS= $(foreach mod,$(MODULE_LIST),$(MODULE_DIR)/$(mod))
ENV_DIR= $(SOURCE_DIR)/env/$(ENV)
OS_DIR=$(SOURCE_DIR)/OS

# +====================+
# | Begin targets      |
# +====================+

# Create phony targets
.PHONY: $(MODULE_DIRS)

# Default action: build file and executable
default: build executable

# Compilation rules for modules
$(MODULE_DIRS):
	@cd $@; make --no-print-directory

# Compilation rules for OS's
$(OS_LIST):
	@cd "$(OS_DIR)/$@"; make --no-print-directory

# Environment compilation rules
env:
	@cd $(ENV_DIR); make --no-print-directory

# Build rules
build: $(MODULE_DIRS) $(OS_LIST) env

# Actually creating an executable
executable:
	@echo "Creating executable..."
	@$(LD) $(LDFLAGS) -Wl,--start-group $(foreach a_a,$(ALL_A),bin/lib$(a_a).a) -Wl,--end-group $(LDFLAGS)
	@echo "Done."

# Clean out each directory
clean: $(patsubst %,%.clean,$(notdir $(MODULE_DIRS))) $(ENV_DIR).clean $(foreach osdir,$(OS_LIST),$(OS_DIR)/$(osdir).clean)
	rm -f a.out a.exe bin/*

%.clean:
	@cd ../Source/$(patsubst %.clean,%,$@); make clean --no-print-directory

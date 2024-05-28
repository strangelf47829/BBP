# +====================================+
# | Make utility for module creation.  |
# +====================================+

# This file should be included by any modules wishing to use this blah blah blah

# MODULE: The name of the module to compile
ifeq ($(MODULE),)
default:
	@echo "Invalid use of makefile: MODULE variable not set."
else
default: printinfo build
endif

# SOURCE_DIR: Where, relative to the module, is the Source folder
ifeq ($(SOURCE_DIR),)
SOURCE_DIR=./..
endif

# MODULE_DIR: Where to move the module to when it has been created
ifeq ($(MODULE_DIR),)
MODULE_DIR=$(SOURCE_DIR)/../build/bin
endif

# MODULE_A: The name of the file to be created after compilation. Defaults to lib(module).a
ifeq ($(MODULE_A),)
MODULE_A=lib$(MODULE).a
endif

# MODULE_O: The actual target files to be compiled. If it does not exist, create it using MODULE_SRC (the source files of the module)
ifeq ($(MODULE_O),)
MODULE_O=$(patsubst %.cpp,%.o,$(MODULE_SRC))
endif

# Finally include configuration
include $(SOURCE_DIR)/makefile.config

# Printinfo:
printinfo:
	@echo
	@echo
	@echo
	@echo "+--- Module info -----------------------"
	@echo "| >\tModule: $(MODULE)"
	@echo "| >\tPlatform: $(PLAT)"
	@echo "| >\tArchitecture: $(ARCH)"
	@echo "| >\tSource directory: $(SOURCE_DIR)"
	@echo "|"

%.o: %.cpp
	@echo "| >\t$(MODULE) +=+ Compiling '$^'"
	@$(CXX) -o $@ $^ -c $(CXXFLAGS) 2> $@.log; $(SOURCE_DIR)/g++errOutput.sh $@.log

$(MODULE_A): $(MODULE_O)
	@echo "|"
	@echo "| >\tCompiled all source files, building $(MODULE_A)."
	@$(AR) $(ARFLAGS) $(MODULE_A) $(MODULE_O)
	@echo "| >\tModule built, moving $(MODULE_A) to $(MODULE_DIR)/$(MODULE_A)"
	@cp $(MODULE_A) $(MODULE_DIR)/$(MODULE_A)

build: $(MODULE_A)
	@echo "+=== Build info =======================+"
	@echo "| >\t Time: $$(date)"
	@echo "+======================================+"
	@echo
	@echo
	@echo

# Clean own targets,
# Build logs,
# And the compiled library (plus the moved one)
clean:
	@echo "Cleaning $(MODULE)..."
	@rm -f $(MODULE_O) $(MODULE_A) libmodule.a $(MODULE_DIR)/$(MODULE_A) *.log


# +=====================+
# | Module make utility |
# +=====================+

# Module information
MODULE= OSBBPOSIX

# System information
SYSTEM_MODULES= Taskpool
SYSTEM_SRC= $(foreach mod,$(SYSTEM_MODULES),system/$(mod).cpp)

# Application information
APPLICATIONS_SRC=
APPLICATIONS=
APP_SRCLIST= $(foreach app,$(APPLICATIONS), $(foreach fil,$(APPLICATIONS_SRC),./applications/$(app)/$(fil)))

# Source files
MODULE_SRC= Configuration.cpp Entry.cpp Registry.cpp $(APP_SRCLIST) $(SYSTEM_SRC)

# Actually include the makefile
SOURCE_DIR=./../../
include ../../makefile

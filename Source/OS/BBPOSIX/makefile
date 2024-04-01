# +=====================+
# | Module make utility |
# +=====================+

# Module information
MODULE= OSBBPOSIX

# Application information
APPLICATIONS_SRC=
APPLICATIONS=
APP_SRCLIST= $(foreach app,$(APPLICATIONS), $(foreach fil,$(APPLICATIONS_SRC),./applications/$(app)/$(fil)))

# Source files
MODULE_SRC= Configuration.cpp Entry.cpp Registry.cpp $(APP_SRCLIST)

# Actually include the makefile
SOURCE_DIR=./../../
include ../../makefile

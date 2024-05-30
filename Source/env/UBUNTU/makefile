# +=====================+
# | Module make utility |
# +=====================+

# Module information
MODULE=ENV

# =========== BIOS targets
BIOS_SRC= Boot Entry Files Keyboard Screen printf Splash Time

# =========== Configuration targets
CONF_SRC= Config Host Memsize CPUData RecordSearch

# =========== EFI targets
EFI_SRC= InteractiveShell Formatting

# =========== Get list of targets from above lists
BIOS_TGT= $(foreach var,$(BIOS_SRC),BIOS/$(var).cpp)
CONF_TGT= $(foreach var,$(CONF_SRC),Configuration/$(var).cpp)
EFI_TGT=  $(foreach var,$(EFI_SRC),EFI/$(var).cpp)

# Source files
MODULE_SRC= $(BIOS_TGT) $(CONF_TGT) $(EFI_TGT)

# Actually include the makefile
SOURCE_DIR=./../..
include ../../makefile

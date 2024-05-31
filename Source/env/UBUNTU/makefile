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
EFI_SRC= DriverInterface InteractiveShell Formatting

# =========== System targets
SYS_SRC= Memory UserLogin

# =========== File system targets
FILE_SRC= Info Inspection Reading Writing

# +++++++++++ Implemented driverss
DRV_LIST= System FileSystem Screen Keyboard
DRV_SRC= Actions Commands Loader

# =========== Get list of targets from above lists
BIOS_TGT= $(foreach var,$(BIOS_SRC),BIOS/$(var).cpp)
CONF_TGT= $(foreach var,$(CONF_SRC),Configuration/$(var).cpp)
EFI_TGT=  $(foreach var,$(EFI_SRC),EFI/$(var).cpp)
SYS_TGT=  $(foreach var,$(SYS_SRC),System/$(var).cpp)
FILE_TGT=  $(foreach var,$(FILE_SRC),FileSystem/$(var).cpp)
DRV_TGT=  $(foreach drv,$(DRV_LIST),$(foreach var,$(DRV_SRC),Drivers/$(drv)/$(var).cpp))

# Source files
MODULE_SRC= $(BIOS_TGT) $(CONF_TGT) $(EFI_TGT) $(SYS_TGT) $(FILE_TGT) $(DRV_TGT)

# Actually include the makefile
SOURCE_DIR=./../..
include ../../makefile

# ==============================================================================
# Initialization
# ==============================================================================

-include .env.mk

export FXCGSDK
export PREFIX
export PRIZMDEV

ifeq ($(strip $(FXCGSDK)),)
$(error FXCGSDK is not set in your environment. \
	Run `export FXCGSDK=<PATH_TO_SDK>` or add `FXCGSDK=<PATH_TO_SDK>` to '.env.mk')
endif

# Clear implicit built-in rules
.SUFFIXES:

# SDK rules
include $(FXCGSDK)/toolchain/prizm_rules

# ==============================================================================
# Build configuration
# ==============================================================================

OUTPUT_NAME	:= Prizmatron
BUILD_DIR	:= build
SOURCE_DIRS	:= src src/fonts
DATA_DIRS	:= data
INCLUDE_DIRS	:=

# Options for add-in generation
MKG3AFLAGS	:= -n basic:Prizmatron \
			-i uns:../res/unselected.bmp -i sel:../res/selected.bmp

# ==============================================================================
# Flags
# ==============================================================================

# -flto in CFLAGS and LDFLAGS enables link-time optimization (LTO). This usually
# allows the compiler to generate smaller and/or faster code, however may expose
# bugs in the source code that wouldn't cause any trouble without LTO enabled.
CFLAGS		= -std=c17 -Os -Wall $(MACHDEP) $(INCLUDE) -ffunction-sections \
			-fdata-sections -flto

LDFLAGS		= $(MACHDEP) -T$(FXCGSDK)/toolchain/prizm.x -Wl,-static \
			-Wl,-gc-sections -flto

# Libraries
LIBS		:= -lc -lfxcg -lcalctype -lgcc
LIB_DIRS	:= $(SYSTEM_PREFIX)

# ==============================================================================
# Output
# ==============================================================================

## If --------------------------------------------------------------------------
ifneq ($(BUILD_DIR),$(notdir $(CURDIR)))
## -----------------------------------------------------------------------------

export OUTPUT	:= $(CURDIR)/$(OUTPUT_NAME)

export VPATH	:= $(foreach dir,$(SOURCE_DIRS),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA_DIRS),$(CURDIR)/$(dir))

export DEPSDIR	:= $(CURDIR)/$(BUILD_DIR)

# Source files
CFILES		:= $(foreach dir,$(SOURCE_DIRS),\
			$(notdir $(wildcard $(dir)/*.c)))
CXXFILES	:= $(foreach dir,$(SOURCE_DIRS),\
			$(notdir $(wildcard $(dir)/*.cc)))
sFILES		:= $(foreach dir,$(SOURCE_DIRS),\
			$(notdir $(wildcard $(dir)/*.s)))
SFILES		:= $(foreach dir,$(SOURCE_DIRS),\
			$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:= $(foreach dir,$(DATA_DIRS),$(notdir $(wildcard $(dir)/*.*)))

# Linker
ifeq ($(strip $(CXXFILES)),)
export LD	:= $(CC)
else
export LD	:= $(CXX)
endif

# Objects
export OFILES	:= $(addsuffix .o,$(BINFILES)) $(CFILES:.c=.o) \
			$(CXXFILES:.cc=.o) $(sFILES:.s=.o) $(SFILES:.S=.o)

# Include path flags
export INCLUDE	:= $(foreach dir,$(INCLUDE_DIRS),-iquote $(CURDIR)/$(dir)) \
			$(foreach dir,$(LIB_DIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD_DIR) -I$(LIBFXCG_INC)

# Library path flags
export LIBPATHS	:= $(foreach dir,$(LIB_DIRS),-L$(dir)/lib) \
			-L$(LIBFXCG_LIB)

### ============================================================================
### Targets
### ============================================================================

.PHONY: all install install_watch uninstall clean

all: $(BUILD_DIR)
	@make --no-print-directory -C $(BUILD_DIR) -f $(CURDIR)/Makefile

$(BUILD_DIR):
	@mkdir $@

install:
ifeq ($(strip $(PRIZMDEV)),)
$(error PRIZMDEV is not set in your environment. \
	Run `export PRIZMDEV=<PATH_TO_DEVICE>` or add `PRIZMDEV=<PATH_TO_DEVICE>` to '.env.mk')
endif
	@if [ ! -e "$(PRIZMDEV)" ]; \
	then \
		echo "Waiting for device '$(PRIZMDEV)'..."; \
		while [ ! -e "$(PRIZMDEV)" ]; \
		do \
			sleep 1; \
		done; \
	fi
	findmnt --source "$(PRIZMDEV)" > /dev/null && (umount "$(PRIZMDEV)" || exit 1) || true && \
		tmp=$$(mktemp --directory) && \
		(mount "$(PRIZMDEV)" "$$tmp" || (rmdir "$$tmp"; exit 1)) && \
		(cp "$(OUTPUT).g3a" "$$tmp" || (umount "$(PRIZMDEV)"; exit 1)) && \
		umount "$(PRIZMDEV)"; \
		rmdir "$$tmp"

install_watch:
	@sum0=""; \
		while [ true ]; \
		do \
			sum1="$$(md5sum "$(OUTPUT).g3a")"; \
			if [ "$$sum0" != "$$sum1" ]; \
			then \
				make install; \
			fi; \
			sum0="$$sum1"; \
			sleep 1; \
		done;

uninstall:
ifeq ($(strip $(PRIZMDEV)),)
$(error PRIZMDEV is not set in your environment. \
	Run `export PRIZMDEV=<PATH_TO_DEVICE>` or add `PRIZMDEV=<PATH_TO_DEVICE>` to '.env.mk')
endif
	@if [ ! -e "$(PRIZMDEV)" ]; \
	then \
		echo "Waiting for device '$(PRIZMDEV)'..."; \
		while [ ! -e "$(PRIZMDEV)" ]; \
		do \
			sleep 1; \
		done; \
	fi
	findmnt --source "$(PRIZMDEV)" > /dev/null && (umount "$(PRIZMDEV)" || exit 1) || true && \
		tmp=$$(mktemp --directory) && \
		(mount "$(PRIZMDEV)" "$$tmp" || (rmdir "$$tmp"; exit 1)) && \
		(rm "$$tmp/$(OUTPUT_NAME).g3a" || (umount "$(PRIZMDEV)"; exit 1)) && \
		umount "$(PRIZMDEV)"; \
		rmdir "$$tmp"

clean:
	$(call rmdir,$(BUILD_DIR))
	$(call rm,$(OUTPUT).bin)
	$(call rm,$(OUTPUT).g3a)

## Else ------------------------------------------------------------------------
else
## -----------------------------------------------------------------------------

DEPENDS := $(OFILES:.o=.d)

$(OUTPUT).g3a: $(OUTPUT).bin
$(OUTPUT).bin: $(OFILES)

-include $(DEPENDS)

## End if ----------------------------------------------------------------------
endif
## -----------------------------------------------------------------------------

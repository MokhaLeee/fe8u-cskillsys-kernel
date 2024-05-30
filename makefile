MAKEFLAGS += --no-print-directory
include version.mk

MK_PATH   := $(abspath $(lastword $(MAKEFILE_LIST)))
MK_DIR    := $(dir $(MK_PATH))

MAIN    := $(MK_DIR)main.event
FE8_CHX := $(MK_DIR)fe8-kernel-$(CONFIG_VERSION).gba
FE8_GBA := $(MK_DIR)fe8.gba

TOOL_DIR := $(MK_DIR)Tools
LIB_DIR  := $(TOOL_DIR)/FE-CLib-Mokha
FE8_REF  := $(LIB_DIR)/reference/fireemblem8.ref.o
FE8_SYM  := $(LIB_DIR)/reference/fireemblem8.sym

CONFIG_DIR := $(MK_DIR)include/Configs
EXT_REF    := $(CONFIG_DIR)/usr-defined.s

WIZARDRY_DIR := $(MK_DIR)Wizardry
CONTANTS_DIR := $(MK_DIR)Contants
GAMEDATA_DIR := $(MK_DIR)Data

HACK_DIRS := $(CONFIG_DIR) $(WIZARDRY_DIR) $(CONTANTS_DIR) $(GAMEDATA_DIR)

all:
	@$(MAKE) pre_build
	@$(MAKE) chax
	@$(MAKE) post_chax

include Contants/contants.mk

CACHE_DIR := $(MK_DIR).cache_dir
$(shell mkdir -p $(CACHE_DIR) > /dev/null)

CLEAN_FILES :=
CLEAN_DIRS  := $(CACHE_DIR) .release_dir $(shell find -name __pycache__)
CLEAN_BUILD :=

# =========
# = Tools =
# =========

ifeq ($(strip $(DEVKITPRO)),)
	$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitpro)
endif

ifeq ($(strip $(DEVKITARM)),)
	$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM)
endif

TOOLCHAIN ?= $(DEVKITARM)
ifneq (,$(TOOLCHAIN))
	export PATH := $(TOOLCHAIN)/bin:$(PATH)
endif

ifeq ($(OS),Windows_NT)
	EXE := .exe
else
	EXE :=
endif

ifeq ($(shell python3 -c 'import sys; print(int(sys.version_info[0] > 2))'),1)
	PYTHON3 := python3
else
	PYTHON3 := python
endif

PREFIX  ?= arm-none-eabi-
CC      := $(PREFIX)gcc
AS      := $(PREFIX)as
OBJCOPY := $(PREFIX)objcopy

EA_DIR            := $(TOOL_DIR)/EventAssembler
EA                := $(EA_DIR)/ColorzCore$(EXE)
PARSEFILE         := $(EA_DIR)/Tools/ParseFile$(EXE)
PNG2DMP           := $(EA_DIR)/Tools/Png2Dmp$(EXE)
COMPRESS          := $(EA_DIR)/Tools/compress$(EXE)
LYN               := $(EA_DIR)/Tools/lyn$(EXE) -longcalls
EA_DEP            := $(EA_DIR)/ea-dep$(EXE)

TEXT_PROCESS      := $(PYTHON3) $(TOOL_DIR)/FE-PyTools/text-process-classic.py
GRIT              := $(DEVKITPRO)/tools/bin/grit$(EXE)

# ========
# = Main =
# ========

PRE_BUILD ?=
chax: $(FE8_CHX)

$(FE8_CHX): $(MAIN) $(FE8_GBA) $(FE8_SYM) $(shell $(EA_DEP) $(MAIN) -I $(EA_DIR) --add-missings)
	@echo "[GEN]	$@"
	@cp -f $(FE8_GBA) $(FE8_CHX)
	@$(EA) A FE8 -werr -input:$(MAIN) -output:$(FE8_CHX) --nocash-sym || rm -f $(FE8_CHX)

CHAX_SYM := $(FE8_CHX:.gba=.sym)
CHAX_REFS := $(FE8_CHX:.gba=.ref.s)
CHAX_REFE := $(FE8_CHX:.gba=.ref.event)
CHAX_DIFF := $(FE8_CHX:.gba=.bsdiff)

post_chax: $(CHAX_DIFF)

$(CHAX_DIFF): $(FE8_CHX)
	@echo "[GEN]	$(CHAX_REFS)"
	@echo  '@ Auto generated at $(shell date "+%Y-%m-%d %H:%M:%S")' > $(CHAX_REFS)
	@cat $(TOOL_DIR)/scripts/refs-preload.txt >> $(CHAX_REFS)
	@nm $(EXT_REF:.s=.o) | $(PYTHON3) $(TOOL_DIR)/scripts/nm2refs.py >> $(CHAX_REFS)
	@$(PYTHON3) $(TOOL_DIR)/scripts/sym2refs.py $(CHAX_SYM) >> $(CHAX_REFS)

	@echo "[GEN]	$(CHAX_REFE)"
	@echo '// Auto generated at $(shell date "+%Y-%m-%d %H:%M:%S")' > $(CHAX_REFE)
	@nm $(EXT_REF:.s=.o) | $(PYTHON3) $(TOOL_DIR)/scripts/nm2refe.py >> $(CHAX_REFE)
	@echo "PUSH" >> $(CHAX_REFE)
	@$(PYTHON3) $(TOOL_DIR)/scripts/sym2refe.py $(CHAX_SYM) >> $(CHAX_REFE)
	@echo "POP" >> $(CHAX_REFE)

	@cat $(FE8_SYM) >> $(CHAX_SYM)

	@echo "[GEN]	$(CHAX_DIFF)"
	@bsdiff $(FE8_GBA) $(FE8_CHX) $(CHAX_DIFF)

	@echo "Done!"

CLEAN_FILES += $(FE8_CHX)  $(CHAX_SYM) $(CHAX_REFS) $(CHAX_REFE) $(CHAX_DIFF)

# ============
# = Wizardry =
# ============

INC_DIRS := include $(LIB_DIR)/include
INC_FLAG := $(foreach dir, $(INC_DIRS), -I $(dir))

ARCH    := -mcpu=arm7tdmi -mthumb -mthumb-interwork
CFLAGS  := $(ARCH) $(INC_FLAG) -Wall -Wextra -Werror -Wno-unused-parameter -O2 -mtune=arm7tdmi -mlong-calls
ASFLAGS := $(ARCH) $(INC_FLAG)

CDEPFLAGS = -MMD -MT "$*.o" -MT "$*.asm" -MF "$(CACHE_DIR)/$(notdir $*).d" -MP
SDEPFLAGS = --MD "$(CACHE_DIR)/$(notdir $*).d"

LYN_REF := $(EXT_REF:.s=.o) $(FE8_REF)

%.lyn.event: %.o $(LYN_REF)
	@echo "[LYN]	$@"
	@$(LYN) $< $(LYN_REF) > $@

%.dmp: %.o
	@echo "[GEN]	$@"
	@$(OBJCOPY) -S $< -O binary $@

%.o: %.s
	@echo "[AS ]	$@"
	@$(AS) $(ASFLAGS) $(SDEPFLAGS) -I $(dir $<) $< -o $@

%.o: %.c
	@echo "[CC ]	$@"
	@$(CC) $(CFLAGS) $(CDEPFLAGS) -g -c $< -o $@

%.asm: %.c
	@echo "[CC ]	$@"
	@$(CC) $(CFLAGS) $(CDEPFLAGS) -S $< -o $@ -fverbose-asm

# Avoid make deleting objects it thinks it doesn't need anymore
# Without this make may fail to detect some files as being up to date
.PRECIOUS: %.o;

-include $(wildcard $(CACHE_DIR)/*.d)

CFILES := $(shell find $(HACK_DIRS) -type f -name '*.c')
CLEAN_FILES += $(CFILES:.c=.o) $(CFILES:.c=.asm) $(CFILES:.c=.dmp) $(CFILES:.c=.lyn.event)

SFILES := $(shell find $(HACK_DIRS) -type f -name '*.s')
CLEAN_FILES += $(SFILES:.s=.o) $(SFILES:.s=.dmp) $(SFILES:.s=.lyn.event)

# =========
# = Texts =
# =========

TEXT_DIR     := $(CONTANTS_DIR)/Texts
TEXT_MAIN   := $(TEXT_DIR)/Source/TextMain.txt
TEXT_SOURCE := $(shell find $(TEXT_DIR) -type f -name '*.txt')

export TEXT_DEF := $(TEXT_DIR)/TextDefinitions.h

text: $(TEXT_DEF)
PRE_BUILD += text

$(TEXT_DEF): $(TEXT_MAIN) $(TEXT_SOURCE)
	@$(MAKE) -C $(TEXT_DIR)

%.fetxt.dmp: %.fetxt
	@$(MAKE) -f $(TEXT_DIR)/makefile $@

CLEAN_BUILD += $(TEXT_DIR)

# ============
# = Spritans =
# ============

PNG_FILES := $(shell find $(HACK_DIRS) -type f -name '*.png')
TSA_FILES := $(shell find $(HACK_DIRS) -type f -name '*.tsa')

%.4bpp: %.png
	@echo "[GEN]	$@"
	@$(PNG2DMP) $< -o $@

%.gbapal: %.png
	@echo "[GEN]	$@"
	@$(PNG2DMP) $< -po $@ --palette-only

%.lz: %
	@echo "[LZ ]	$@"
	@$(COMPRESS) $< $@

CLEAN_FILES += $(PNG_FILES:.png=.gbapal) $(PNG_FILES:.png=.4bpp) $(PNG_FILES:.png=.4bpp.lz)
CLEAN_FILES += $(TSA_FILES:.tsa=.tsa.lz)

%.img.bin %.map.bin %.pal.bin: %.png
	@echo "[GEN]	$@"
	@$(GRIT) $< -gB 4 -gzl -m -mLf -mR4 -mzl -pn 16 -ftb -fh! -o $@

CLEAN_FILES += $(PNG_FILES:.png=.img.bin) $(PNG_FILES:.png=.map.bin) $(PNG_FILES:.png=.pal.bin)

# ============
# = EfxAnims =
# ============

EFX_ANIM_DIR := $(MK_DIR)Contants/EfxAnim
EFX_ANIMTOR  := $(PYTHON3) $(EFX_ANIM_DIR)/Scripts/efx-anim-creator.py

EFX_SCRIPTS  := $(shell find $(HACK_DIRS) -type f -name '*.efx.txt')
EFX_SCR_DEPS := $(EFX_SCRIPTS:.efx.txt=.efx.txt.d)
EFX_TARGET   := $(EFX_SCRIPTS:.efx.txt=.efx.event)

%.efx.event: %.efx.txt
	@echo "[GEN]	$@"
	@$(EFX_ANIMTOR) $< > $@

%.efx.txt.d: %.efx.txt
	@echo -n "$(patsubst %.efx.txt, %.efx.event, $<): " > $@
	@$(EFX_ANIMTOR) $< --list-files >> $@

-include $(EFX_SCR_DEPS)

efx: $(EFX_TARGET)
PRE_BUILD += efx

CLEAN_FILES += $(EFX_SCR_DEPS) $(EFX_TARGET)

# =======
# = GFX =
# =======

GFX_DIR     := $(MK_DIR)Contants/Gfx
GFX_SOURCES := $(shell find $(GFX_DIR)/Sources -type f -name '*.png')

export GFX_HEADER := $(GFX_DIR)/GfxDefs.h

gfx: $(GFX_HEADER)
PRE_BUILD += gfx

$(GFX_HEADER): $(GFX_SOURCES)
	@$(MAKE) -C $(GFX_DIR)

CLEAN_BUILD += $(GFX_DIR)
CLEAN_FILES += $(GFX_HEADER)

# ========
# = ENUM =
# ========

SKILLS_ENUM_CONFIG := $(MK_DIR)include/constants/skills.enum.txt
SKILLS_ENUM_HEADER := $(MK_DIR)include/constants/skills.h
SKILLS_ENUM_COMBO := $(MK_DIR)Patches/combo.skills.txt

enum: $(SKILLS_ENUM_HEADER)

$(SKILLS_ENUM_HEADER): $(SKILLS_ENUM_CONFIG)
	@echo "[GEN]	$(SKILLS_ENUM_HEADER) $(SKILLS_ENUM_COMBO)"
	@$(PYTHON3) $(TOOL_DIR)/scripts/enum2h.py $(SKILLS_ENUM_CONFIG) > $(SKILLS_ENUM_HEADER)
	@$(PYTHON3) $(TOOL_DIR)/scripts/enum2combo.py $(SKILLS_ENUM_CONFIG) > $(SKILLS_ENUM_COMBO)

PRE_BUILD += enum
CLEAN_FILES += $(SKILLS_ENUM_HEADER) $(SKILLS_ENUM_COMBO)

# =============
# = PRE-BUILD =
# =============
pre_build: $(PRE_BUILD)

# ==============
# = MAKE CLEAN =
# ==============

clean_basic:
	@rm -f $(CLEAN_FILES)
	@rm -rf $(CLEAN_DIRS)

clean:
	@for i in $(CLEAN_BUILD); do if test -e $$i/makefile ; then $(MAKE) -f $$i/makefile clean || { exit 1;} fi; done;
	@$(MAKE) clean_basic
	@echo "Kernel cleaned .."

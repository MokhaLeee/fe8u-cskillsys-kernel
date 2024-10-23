MAKEFLAGS += --no-print-directory

include configs.mk

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
RAM_REF    := $(CONFIG_DIR)/config-memmap.s

WIZARDRY_DIR := $(MK_DIR)Wizardry
CONTENTS_DIR := $(MK_DIR)Contents
GAMEDATA_DIR := $(MK_DIR)Data

HACK_DIRS := $(CONFIG_DIR) $(WIZARDRY_DIR) $(CONTENTS_DIR) $(GAMEDATA_DIR) $(TEXTS_DIR)

all:
	@$(MAKE) pre_build	|| exit 1
	@$(MAKE) chax		|| exit 1
	@$(MAKE) post_chax	|| exit 1

include Contents/contents.mk

CACHE_DIR := $(MK_DIR).cache_dir
$(shell mkdir -p $(CACHE_DIR) > /dev/null)

CLEAN_FILES :=
CLEAN_DIRS  := $(CACHE_DIR) .release_dir $(shell find -name __pycache__)
CLEAN_BUILD :=

# =========
# = Tools =
# =========

ifeq ($(OS),Windows_NT)
	$(error "need linux environment, abort)
endif

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

ifeq ($(CONFIG_LONG_CALL), 1)
	LYN_LONG_CALL := -longcalls
	GCC_LONG_CALL := -mlong-calls
else
	LYN_LONG_CALL :=
	GCC_LONG_CALL :=
endif

PREFIX  ?= arm-none-eabi-
CC      := $(PREFIX)gcc
AS      := $(PREFIX)as
OBJCOPY := $(PREFIX)objcopy

EA_DIR            := $(TOOL_DIR)/EventAssembler
EA                := $(EA_DIR)/ColorzCore
PARSEFILE         := $(EA_DIR)/Tools/ParseFile
PNG2DMP           := $(EA_DIR)/Tools/Png2Dmp
COMPRESS          := $(EA_DIR)/Tools/compress
LYN               := $(EA_DIR)/Tools/lyn $(LYN_LONG_CALL)
EA_DEP            := $(EA_DIR)/ea-dep

LYN_PROTECTOR := $(TOOL_DIR)/scripts/lynjump-protector.sh
LYN_DETECTOR  := $(TOOL_DIR)/scripts/lynjump-detector.sh

GRIT := $(DEVKITPRO)/tools/bin/grit
LZSS := $(DEVKITPRO)/tools/bin/gbalzss

GRITLZ77ARGS      := -gu 16 -gzl -gB 4 -p! -m! -ft bin -fh!
GRIT4BPPARGS      := -gu 16 -gB 4 -p! -m! -ft bin -fh!
GRIT2BPPARGS      := -gu 16 -gb -gB 2 -p! -m! -ft bin -fh!
GRITPALETTEARGS	  := -g! -m! -p -ft bin -fh!
MAPPALETTEARGS    := -pn 160
BTLPALETTEARGS    := -pn 80

# ========
# = Main =
# ========

PRE_BUILD ?=
chax: $(FE8_CHX)

EA_FLAG := A FE8

ifeq ($(CONFIG_EA_WERR), 1)
	EA_FLAG += -werr
endif

$(FE8_CHX): $(MAIN) $(FE8_GBA) $(FE8_SYM) $(shell $(EA_DEP) $(MAIN) -I $(EA_DIR) --add-missings)
	@echo "[GEN]	$@"
	@cp -f $(FE8_GBA) $(FE8_CHX)
	@$(EA) $(EA_FLAG) -input:$(MAIN) -output:$(FE8_CHX) --nocash-sym || { rm -f $(FE8_CHX); exit 1; }

CHAX_SYM := $(FE8_CHX:.gba=.sym)
CHAX_REFS := $(FE8_CHX:.gba=.ref.s)
CHAX_REFE := $(FE8_CHX:.gba=.ref.event)
CHAX_DIFF := $(FE8_CHX:.gba=.bsdiff)

post_chax: $(CHAX_DIFF)

$(CHAX_DIFF): $(FE8_CHX)
	@echo "[SEC]	Lyn-jump detection..."
	@$(LYN_DETECTOR) || exit 1
	@echo "[SEC]	Lyn-jump detection passed"

ifeq ($(CONFIG_RELEASE_COMPILATION), 1)

	@echo "[GEN]	$(CHAX_REFS)"
	@echo  '@ Auto generated at $(shell date "+%Y-%m-%d %H:%M:%S")' > $(CHAX_REFS)
	@cat $(TOOL_DIR)/scripts/refs-preload.txt >> $(CHAX_REFS)
	@nm $(EXT_REF:.s=.o) | python3 $(TOOL_DIR)/scripts/nm2refs.py >> $(CHAX_REFS)
	@nm $(RAM_REF:.s=.o) | python3 $(TOOL_DIR)/scripts/nm2refs.py >> $(CHAX_REFS)
	@python3 $(TOOL_DIR)/scripts/sym2refs.py $(CHAX_SYM) >> $(CHAX_REFS)

	@echo "[GEN]	$(CHAX_REFE)"
	@echo '// Auto generated at $(shell date "+%Y-%m-%d %H:%M:%S")' > $(CHAX_REFE)
	@nm $(EXT_REF:.s=.o) | python3 $(TOOL_DIR)/scripts/nm2refe.py >> $(CHAX_REFE)
	@nm $(RAM_REF:.s=.o) | python3 $(TOOL_DIR)/scripts/nm2refe.py >> $(CHAX_REFE)
	@echo "PUSH" >> $(CHAX_REFE)
	@python3 $(TOOL_DIR)/scripts/sym2refe.py $(CHAX_SYM) >> $(CHAX_REFE)
	@echo "POP" >> $(CHAX_REFE)

	@nm $(EXT_REF:.s=.o) | python3 $(TOOL_DIR)/scripts/nm2sym.py >> $(CHAX_SYM)
	@nm $(RAM_REF:.s=.o) | python3 $(TOOL_DIR)/scripts/nm2sym.py >> $(CHAX_SYM)

	@echo "[GEN]	$(CHAX_DIFF)"
	@bsdiff $(FE8_GBA) $(FE8_CHX) $(CHAX_DIFF)
endif

	@cat $(FE8_SYM) >> $(CHAX_SYM)
	@cat $(CHAX_SYM) | python3 $(TOOL_DIR)/scripts/sym_modify.py | sponge $(CHAX_SYM)
	@echo "Done!"

CLEAN_FILES += $(FE8_CHX)  $(CHAX_SYM) $(CHAX_REFS) $(CHAX_REFE) $(CHAX_DIFF)

# ============
# = Wizardry =
# ============

INC_DIRS := include $(LIB_DIR)/include
INC_FLAG := $(foreach dir, $(INC_DIRS), -I $(dir))

ARCH    := -mcpu=arm7tdmi -mthumb -mthumb-interwork
CFLAGS  := $(ARCH) $(INC_FLAG) -Wall -Wextra -Werror -Wno-unused-parameter -O2 -mtune=arm7tdmi $(GCC_LONG_CALL)
ASFLAGS := $(ARCH) $(INC_FLAG)

CDEPFLAGS = -MMD -MT "$*.o" -MT "$*.asm" -MF "$(CACHE_DIR)/$(notdir $*).d" -MP
SDEPFLAGS = --MD "$(CACHE_DIR)/$(notdir $*).d"

LYN_REF := $(EXT_REF:.s=.o) $(RAM_REF:.s=.o) $(FE8_REF)

%.lyn.event: %.o $(LYN_REF) $(FE8_SYM)
	@echo "[LYN]	$@"
	@$(LYN) $< $(LYN_REF) > $@
	@$(LYN_PROTECTOR) $@ $(FE8_SYM) >> $@

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
TEXTS_DIR   := $(CONTENTS_DIR)/Texts
TEXT_SOURCE := $(shell find $(TEXTS_DIR) -type f -name '*.txt')

export TEXT_DEF := $(TEXTS_DIR)/build/msgs.h

text: $(TEXT_DEF)
PRE_BUILD += text

$(TEXT_DEF): $(TEXT_SOURCE)
	@$(MAKE) -C $(TEXTS_DIR)

CLEAN_BUILD += $(TEXTS_DIR)

# ============
# = Spritans =
# ============

PNG_FILES := $(shell find $(HACK_DIRS) -type f -name '*.png')
TSA_FILES := $(shell find $(HACK_DIRS) -type f -name '*.tsa')

%.4bpp: %.png
	@echo "[GEN]	$@"
	@cd $(dir $<) && $(GRIT) $(notdir $<) $(GRIT4BPPARGS)
	@mv $(basename $<).img.bin $@

%.gbapal: %.png
	@echo "[GEN]	$@"
	@cd $(dir $<) && $(GRIT) $(notdir $<) $(GRITPALETTEARGS)
	@mv $(basename $<).pal.bin $@

%.lz: %
	@echo "[LZ ]	$@"
	@$(LZSS) e $< $@

%.lz77: %.png
	@echo "[LZ ]	$@"
	@cd $(dir $<) && $(GRIT) $< $(GRITLZ77ARGS)
	@mv $(basename $<).img.bin $@

CLEAN_FILES += $(PNG_FILES:.png=.gbapal) $(PNG_FILES:.png=.4bpp) $(PNG_FILES:.png=.4bpp.lz)
CLEAN_FILES += $(PNG_FILES:.png=.lz77)
CLEAN_FILES += $(TSA_FILES:.tsa=.tsa.lz)

%.img.bin %.map.bin %.pal.bin: %.png
	@echo "[GEN]	$@"
	@$(GRIT) $< -gB 4 -gzl -m -mLf -mR4 -mzl -pn 16 -ftb -fh! -o $@

CLEAN_FILES += $(PNG_FILES:.png=.img.bin) $(PNG_FILES:.png=.map.bin) $(PNG_FILES:.png=.pal.bin)

# ============
# = EfxAnims =
# ============

EFX_ANIM_DIR := $(MK_DIR)Contents/EfxAnim
EFX_ANIMTOR  := python3 $(EFX_ANIM_DIR)/Scripts/efx-anim-creator.py

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

CLEAN_BUILD += $(EFX_ANIM_DIR)
# CLEAN_FILES += $(EFX_SCR_DEPS) $(EFX_TARGET)

# =======
# = GFX =
# =======

GFX_DIR     := $(MK_DIR)Contents/Gfx
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

ENUM2H := $(TOOL_DIR)/scripts/enum2h.py
ENUM2C := $(TOOL_DIR)/scripts/enum2combo.py

SKILLS_ENUM_DIR  := $(MK_DIR)include/constants
SKILLS_COMBO_DIR := $(MK_DIR)Patches

SKILLS_ENUM_SRC := $(SKILLS_ENUM_DIR)/skills-equip.enum.txt
SKILLS_ENUM_SRC += $(SKILLS_ENUM_DIR)/skills-others.enum.txt
SKILLS_ENUM_SRC += $(SKILLS_ENUM_DIR)/skills-item.enum.txt

SKILLS_ENUM_HEADER := $(SKILLS_ENUM_DIR)/skills.h

enum: $(SKILLS_ENUM_HEADER)

$(SKILLS_ENUM_HEADER): $(SKILLS_ENUM_SRC)
	@echo "[GEN]	$(SKILLS_ENUM_HEADER)"
	@echo "#pragma once" > $(SKILLS_ENUM_HEADER)
ifeq ($(CONFIG_CI_NO_SKILL_TEST), 0)
	@python3 $(ENUM2H) 0x000 $(SKILLS_ENUM_DIR)/skills-equip.enum.txt 	>> $(SKILLS_ENUM_HEADER)
	@python3 $(ENUM2H) 0x100 $(SKILLS_ENUM_DIR)/skills-others.enum.txt  >> $(SKILLS_ENUM_HEADER)
	@python3 $(ENUM2H) 0x300 $(SKILLS_ENUM_DIR)/skills-item.enum.txt    >> $(SKILLS_ENUM_HEADER)
	@python3 $(ENUM2C) 0x000 $(SKILLS_ENUM_DIR)/skills-equip.enum.txt 	>  $(SKILLS_COMBO_DIR)/combo.skills_equip.txt
	@python3 $(ENUM2C) 0x100 $(SKILLS_ENUM_DIR)/skills-others.enum.txt  >  $(SKILLS_COMBO_DIR)/combo.skills_others.txt
	@python3 $(ENUM2C) 0x300 $(SKILLS_ENUM_DIR)/skills-item.enum.txt    >  $(SKILLS_COMBO_DIR)/combo.skills_item.txt
	@python3 $(ENUM2C) 0x000 $(SKILLS_ENUM_DIR)/skills-equip.enum.txt 	>  $(SKILLS_COMBO_DIR)/combo.skills.txt
	@python3 $(ENUM2C) 0x100 $(SKILLS_ENUM_DIR)/skills-others.enum.txt  >> $(SKILLS_COMBO_DIR)/combo.skills.txt
	@python3 $(ENUM2C) 0x300 $(SKILLS_ENUM_DIR)/skills-item.enum.txt    >> $(SKILLS_COMBO_DIR)/combo.skills.txt
else
	@echo "[WARNNING] ======================================"
	@echo "[WARNNING] ======================================"
	@echo "[WARNNING] Build on removing all skills for debug"
	@echo "[WARNNING] ======================================"
	@echo "[WARNNING] ======================================"
endif

PRE_BUILD += enum
CLEAN_FILES += $(SKILLS_ENUM_HEADER)
CLEAN_FILES += $(MK_DIR)Patches/combo.skills.txt
CLEAN_FILES += $(MK_DIR)Patches/combo.skills_equip.txt
CLEAN_FILES += $(MK_DIR)Patches/combo.skills_others.txt
CLEAN_FILES += $(MK_DIR)Patches/combo.skills_item.txt

# =============
# = PRE-BUILD =
# =============
pre_build: $(PRE_BUILD)

# ==============
# = MAKE CLEAN =
# ==============

# misc
CLEAN_FILES += $(shell find $(HACK_DIRS) -type f -name '*.EXPERIMENTAL-checkpatch-fixes')

clean_basic:
	@rm -f $(CLEAN_FILES)
	@rm -rf $(CLEAN_DIRS)

clean:
	@for i in $(CLEAN_BUILD); do if test -e $$i/makefile ; then $(MAKE) -f $$i/makefile clean || { exit 1;} fi; done;
	@$(MAKE) clean_basic
	@echo "Kernel cleaned .."

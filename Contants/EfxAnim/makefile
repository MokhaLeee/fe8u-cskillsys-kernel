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

ifeq ($(strip $(DEVKITPRO)),)
	$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitpro)
endif

GRIT := $(DEVKITPRO)/tools/bin/grit$(EXE)

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

EFX_ANIMTOR := $(PYTHON3) $(mkfile_dir)Scripts/efx-anim-creator.py

EFX_SCRIPTS := $(shell find ./ -type f -name '*.efx.txt')
EFX_TARGET  := $(EFX_SCRIPTS:.efx.txt=.efx.event)

all: $(EFX_TARGET)

%.efx.event: %.efx.txt
	@echo "[GEN]	$@"
	@$(EFX_ANIMTOR) $< > $@

%.efx.txt.d: %.efx.txt
	@echo -n "$(patsubst %.efx.txt, %.efx.event, $<): " > $@
	@$(EFX_ANIMTOR) $< --list-files >> $@

CLEAN_FILES += $(EFX_TARGET)

EFX_SCR_DEPS := $(EFX_SCRIPTS:.efx.txt=.efx.txt.d)
include $(EFX_SCR_DEPS)

CLEAN_FILES += $(EFX_SCR_DEPS)

# Grit related
%.img.bin %.map.bin %.pal.bin: %.png
	@echo "[GEN]	$@"
	@$(GRIT) $< -gB 4 -gzl -m -mLf -mR4 -mzl -pn 16 -ftb -fh! -o $@

PNG_FILES := $(shell find ./ -type f -name '*.png')
CLEAN_FILES += $(PNG_FILES:.png=.img.bin) $(PNG_FILES:.png=.map.bin) $(PNG_FILES:.png=.pal.bin)

clean:
	rm -f $(CLEAN_FILES)

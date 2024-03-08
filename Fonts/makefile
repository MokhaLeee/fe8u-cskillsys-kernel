ifeq ($(strip $(DEVKITPRO)),)
	$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitpro)
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

GRIT := $(DEVKITPRO)/tools/bin/grit$(EXE)

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

GLYPH_GENERATOR := $(PYTHON3) $(mkfile_dir)Scripts/glyph-installer-generator.py
GLYPH_INSTALLER := $(mkfile_dir)GlyphInstaller.event

GLYPH_DEPS := $(mkfile_dir)FontList.txt

$(GLYPH_INSTALLER): $(GLYPH_DEPS)
	@echo "[GEN]	$@"
	@$(GLYPH_GENERATOR) -i $<

CLEAN_FILES += $(GLYPH_INSTALLER)

%_font.img.bin: %_font.png
	@echo "[GEN]	$@"
	@$(GRIT) $< -gB2 -p! -tw16 -th16 -ftb -fh! -o $@

CFILES := $(shell find $(mkfile_dir)Hacks -type f -name '*.c')
CLEAN_FILES += $(CFILES:.c=.o) $(CFILES:.c=.lyn.event)
CLEAN_DIRS  += $(mkfile_dir)build

clean:
	rm -f $(CLEAN_FILES)
	rm -rf $(CLEAN_DIRS)

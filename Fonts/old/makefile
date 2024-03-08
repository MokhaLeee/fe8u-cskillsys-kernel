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

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

GLYPH_GEN := $(PYTHON3) $(mkfile_dir)Scripts/utf8-glyph-generator.py
PNG2FONT  := $(PYTHON3) $(mkfile_dir)Scripts/Png2Font.py

CLEAN_DIRS  := $(mkfile_dir)Scripts/__pycache__
CLEAN_FILES :=

# Fonts
FONT_DIR := $(mkfile_dir)FontData
FONT_GLYPHS := $(shell find $(FONT_DIR) -type f -name 'Font*.png')
FONT_TARGET := $(FONT_GLYPHS:.png=.fefont)

%.fefont: %.png
	@echo "[GEN]	$@"
	@$(PNG2FONT) $<

CLEAN_FILES += $(FONT_TARGET)

# EA installer

INSTALL_GLYPH := $(mkfile_dir)InstallGlyph.event
ROM := $(mkfile_dir)../../fe8.gba

$(INSTALL_GLYPH): $(FONT_GLYPHS) $(ROM)
	@echo "[GEN]	$@"
	@$(GLYPH_GEN) $(ROM)

CLEAN_DIRS  += $(mkfile_dir)FontInstaller
CLEAN_FILES += $(INSTALL_GLYPH)

clean:
	rm -rf $(CLEAN_DIRS)
	rm -f $(CLEAN_FILES)

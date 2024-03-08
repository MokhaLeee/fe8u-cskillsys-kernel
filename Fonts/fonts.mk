# =========
# = Glyph =
# =========

FONT_DIR := $(MK_DIR)/Fonts

GLYPH_INSTALLER := $(FONT_DIR)/GlyphInstaller.event
GLYPH_DEPS := $(FONT_DIR)/FontList.txt

font: $(GLYPH_INSTALLER)
PRE_BUILD += font

$(GLYPH_INSTALLER): $(GLYPH_DEPS)
	@$(MAKE) -C $(FONT_DIR)

%_font.img.bin: %_font.png
	@echo "[GEN]	$@"
	@$(GRIT) $< -gB2 -p! -tw16 -th16 -ftb -fh! -o $@

CLEAN_BUILD += $(FONT_DIR)

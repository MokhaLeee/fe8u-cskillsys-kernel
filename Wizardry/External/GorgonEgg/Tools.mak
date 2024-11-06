
# Folders

export TEMPLATEDIR = template
export CONFIGDIR   = config
export OUTPUTDIR   = output
export BACKUPDIR   = backups

export NATIVE_MODULESDIR = source/modules
export CUSTOM_MODULESDIR = custom/modules


# Tools

export create_backup   := $(PYTHON) $(TOOLSDIR)/create_backup.py
export read_config     := $(PYTHON) $(TOOLSDIR)/read_config.py
export convert_tilemap := $(PYTHON) $(TOOLSDIR)/convert_tilemap.py


# Misc. recipes

%.c: %.tmx
	@$(convert_tilemap) $< $@

%.c %.h &: %.tmx
	@$(convert_tilemap) $< $*.c $*.h

%.4bpp: %.png
	@$(png2dmp) $< -o $@

%.pal: %.png
	@$(png2dmp) $< -po $@

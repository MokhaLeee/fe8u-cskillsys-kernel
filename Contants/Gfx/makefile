mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir  := $(dir $(mkfile_path))
build_dir   := $(shell pwd)

GFX_GENERATOR := $(mkfile_dir)Scripts/gfx-installer.sh

SOURCE_DIR  := $(mkfile_dir)Sources/
ALL_SOURCES := $(shell find $(SOURCE_DIR)/ -type f -name '*.png')

GFX_HEADER    ?= $(mkfile_dir)GfxDefs.h
GFX_INSTALLER ?= $(mkfile_dir)GfxInstaller.event

$(GFX_HEADER): $(ALL_SOURCES)
	@echo "[GEN]	$@"
	@cd $(dir $(mkfile_dir)..) && $(GFX_GENERATOR) $(GFX_HEADER) $(GFX_INSTALLER) && cd $(build_dir)

clean:
	rm -f $(GFX_HEADER) $(GFX_INSTALLER)

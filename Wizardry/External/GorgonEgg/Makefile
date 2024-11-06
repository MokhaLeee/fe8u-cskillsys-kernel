
.SUFFIXES:

include Prerequisites.mak
include Tools.mak
include Code.mak

build: backup clean_output template config deps
ifeq (,$(findstring s,$(firstword -$(MAKEFLAGS))))
	@echo GORGON-EGG built successfully!
endif

.PHONY: build template backup clean_output config clean
.DEFAULT_GOAL := build


# Pre-build output generation

$(OUTPUTDIR):
	@mkdir -p $(OUTPUTDIR)


# Fixed input file copying

TEMPLATE_FILES := $(foreach f,$(wildcard $(TEMPLATEDIR)/*),$(notdir $(f)))

template: | $(OUTPUTDIR)
	@for f in $(TEMPLATE_FILES); do cp -f $(TEMPLATEDIR)/$$f $(OUTPUTDIR)/$$f; done


# Pre-build output cleaning

clean_output:
	@$(RM) -r $(OUTPUTDIR)/*


# Config reading

CONFIG                := $(CONFIGDIR)/Config.xml
GENERATED_DEFINITIONS := $(OUTPUTDIR)/GeneratedDefinitions.h
GENERATED_INSTALLER   := $(OUTPUTDIR)/GeneratedInstaller.event

config $(GENERATED_DEFINITIONS) $(GENERATED_INSTALLER) &: $(CONFIG)
	@$(read_config) $(CONFIG) $(OUTPUTDIR) $(NATIVE_MODULESDIR) $(CUSTOM_MODULESDIR)


# Automatic backup generation

# This will keep backups of the config and output of the last 5 builds.
# Second line taken from https://stackoverflow.com/a/34862475

backup $(BACKUPDIR) &: | $(OUTPUTDIR)
	@$(create_backup) $(BACKUPDIR) $(CONFIG) $(OUTPUTDIR)
	@(cd $(BACKUPDIR) && ls -tp | grep -v '/$$' | tail -n +6 | tr '\n' '\0' | xargs -0 rm -f --)


# Dependency generation

INSTALLER := $(OUTPUTDIR)/Installer.event

deps:
	@$(MAKE) -s $(shell $(ea-dep) $(INSTALLER) --add-missings)


# Cleaning

clean::
	@$(RM) -r $(OUTPUTDIR)/*

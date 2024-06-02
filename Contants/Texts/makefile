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

TEXT_PROCESS := $(PYTHON3) $(mkfile_dir)Scripts/text-process-c.py
PARSEFILE    := $(PYTHON3) $(mkfile_dir)Scripts/parsefile-utf8.py

TEXT_DIR ?= $(mkfile_dir)

TEXT_MAIN   := $(TEXT_DIR)Source/TextMain.txt
TEXT_SOURCE := $(shell find $(TEXT_DIR) -type f -name '*.txt')

TEXT_EVENT  := $(TEXT_DIR)TextInstaller.event
TEXT_DEF    ?= $(TEXT_DIR)TextDefinitions.h

all: $(TEXT_DEF)

$(TEXT_EVENT) $(TEXT_DEF): $(TEXT_MAIN) $(TEXT_SOURCE)
	@echo "[TXT]	$@"
	@$(TEXT_PROCESS) $(TEXT_MAIN) --installer $(TEXT_EVENT) --definitions $(TEXT_DEF)

%.fetxt.dmp: %.fetxt
	@$(PARSEFILE) -i $< -o $@ > /dev/null

clean:
	@rm -rf $(mkfile_dir)Source/.TextEntries
	@rm -f $(TEXT_EVENT) $(TEXT_DEF)

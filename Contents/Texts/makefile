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

TEXT_PROCESS := $(PYTHON3) $(mkfile_dir)Scripts/textprocess.py

TEXT_DIR ?= $(mkfile_dir)

TEXT_MAIN   := $(TEXT_DIR)text-main.txt
TEXT_DEFS   := $(TEXT_DIR)textdefs.txt
TEXT_SOURCE := $(shell find $(TEXT_DIR) -type f -name '*.txt')

TEXT_HEADER ?= $(TEXT_DIR)msg_data.h
MSG_LIST    := $(TEXT_DIR)msg_data.c

all: $(TEXT_HEADER)

$(TEXT_HEADER): $(TEXT_SRC) $(TEXT_DEFS)
	@echo "[GEN]	$@"
	@$(TEXT_PROCESS) $(TEXT_MAIN) $(TEXT_DEFS) $(MSG_LIST) $(TEXT_HEADER) utf8

clean:
	@rm -rf $(mkfile_dir)Source/.TextEntries
	@rm -f $(MSG_LIST) $(TEXT_HEADER)

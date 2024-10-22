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

TEXT_PROCESS := $(PYTHON3) $(mkfile_dir)Scripts/textprocess-chax.py

TEXT_DIR ?= $(mkfile_dir)

TEXT_MAIN   := $(TEXT_DIR)text-main.txt
TEXT_DEFS   := $(TEXT_DIR)textdefs.txt
TEXT_SOURCE := $(shell find $(TEXT_DIR) -type f -name '*.txt')

MSG_HEADER ?= $(TEXT_DIR)build/msg_data.h
MSG_DATA   := $(TEXT_DIR)build/msg_data.c
MSG_TABLE  := $(TEXT_DIR)build/msg_table.c

all: $(MSG_HEADER)

$(MSG_HEADER): $(TEXT_SRC) $(TEXT_DEFS)
	@echo "[GEN]	$@"
	@install -d $(TEXT_DIR)build
	@$(TEXT_PROCESS) $(TEXT_MAIN) $(TEXT_DEFS) $(MSG_TABLE) $(MSG_DATA) $(MSG_HEADER) utf8

clean:
	@rm -rf $(TEXT_DIR)build
	@rm -f $(MSG_DATA) $(MSG_HEADER)

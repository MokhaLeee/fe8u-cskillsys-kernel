ifeq ($(OS),Windows_NT)
  EXE := .exe
else
  EXE :=
endif

LIB_DIR := $(shell dirname $(lastword $(MAKEFILE_LIST)))

FE8_REF := $(LIB_DIR)/reference/fireemblem8.ref.s
INC_DIRS += $(LIB_DIR)/include

INC_FLAG := $(foreach dir, $(INC_DIRS), -I $(dir))

PREFIX ?= arm-none-eabi-
CC  := $(PREFIX)gcc
AS  := $(PREFIX)as

ARCH    := -mcpu=arm7tdmi -mthumb -mthumb-interwork
CFLAGS  := $(ARCH) $(INC_FLAG) -Wall -Wextra -Wno-unused-parameter -O2 -mtune=arm7tdmi -mlong-calls
ASFLAGS := $(ARCH) $(INC_FLAG)

%.o: %.s
	$(AS) $(ASFLAGS) -I $(dir $<) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -g -c $< -o $@

LYN ?= ~/EventAssembler/Tools/lyn$(EXE)
LYN_REF := $(FE8_REF:.s=.o)

%.lyn.event: %.o
	$(LYN) $< $(LYN_REF) > $@

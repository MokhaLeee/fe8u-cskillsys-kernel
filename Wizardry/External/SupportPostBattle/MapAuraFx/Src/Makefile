.SUFFIXES:
.PHONY:

# Making sure devkitARM exists and is set up.
ifeq ($(strip $(DEVKITARM)),)
	$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

# Including devkitARM tool definitions
include $(DEVKITARM)/base_tools

# defining path of lyn
LYN := $(abspath D:\devkitPro\lyn.exe)

# setting up compilation flags
ARCH   := -mcpu=arm7tdmi -mthumb -mthumb-interwork -I "D://devkitpro/FE-CLib/include"
CFLAGS := $(ARCH) -Wall -Os -mlong-calls -mtune=arm7tdmi

# C to ASM rule
%.asm: %.c
	$(CC) $(CFLAGS) -S $< -o $@ -fverbose-asm

# C to OBJ rule
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ASM to OBJ
%.o: %.s *.s
	$(AS) $(ARCH) $< -o $@

# ASM to OBJ
%.o: %.asm *.asm
	$(AS) $(ARCH) $< -o $@
	
# OBJ to DMP rule
%.dmp: %.o
	$(OBJCOPY) -S $< -O binary $@

# OBJ to LYN.EVENT rule
%.lyn.event: %.o
	$(MAKE) $(dir $<)Definitions.o
	$(LYN) $< $(dir $<)Definitions.o > $@
	rm $(dir $<)Definitions.o

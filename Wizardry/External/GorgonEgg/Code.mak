
# Code helpers

LYN_REFERENCE := $(CLIBDIR)/reference/FE8U-20190316.o output/InternalDefinitions.o

INCLUDE_DIRS := $(CLIBDIR)/include $(OUTPUTDIR)
INCFLAGS     := $(foreach dir,$(INCLUDE_DIRS), -I "$(dir)")

# Assembly/compilation flags
ARCH    := -mcpu=arm7tdmi -mthumb -mthumb-interwork
CFLAGS  := $(ARCH) $(INCFLAGS) -Wall -Os -mtune=arm7tdmi -ffreestanding -fomit-frame-pointer -mlong-calls
ASFLAGS := $(ARCH) $(INCFLAGS)

%.dmp: %.o
	@$(OBJCOPY) -S "$<" -O binary "$@"

%.o: %.c
	@$(CC) $(CFLAGS) -g -c "$<" -o "$@"

%.lyn.event: %.o $(LYN_REFERENCE)
	@$(lyn) "$<" $(LYN_REFERENCE) > "$@" || ($(RM) "$@" && false)

%.asm: %.c
	@$(CC) $(CFLAGS) -S "$<" -o "$@" -fverbose-asm

%.o: %.s
	@$(AS) $(ASFLAGS) -I "$(dir $<)" "$<" -o "$@"


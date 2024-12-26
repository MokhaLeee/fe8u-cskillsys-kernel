It is recommanded to put your own ***C*** or ***ASM*** hacks to [Wizardry](../Wizardry/) directory.

For ***C*** hack, you should include [common-chax.h](../include/common-chax.h) at head, which may introduce decomp c-libs and other basic definitions.

```c
#include "common-chax.h"
```

For ***ASM*** hack, you need name it as ***\<\*.s\>*** rather than ~~***\<\*.asm\>***~~, you also need to include [macros.inc](../include/macros.inc).

```assembly
.thumb
.include "macros.inc"
```

After you have done, dirctly put the ***\<\*.lyn.event\>*** file to your buildfile, and put the event included by [wizardry.event](../Wizardry/Wizardry.event). Kernel may automatically generate the dependence and compile the source file to ***\<\*.lyn.event\>*** via *ea_dep* and *lyn*.

## Debugging

For developers, it is recommanded to uncommend `CONFIG_USE_DEBUG` in [config-debug.h](../include/Configs/config-debug.h#L5). This operation may enable debug system, and make kernel print log to tty debug window on emulator.

Once enabled, you may use non-standard kernel stdio functions defined in [debug-kit.h](../include/debug-kit.h) (which is included in [common-chax.h](../include/common-chax.h#L5)), such as `Print()`, `Printf()`, `Assert()` to insert debug hooks in your own functions.

## BL range
Now, the kernel allows programs to run simultaneously in both the in-BL range space and other spaces. Functions within the in-BL range can directly use the `BL` instruction, which creates differences in the way these functions are compiled.

All `%.c` and `%.s` files located in the [Wizardry](./Wizardry/) directory will be compiled according to the in-BL range strategy, and you need to place all functions in the [Wizardry](./Wizardry/) directory within the in-BL range area.

```makefile
Wizardry/%.o: Wizardry/%.c
	$(CC) $(CFLAGS) $(CDEPFLAGS) -g -c $< -o $@

Wizardry/%.asm: Wizardry/%.c
	$(CC) $(CFLAGS) $(CDEPFLAGS) -S $< -o $@ -fverbose-asm

Wizardry/%.lyn.event: Wizardry/%.o
	$(LYN) $< $(LYN_REF) > $@
```

Others will be compiled with the long-call method, and it's recommended to place them at non-bl range spcae, such as `0xB2A604` or `0x01000000`:

```makefile
%.lyn.event: %.o
	$(LYN) -longcalls $< $(LYN_REF) > $@

%.o: %.c
	$(CC) $(CFLAGS) -mlong-calls $(CDEPFLAGS) -g -c $< -o $@

%.asm: %.c
	$(CC) $(CFLAGS) -mlong-calls $(CDEPFLAGS) -S $< -o $@ -fverbose-asm
```

## Math

Kernel offered a high performance dev/mod function set and recommanded to replace unstable `/` and `%`:

```c
// kernel-lib.h
u32 k_udiv(u32 a, u32 b);
u32 k_umod(u32 a, u32 b);
```

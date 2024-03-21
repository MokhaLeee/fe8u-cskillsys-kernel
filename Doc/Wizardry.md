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

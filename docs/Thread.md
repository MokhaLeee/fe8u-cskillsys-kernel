Thanks to the discussion with [luckytyphlosion](https://github.com/luckytyphlosion), the kernel now achieves basic dual-thread switching in the `VBlankIntrWait` process by manually triggering a context switch.

## SYNOPSIS

```c
#include <kthread.h>

typedef void (*thread_task_func)(void);
void CreateSubThread(thread_task_func func);
```

Insert a sub thread to call for the task function during main routine is waiting for VBlank interrupt. CSkillsys kernel may automatically switch back to main routine during VBlank IRQ handler.

- Note1: The current implementation allows only one active task. New tasks replace the old.
- Note2: Due to the GBA constrained processing power, frequent context switches degrade performance. The `YieldSubThread()` API provides manual yield capability, forcing switches to main thread prior to VBlank interrupts and preventing visual lag.
- Note3: The IRQ handler's code modifications have compressed its trailing memory area. This space is no longer safe for reuse—do not treat it as free space.
- Note4: The stack is anchored at the tail of `FreeRamSpace2`, which shares space with the heap (starting from the region's head). Excessive stack usage in threads may lead to heap-stack collisions.

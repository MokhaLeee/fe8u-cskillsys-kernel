# ROM space

ROM space distribution is configured in [config-memmap.h](../include/Configs/config-memmap.h)

| Address   | Size    | Usage
| -------   | ------- | -----
| 0x0B2A604 | 0xD5DFC | Kernel
| 0x0EFB2E0 | 0xE4D20 | Fonts
| 0x1000000 | ---     | ***reserved for DEMO***

For kernel, we mainly use free-space starting from `0xB2A604`.

Free space at `0x0EFB2E0` is used to [insert UTF8 characters](../Fonts/Fonts.event) for multi-languge support, but that space is free on default. For more infomation, see [Multi-Language.md](./Multi-Language.md).

In order to co-work on FEBuilderGBA and make DEMO based on kernel, we need also put some inportant data at the fixed location:

### a). Magic pattern

[A serial of characters](../main.event#L11) is set at the head of kernel free-space (`0xB2A604`), which can make it as an identifier for [FEBuilder patch](../Patches/PATCH_SkillInfo.txt#L4).

### b). Pointer list

There is a pointer list after magic pattern, start at `0xB2A614` with size = `0x400`. Both wizardry c-hacks and FEBuilder Patches may find the data via such list, so that you can expand the data via FEB.

### c). Text table

TextTable is repointed at a fixed location behind the pointer list, [0xB2AA14](../include/Configs/config-memmap.h#38) with [size = `0x1300 * sizeof(uintptr_t)`](../Repoint/RepointText/RepointText.event#L9) free space allocated. If you want to make DEMO based on kernel, it is recommanded to directly use the kernel allocated text table rather than re-repoint it by such operations:

```c
#include "fe8-kernel-beta.ref.event" // auto generated by kernel
#define TextTable NewTextTable
#include "Tools/Tool Helpers.txt"    // EventAssembler helper
```


# RAM space

RAM space distribution is configured in [usr-defined.s](../include/Configs/usr-defined.s)

| Address    | Size    | Usage
| -------    | ------  | -----
| 0x02026E30 | 0x2028  | Kernel
| 0x0203F000 | 0x1000  | ***reserved for DEMO***

Since all source codes are all compiled at once, CHAX can offer a better Free-RAM-Space control method.
Free-RAM-Space, unused memory from vanilla is collected by wizardries and now can be refered by [StanH's DOC](https://github.com/StanHash/DOC/blob/master/FREE-RAM-SPACE.md). Here we mainly use space start at `0x02026E30` with size `0x2028`, which is the debug print buffer in vanilla (and unused). They are all defined in [usr-defined.s](../include/Configs/usr-defined.s).

In kernel, free-ram space is alloced from the bottom to the top:

```assembly
0x02026E30, FreeRamSpaceTop

<--- gKernelUsedFreeRamSpaceTop
<------------
<!used kernel space>
0x02028E58, FreeRamSpaceBottom
```

Developed should ensure that the free-ram space not overflowed, which means asseration `(gKernelUsedFreeRamSpaceTop > FreeRamSpaceTop)` should be valid. We have also add a detection for RAM space overflow, CHAX may auto detect on overflow error on [game-init](../Wizardry/Common/GameInitHook/source/GameInit.c#L14).

## Example

Here is an example to alloc ram spaces in kernel:

Suppose you want a 4 Byte RAM space (`u8 NewAlloc4Bytes[4]`)

1. Get into [usr-defined.ref.s](../include/Configs/usr-defined.s), find `gKernelUsedFreeRamSpaceTop` definition, assume that the old allocation is:
```
SET_DATA gKernelUsedFreeRamSpaceTop, some_used_space
```

2. Insert new allocation to the top:
```
SET_DATA NewAlloc4Bytes, some_used_space - 4 @ since NewAlloc4Bytes is 4 Bytes
```

**<!> Make sure that the allocated space should be 32bits alligned**.

3. Update `gKernelUsedFreeRamSpaceTop`
```
SET_DATA gKernelUsedFreeRamSpaceTop, NewAlloc4Bytes
```

4. Declare such variable in your own C file, `extern u8 NewAlloc4Bytes[4];`

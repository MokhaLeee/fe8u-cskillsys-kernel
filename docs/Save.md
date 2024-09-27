```
Mokha's plan:
SAVE NAME       SAVE OFFSET     SAVE LENGTH
meta            0x00            0xD4
suspend         0xD4            0x3000
save1           0x30D4          0x19B0
save2           0x4A84          0x19B0
save3           0x6434          0x19B0
magic           0x7DE4          0x0008
link_arena      ------          0x0
extra_map       ------          0x0
```

SRAM space allocation is set in [SaveData](../Wizardry/Common/SaveData/data.event). Although the organization on data is similar to the public EMS patch, we have made more optimization to make it easier for users to use.

1. No need to define the offset in EmsChunk, now we just need to define the size we need, and then kernel may automatically calculate on physical address.

    Also, no need on worry of overflow. Kernel may detect on it and directly pannic on game-start if overflowed.

2. A set of hook functions are reserved so that users can decide the remaining space allocation by themselves.

    We have offered such APIs in [Reloc.event](../Data/Reloc.event) so that users may handler on their EMS function:

    ```c
    extern void (*const gpMsa_Saver )(u8 *dst, const u32 size);
    extern void (*const gpMsa_Loader)(u8 *src, const u32 size);
    extern void (*const gpMsu_Saver )(u8 *dst, const u32 size);
    extern void (*const gpMsu_Loader)(u8 *src, const u32 size);
    ```

    These hooks is set in [Reloc.event](../Data/Reloc.event), a [fixed pointer list](./Memory.md#L22), if a user has written his own functions (for example, `Demo_MsaSaver`, `Demo_MsaLoader`, `Demo_MsuSaver`, `Demo_MsuLoader`), he may insert them by:

    ```
    #include "fe8-kernel-060beta.ref.event" // this is dependent on release version
    PUSH
        ORG gpMsa_Saver
        POIN Demo_MsaSaver
        ORG gpMsa_Loader
        POIN Demo_MsaLoader
        ORG gpMsu_Saver
        POIN Demo_MsuSaver
        ORG gpMsu_Loader
        POIN Demo_MsuLoader
    POP
    ```

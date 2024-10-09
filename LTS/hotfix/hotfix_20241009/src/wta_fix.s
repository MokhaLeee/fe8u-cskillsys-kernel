    .INCLUDE "macro.inc"
    .SYNTAX UNIFIED

    .thumb

    @ hooked from 0xBA9104: Wizardry/Core/BattleSys/Source/WeaponTriangle.asm line:269
    ldrh r1, [r6, #6]
    ldr r3, [r3]

    push {r1}

    bl .Lblbxr3

    pop {r1}
    cmp r1, #0
    bne 1f
    movs r0, #1

1:
    ldr r1, =0x08BA910D
    bx r1

.Lblbxr3:
    bx r3

    .INCLUDE "macro.inc"
    .SYNTAX UNIFIED

    .thumb
    push {r4, lr}
    movs r4, r0

    ldr r3, =0x08BACE35 // new CheckBattleUnitLevelUp
    bl .Lblbxr3

    ldrb r0, [r4, #0xB]
    bl GetUnit

    movs r1, r4
    bl CheckBattleUnitStatCaps

    pop {r4, pc}

.Lblbxr3:
    bx r3

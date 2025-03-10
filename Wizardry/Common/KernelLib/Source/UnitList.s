    .INCLUDE "macros.inc"
    .SYNTAX UNIFIED

    .section .text

THUMB_FUNC_START JudgeUnitList
JudgeUnitList:
    ldr r3, _JudgeUnitList
    bx r3
.global _JudgeUnitList
.align 2, 0
_JudgeUnitList:
    .4byte ARM_UnitList + (.Lfun_judge - _ARM_UnitList_CopyStart)

THUMB_FUNC_START WriteUnitList
WriteUnitList:
    ldr r3, _WriteUnitList
    bx r3
.global _WriteUnitList
.align 2, 0
_WriteUnitList:
    .4byte ARM_UnitList + (.Lfun_write - _ARM_UnitList_CopyStart)

    .arm
    .section .rodata

.global _ARM_UnitList_CopyStart
_ARM_UnitList_CopyStart:

@ r0 = unit
@ r1 = ref
.Lfun_judge:
    @ pinfo
    ldr r2, [r0], #4
    ldr r3, [r1], #4
    cmp r3, r2
    bne 1f

    @ jinfo
    ldr r2, [r0], #4
    ldr r3, [r1], #4
    cmp r3, r2
    bne 1f

    @ level
    ldrb r2, [r0], #3
    ldrb r3, [r1], #1
    cmp r3, r2
    bne 1f

    @ uid
    ldrb r2, [r0], #0x13
    ldrb r3, [r1], #1
    cmp r3, r2
    bne 1f

    @ items
    ldrb r2, [r0], #2
    ldrb r3, [r1], #1
    cmp r3, r2
    bne 1f
    ldrb r2, [r0], #2
    ldrb r3, [r1], #1
    cmp r3, r2
    bne 1f
    ldrb r2, [r0], #2
    ldrb r3, [r1], #1
    cmp r3, r2
    bne 1f
    ldrb r2, [r0], #2
    ldrb r3, [r1], #1
    cmp r3, r2
    bne 1f
    ldrb r2, [r0], #2
    ldrb r3, [r1], #1
    cmp r3, r2
    bne 1f

    mov r0, #1
    bx lr

1:
    mov r0, #0
    bx lr

@ r0 = unit
@ r1 = ref
.Lfun_write:

    @ pinfo
    ldr r2, [r0], #4
    str r2, [r1], #4

    @ jinfo
    ldr r2, [r0], #4
    str r2, [r1], #4

    @ level
    ldrb r2, [r0], #3
    strb r2, [r1], #1

    @ uid
    ldrb r2, [r0], #0x13
    strb r2, [r1], #1

    @ items
    ldrb r2, [r0], #2
    strb r2, [r1], #1
    ldrb r2, [r0], #2
    strb r2, [r1], #1
    ldrb r2, [r0], #2
    strb r2, [r1], #1
    ldrb r2, [r0], #2
    strb r2, [r1], #1
    ldrb r2, [r0], #2
    strb r2, [r1], #1

    bx lr

.global _ARM_UnitList_CopyEnd
_ARM_UnitList_CopyEnd:

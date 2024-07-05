    .INCLUDE "macros.inc"
    .SYNTAX UNIFIED

    .section .rodata

.global _JudgeUnitList
_JudgeUnitList:
    .4byte ARM_UnitList + (.Lfun_judge - _ARM_UnitList_CopyStart)

.global _WriteUnitList
_WriteUnitList:
    .4byte ARM_UnitList + (.Lfun_write - _ARM_UnitList_CopyStart)

    .arm
    .section .text

    .global _ARM_UnitList_CopyStart
_ARM_UnitList_CopyStart:

@ r0 = unit
@ r1 = ref
.Lfun_judge:
    push {r4, lr}
    ldr r4, [r1]
    bl .Lfun_cksum_unit
    cmp r0, r4
    beq 1f

    mov r0, #0
    pop {r4}
    pop {r1}
    bx r1

1:
    mov r0, #1
    pop {r4}
    pop {r1}
    bx r1

@ r0 = unit
@ r1 = ref
.Lfun_write:
    push {r4, lr}
    mov r4, r1
    bl .Lfun_cksum_unit
    str r0, [r4]
    pop {r4}
    pop {r1}
    bx r1

.Lfun_cksum_unit:
    @ r0 = unit
    mov r2, r0
    mov r0, #0

    # self
    mov r1, r0
    eor r0, r0, r1

    # pinfo
    ldr r1, [r2, #0x0]
    eor r0, r0, r1

    # jinfo
    ldr r1, [r2, #0x4]
    eor r0, r0, r1

    # level
    ldrsb r1, [r2, #0x8]
    eor r0, r0, r1

    # items
    ldrb r1, [r2, #0x1E]
    eor r0, r0, r1
    ldrb r1, [r2, #0x20]
    eor r0, r0, r1
    ldrb r1, [r2, #0x22]
    eor r0, r0, r1
    ldrb r1, [r2, #0x24]
    eor r0, r0, r1
    ldrb r1, [r2, #0x26]
    eor r0, r0, r1

    mov pc, lr

    .global _ARM_UnitList_CopyEnd
_ARM_UnitList_CopyEnd:

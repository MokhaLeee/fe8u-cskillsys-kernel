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

    mov r2, #0          @ r2 = add_acc
    mov r3, #0          @ r3 = xor_acc

    # self
    mov r1, r0
    add r2, r2, r1
    eor r3, r3, r1

    # pinfo
    ldr r1, [r0, #0x0]
    add r2, r2, r1
    eor r3, r3, r1

    # jinfo
    ldr r1, [r0, #0x4]
    add r2, r2, r1
    eor r3, r3, r1

    # level
    ldrsb r1, [r0, #0x8]
    add r2, r2, r1
    eor r3, r3, r1

    # items
    ldrb r1, [r0, #0x1E]
    add r2, r2, r1
    eor r3, r3, r1
    ldrb r1, [r0, #0x20]
    add r2, r2, r1
    eor r3, r3, r1
    ldrb r1, [r0, #0x22]
    add r2, r2, r1
    eor r3, r3, r1
    ldrb r1, [r0, #0x24]
    add r2, r2, r1
    eor r3, r3, r1
    ldrb r1, [r0, #0x26]
    add r2, r2, r1
    eor r3, r3, r1

    add r0, r2, r3
    mov pc, lr

    .global _ARM_UnitList_CopyEnd
_ARM_UnitList_CopyEnd:

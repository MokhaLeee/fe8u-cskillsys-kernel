    .INCLUDE "macros.inc"
    .SYNTAX UNIFIED

    .section .rodata

.global _JudgeSkillViaListFast
_JudgeSkillViaListFast:
    .4byte ARM_SkillList + (.Lfun_judgeskillFast - _ARM_SkillList_CopyStart)

    .arm
    .section .rodata

.global _ARM_SkillList_CopyStart
_ARM_SkillList_CopyStart:

@ r0: gBattleActor or gBattleTarget
@ r1: skill index
.Lfun_judgeskillFast:
    ldr r2, .LSkillList
    ldr r3, .LgBattleActor
    sub r0, r3
    lsr r0, #1
    add r0, r2

    add r0, #0x10
    ldrb r3, [r0], #2
    lsl r3, #1
    add r2, r0, r3

1:
    cmp r2, r0
    beq 2f
    ldrh r3, [r0], #2
    cmp r1, r3
    beq 3f
    b 1b

2:
    mov r0, #0
    bx lr

3:
    mov r0, #1
    bx lr

.LSkillList:
    .4byte sSkillList + 0x40

.LgBattleActor:
    .4byte gBattleActor

.global _ARM_SkillList_CopyEnd
_ARM_SkillList_CopyEnd:

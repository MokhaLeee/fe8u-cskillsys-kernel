    .INCLUDE "macros.inc"
    .SYNTAX UNIFIED

    .section .text

THUMB_FUNC_START BattleFastSkillTester
BattleFastSkillTester:
    ldr r3, _BattleFastSkillTester
    bx r3
.global _BattleFastSkillTester
.align 2, 0
_BattleFastSkillTester:
    .4byte ARM_SkillList + (.Lfun_judgeskillFast - _ARM_SkillList_CopyStart)

    .arm
    .section .rodata

.global _ARM_SkillList_CopyStart
_ARM_SkillList_CopyStart:

@ r0: gBattleActor or gBattleTarget
@ r1: skill index
.Lfun_judgeskillFast:
    ldr r2, .LSkillFastList
    ldr r3, .LgBattleActor
    sub r0, r3
    add r0, r2

    @ list[idx / 8]
    lsr r2, r1, #3
    ldrb r0, [r0, r2]

    @ 1 << (idx % 8)
    and r3, r1, #7
    mov r1, #1
    lsl r1, r3

    tst r0, r1
    movne r0, #1
    moveq r0, #0

    bx lr

.LSkillFastList:
    .4byte sSkillFastList

.LgBattleActor:
    .4byte gBattleActor

.global _ARM_SkillList_CopyEnd
_ARM_SkillList_CopyEnd:

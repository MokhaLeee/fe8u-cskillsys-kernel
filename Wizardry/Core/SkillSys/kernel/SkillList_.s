@     .INCLUDE "macros.inc"
@     .SYNTAX UNIFIED

@     .section .rodata

@ .global _BattleSkillTester
@ _BattleSkillTester:
@     .4byte ARM_SkillList + (.Lfun_judgeskillFast - _ARM_SkillList_CopyStart)

@     .arm
@     .section .rodata

@ .global _ARM_SkillList_CopyStart
@ _ARM_SkillList_CopyStart:

@ @ r0: gBattleActor or gBattleTarget
@ @ r1: skill index
@ .Lfun_judgeskillFast:
@     ldr r2, .LSkillFastList
@     ldr r3, .LgBattleActor
@     sub r0, r3
@     add r0, r2

@     @ list[idx / 8]
@     lsr r2, r1, #3
@     ldrb r0, [r0, r2]

@     @ 1 << (idx % 8)
@     and r3, r1, #7
@     mov r1, #1
@     lsl r1, r3

@     tst r0, r1
@     movne r0, #1
@     moveq r0, #0

@     bx lr

@ .LSkillFastList:
@     .4byte sSkillFastList

@ .LgBattleActor:
@     .4byte gBattleActor

@ .global _ARM_SkillList_CopyEnd
@ _ARM_SkillList_CopyEnd:


    .INCLUDE "macros.inc"
    .SYNTAX UNIFIED

    .section .rodata

.global _GetUnitSkillList
_GetUnitSkillList:
    .4byte ARM_SkillList + (.Lfun_get - _ARM_SkillList_CopyStart)

.global _JudgeSkillViaList
_JudgeSkillViaList:
    .4byte ARM_SkillList + (.Lfun_judgeskill - _ARM_SkillList_CopyStart)

    .arm
    .section .rodata

.global _ARM_SkillList_CopyStart
_ARM_SkillList_CopyStart:

.Lfun_get:
    push {r4, r5, lr}
    mov r4, r0                  @ r4 = unit
    ldr r1, .LSkillList
    ldr r2, .LgBattleActor
    cmp r0, r2
    addeq r1, #0x40             @ sizeof(struct SkillList)
    add r2, #0x80               @ sizeof(struct BattleUnit)
    cmp r0, r2
    addeq r1, #0x80             @ sizeof(struct SkillList)
    mov r5, r1                  @ r5 = list
    bl .Lfunc_JudgeUnitList
    cmp r0, #1
    bne 2f
1:
    mov r0, r5
    pop {r4, r5}
    pop {r1}
    bx r1

2:
    @ This is a bad condition, should only exec once
    mov r0, r4
    mov r1, r5
    bl .Lfunc_GenerateSkillListExt
    b 1b

.Lfun_judgeskill:
    @ r0 = unit
    @ r1 = sid
    push {r4, lr}
    mov r4, r1
    bl .Lfun_get

    add r0, #0x10
    ldrb r2, [r0], #2
    lsl r2, #1
    add r3, r0, r2      @ r3: &list->sid[list->amt]
1:
    cmp r3, r0          @ r0: &list->sid[i]
    beq 2f
    ldrh r2, [r0], #2
    cmp r2, r4
    beq 3f
    b 1b

2:
    mov r0, #0
    pop {r4}
    pop {r1}
    bx r1

3:
    mov r0, #1
    pop {r4}
    pop {r1}
    bx r1

.Lget_list_generic_unit:

    mov pc, lr

.Lfunc_JudgeUnitList:
    ldr r2, 1f
    ldr pc, [r2]
    1:
    .4byte _JudgeUnitList

.Lfunc_GenerateSkillListExt:
    ldr r2, 1f
    bx r2
    1:
    .4byte GenerateSkillListExt

.LSkillList:
    .4byte sSkillList
    .4byte sSkillList + 0x40

.LgBattleActor:
    .4byte gBattleActor

.global _ARM_SkillList_CopyEnd
_ARM_SkillList_CopyEnd:

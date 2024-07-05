    .INCLUDE "macros.inc"
    .SYNTAX UNIFIED

    .section .rodata

.global _GetUnitSkillList
_GetUnitSkillList:
    .4byte ARM_SkillList + (.Lfun_get - _ARM_SkillList_CopyStart)

.global JudgeSkillViaList
JudgeSkillViaList:
    .4byte ARM_SkillList + (.Lfun_judgeskill - _ARM_SkillList_CopyStart)

    .arm
    .section .text

    .global _ARM_SkillList_CopyStart
_ARM_SkillList_CopyStart:

.Lfun_get:
    push {r4, r5, lr}
    mov r4, r0
    bl .Lget_list_generic_unit
    mov r5, r1
    bl .Lfunc_JudgeUnitList
    cmp r0, #1
    beq 2f

    @ This is a bad condition, should only exec once
    mov r0, r4
    mov r1, r5
    bl .Lfunc_GenerateSkillListExt

2:
    mov r0, r5
    pop {r4, r5}
    pop {r1}
    bx r1

.Lfun_judgeskill:
    @ r0 = unit
    @ r1 = sid
    push {r4, r5, lr}
    mov r4, r0
    mov r5, r1
    bl .Lget_list_battle_unit
    bl .Lfunc_JudgeUnitList
    cmp r0, #1
    beq 1f

    @ This is a bad condition, should only exec once
    push {r1}
    mov r0, r4
    mov r1, r5
    bl .Lfunc_GenerateSkillListExt
    pop {r1}

1:
    add r0, #0x10
    ldrb r2, [r0], #2
    add r3, r2          @ r3: &list->sid[list->amt]
    cmp r3, r0          @ r0: &list->sid[i]
    beq 2f
    ldrh r2, [r0], #2
    cmp r2, r1
    beq 3f
    b 1b

2:
    mov r0, #0
    pop {r4, r5}
    pop {r1}
    bx r1

3:
    mov r0, #1
    pop {r1}
    bx r1

.Lget_list_generic_unit:
    ldr r1, .LSkillList
    ldr r2, .LgBattleActor
    cmp r0, r2
    addeq r1, #0x40 @ sizeof(struct SkillList)
    add r2, #0x80   @ sizeof(struct BattleUnit)
    cmp r0, r2
    addeq r1, #0x80 @ sizeof(struct SkillList)
    mov pc, lr

.Lget_list_battle_unit:
    ldr r1, .LSkillList + 4
    ldr r3, .LgBattleActor + 4
    add r3, r0
    lsr r3, #1
    add r1, r3
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
    .4byte 0xFDFC5B14 // -gBattleActor

    .global _ARM_SkillList_CopyEnd
_ARM_SkillList_CopyEnd:

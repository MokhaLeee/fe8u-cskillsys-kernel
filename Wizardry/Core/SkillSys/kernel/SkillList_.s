    .INCLUDE "macros.inc"
    .SYNTAX UNIFIED

    .section .rodata

.global _GetUnitSkillList
_GetUnitSkillList:
    .4byte ARM_SkillList + (.Lfun_get - _ARM_SkillList_CopyStart)

    .arm
    .section .text

    .global _ARM_SkillList_CopyStart
_ARM_SkillList_CopyStart:

/**
struct SkillList * GetUnitSkillList(struct Unit * unit)
{
    struct SkillList * list = SkillListGeneric;
    if (unit == &gBattleActor.unit)
        list = SkillListBattleActor;
    else if (unit == &gBattleTarget.unit)
        list = SkillListBattleTarget;

    if (!JudgeUnitList(unit, &list->header))
        GenerateSkillListExt(unit, list);

    return list;
}
*/

.Lfun_get:
    push {r4, r5, lr}
    mov r4, r0
    ldr r5, .Llists

    ldr r1, .LBattleUnits
    cmp r0, r1
    bne 1f

    @ unit == &gBattleActor.unit
    ldr r5, .Llists + 4
    b 2f

1:
    ldr r1, .LBattleUnits + 4
    cmp r0, r1
    bne 2f

    @ unit == &gBattleTarget.unit
    ldr r5, .Llists + 8

2:
    ldr r5, [r5]
    mov r1, r5
    ldr r2, .LUnitList
    bl .Lfunc_bxldrr2
    cmp r0, #1
    beq 3f

    @ This is a bad condition, should only exec once
    mov r0, r4
    mov r1, r5
    ldr r2, .LGenerateSkillListExt
    bl .Lfunc_bxr2

3:
    mov r0, r5
    pop {r4, r5}
    pop {r1}
    bx r1

.Lfunc_bxldrr2:
    ldr r2, [r2]
.Lfunc_bxr2:
    bx r2

.LUnitList:
    .4byte _JudgeUnitList

.Llists:
    .4byte SkillListGeneric
    .4byte SkillListBattleActor
    .4byte SkillListBattleTarget

.LBattleUnits:
    .4byte gBattleActor
    .4byte gBattleTarget

.LGenerateSkillListExt:
    .4byte GenerateSkillListExt

    .global _ARM_SkillList_CopyEnd
_ARM_SkillList_CopyEnd:

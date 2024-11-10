#include "common-chax.h"
#include "debuff.h"
#include "weapon-range.h"
#include "kernel-lib.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

STATIC_DECLAR void AddTargetForRally(struct Unit * unit)
{
    if (UNIT_ALIVE(unit) && AreUnitsAllied(gSubjectUnit->index, unit->index))
        AddTarget(unit->xPos, unit->yPos, unit->index, 1);
}

STATIC_DECLAR void MakeTargetListForRally(struct Unit * unit)
{
    int x = unit->xPos;
    int y = unit->yPos;

    gSubjectUnit = unit;
    BmMapFill(gBmMapRange, 0);
    MapAddInBoundedRange(x, y, 1, 2);

    InitTargets(x, y);
    ForEachUnitInRange(AddTargetForRally);
}

u8 Rally_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (!HasSelectTarget(gActiveUnit, MakeTargetListForRally))
        return MENU_DISABLED;

    for (int i = 0; i < ARRAY_COUNT_RANGE3x3; i++)
    {
        int _x = gActiveUnit->xPos + gVecs_3x3[i].x;
        int _y = gActiveUnit->yPos + gVecs_3x3[i].y;

        struct Unit * unit_enemy = GetUnitAtPosition(_x, _y);

        if (!UNIT_IS_VALID(unit_enemy) || UNIT_STONED(unit_enemy) ||
            AreUnitsAllied(gActiveUnit->index, unit_enemy->index))
            continue;

#if (defined(SID_RallyStrength) && (COMMON_SKILL_VALID(SID_RallyStrength)))
        if (SkillTester(gActiveUnit, SID_RallyStrength))
#if (defined(SID_LullSpectrum) && (COMMON_SKILL_VALID(SID_LullSpectrum)))
            if (SkillTester(unit_enemy, SID_LullSpectrum))
                return MENU_DISABLED;
#else
            continue;
#endif
#if (defined(SID_LullStrength) && (COMMON_SKILL_VALID(SID_LullStrength)))
        if (SkillTester(unit_enemy, SID_LullStrength))
            return MENU_DISABLED;
#else
            continue;
#endif
#endif

#if (defined(SID_RallyMagic) && (COMMON_SKILL_VALID(SID_RallyMagic)))
        if (SkillTester(gActiveUnit, SID_RallyMagic))
#if (defined(SID_LullSpectrum) && (COMMON_SKILL_VALID(SID_LullSpectrum)))
            if (SkillTester(unit_enemy, SID_LullSpectrum))
                return MENU_DISABLED;
#else
            continue;
#endif
#if (defined(SID_LullMagic) && (COMMON_SKILL_VALID(SID_LullMagic)))
        if (SkillTester(unit_enemy, SID_LullMagic))
            return MENU_DISABLED;
#else
            continue;
#endif
#endif

#if (defined(SID_RallySkill) && (COMMON_SKILL_VALID(SID_RallySkill)))
        if (SkillTester(gActiveUnit, SID_RallySkill))
#if (defined(SID_LullSpectrum) && (COMMON_SKILL_VALID(SID_LullSpectrum)))
            if (SkillTester(unit_enemy, SID_LullSpectrum))
                return MENU_DISABLED;
#else
            continue;
#endif
#if (defined(SID_LullSkill) && (COMMON_SKILL_VALID(SID_LullSkill)))
        if (SkillTester(unit_enemy, SID_LullSkill))
            return MENU_DISABLED;
#else
            continue;
#endif
#endif

#if (defined(SID_RallySpeed) && (COMMON_SKILL_VALID(SID_RallySpeed)))
        if (SkillTester(gActiveUnit, SID_RallySpeed))
#if (defined(SID_LullSpectrum) && (COMMON_SKILL_VALID(SID_LullSpectrum)))
            if (SkillTester(unit_enemy, SID_LullSpectrum))
                return MENU_DISABLED;
#else
            continue;
#endif
#if (defined(SID_LullSpeed) && (COMMON_SKILL_VALID(SID_LullSpeed)))
        if (SkillTester(unit_enemy, SID_LullSpeed))
            return MENU_DISABLED;
#else
            continue;
#endif
#endif

#if (defined(SID_RallyLuck) && (COMMON_SKILL_VALID(SID_RallyLuck)))
        if (SkillTester(gActiveUnit, SID_RallyLuck))
#if (defined(SID_LullSpectrum) && (COMMON_SKILL_VALID(SID_LullSpectrum)))
            if (SkillTester(unit_enemy, SID_LullSpectrum))
                return MENU_DISABLED;
#else
            continue;
#endif
#if (defined(SID_LullLuck) && (COMMON_SKILL_VALID(SID_LullLuck)))
        if (SkillTester(unit_enemy, SID_LullLuck))
            return MENU_DISABLED;
#else
            continue;
#endif
#endif

#if (defined(SID_RallyDefense) && (COMMON_SKILL_VALID(SID_RallyDefense)))
        if (SkillTester(gActiveUnit, SID_RallyDefense))
#if (defined(SID_LullSpectrum) && (COMMON_SKILL_VALID(SID_LullSpectrum)))
            if (SkillTester(unit_enemy, SID_LullSpectrum))
                return MENU_DISABLED;
#else
            continue;
#endif
#if (defined(SID_LullDefense) && (COMMON_SKILL_VALID(SID_LullDefense)))
        if (SkillTester(unit_enemy, SID_LullDefense))
            return MENU_DISABLED;
#else
            continue;
#endif
#endif

#if (defined(SID_RallyResistance) && (COMMON_SKILL_VALID(SID_RallyResistance)))
        if (SkillTester(gActiveUnit, SID_RallyResistance))
#if (defined(SID_LullSpectrum) && (COMMON_SKILL_VALID(SID_LullSpectrum)))
            if (SkillTester(unit_enemy, SID_LullSpectrum))
                return MENU_DISABLED;
#else
            continue;
#endif
#if (defined(SID_LullResistance) && (COMMON_SKILL_VALID(SID_LullResistance)))
        if (SkillTester(unit_enemy, SID_LullResistance))
            return MENU_DISABLED;
#else
            continue;
#endif
#endif

#if (defined(SID_RallyMovement) && (COMMON_SKILL_VALID(SID_RallyMovement)))
        if (SkillTester(gActiveUnit, SID_RallyMovement))
#if (defined(SID_LullSpectrum) && (COMMON_SKILL_VALID(SID_LullSpectrum)))
            if (SkillTester(unit_enemy, SID_LullSpectrum))
                return MENU_DISABLED;
#else
            continue;
#endif
#if (defined(SID_LullMovement) && (COMMON_SKILL_VALID(SID_LullMovement)))
        if (SkillTester(unit_enemy, SID_LullMovement))
            return MENU_DISABLED;
#else
            continue;
#endif
#endif

#if (defined(SID_RallySpectrum) && (COMMON_SKILL_VALID(SID_RallySpectrum)))
        if (SkillTester(gActiveUnit, SID_RallySpectrum))
#if (defined(SID_LullSpectrum) && (COMMON_SKILL_VALID(SID_LullSpectrum)))
            if (SkillTester(unit_enemy, SID_LullSpectrum))
                return MENU_DISABLED;
#else
            continue;
#endif
#endif
    }

    return MENU_ENABLED;
}

int Rally_Hover(struct MenuProc * menu, struct MenuItemProc * item)
{
    BmMapFill(gBmMapMovement, -1);
    BmMapFill(gBmMapRange, 0);
    GenerateUnitStandingReachRange(gActiveUnit, 0b110);
    DisplayMoveRangeGraphics(MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_GREEN);
    return 0;
}

int Rally_Unhover(struct MenuProc * menu, struct MenuItemProc * menuItem)
{
    HideMoveRangeGraphics();
    return 0;
}

u8 Rally_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_MenuSkill_Rally_FRtext);
        return MENU_ACT_SND6B;
    }

    gActionData.unk08 = UnitMenuSkills[MENU_SKILL_INDEX(item->def)];
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

static void callback_anim(ProcPtr proc)
{
    StartLightRuneAnim(proc, gActiveUnit->xPos, gActiveUnit->yPos);
}

static void callback_exec(ProcPtr proc)
{
    int i;

    MakeTargetListForRally(gActiveUnit);

    for (i = 0; i < GetSelectTargetCount(); i++)
    {
        struct Unit * unit = GetUnit(GetTarget(i)->uid);
        if (!UNIT_ALIVE(unit))
            continue;

        switch (gActionData.unk08)
        {

#if defined(SID_RallyStrength) && (COMMON_SKILL_VALID(SID_RallyStrength))
            case SID_RallyStrength:
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_POW);
                break;
#endif

#if defined(SID_RallyMagic) && (COMMON_SKILL_VALID(SID_RallyMagic))
            case SID_RallyMagic:
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_MAG);
                break;
#endif

#if defined(SID_RallySkill) && (COMMON_SKILL_VALID(SID_RallySkill))
            case SID_RallySkill:
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_SKL);
                break;
#endif

#if defined(SID_RallySpeed) && (COMMON_SKILL_VALID(SID_RallySpeed))
            case SID_RallySpeed:
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_SPD);
                break;
#endif

#if defined(SID_RallyLuck) && (COMMON_SKILL_VALID(SID_RallyLuck))
            case SID_RallyLuck:
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_LCK);
                break;
#endif

#if defined(SID_RallyDefense) && (COMMON_SKILL_VALID(SID_RallyDefense))
            case SID_RallyDefense:
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_DEF);
                break;
#endif

#if defined(SID_RallyResistance) && (COMMON_SKILL_VALID(SID_RallyResistance))
            case SID_RallyResistance:
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_RES);
                break;
#endif

#if defined(SID_RallyMovement) && (COMMON_SKILL_VALID(SID_RallyMovement))
            case SID_RallyMovement:
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_MOV);
                break;
#endif

#if defined(SID_RallySpectrum) && (COMMON_SKILL_VALID(SID_RallySpectrum))
            case SID_RallySpectrum:
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_POW);
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_MAG);
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_SKL);
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_SPD);
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_LCK);
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_DEF);
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_RES);
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_MOV);
                break;
#endif
        }
    }
}

bool Action_Rally(ProcPtr parent)
{
    NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec);
    return true;
}

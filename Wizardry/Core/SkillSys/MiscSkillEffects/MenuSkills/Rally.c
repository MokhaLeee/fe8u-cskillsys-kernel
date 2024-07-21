#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
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

    return MENU_ENABLED;
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

static void anim_init(ProcPtr proc)
{
    struct MuProc * mu;

    HideUnitSprite(gActiveUnit);
    mu = StartMu(gActiveUnit);

    FreezeSpriteAnim(mu->sprite_anim);
    SetMuDefaultFacing(mu);
    SetDefaultColorEffects();
    EnsureCameraOntoPosition(proc, gActiveUnit->xPos, gActiveUnit->yPos);
}

static void anim_act(ProcPtr proc)
{
    StartMuActionAnim(GetUnitMu(gActiveUnit));
}

static void anim_core(ProcPtr proc)
{
    StartLightRuneAnim(proc, gActiveUnit->xPos, gActiveUnit->yPos);
}

static void exec(ProcPtr proc)
{
    int i;

    MakeTargetListForRally(gActiveUnit);

    for (i = 0; i < GetSelectTargetCount(); i++)
    {
        struct Unit * unit = GetUnit(GetTarget(i)->uid);
        if (!UNIT_ALIVE(unit))
            continue;

        switch (gActionData.unk08) {
        case SID_RallyStrength:
            SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_POW);
            break;

        case SID_RallyMagic:
            SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_MAG);
            break;

        case SID_RallySkill:
            SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_SKL);
            break;

        case SID_RallySpeed:
            SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_SPD);
            break;

        case SID_RallyLuck:
            SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_LCK);
            break;

        case SID_RallyDefense:
            SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_DEF);
            break;

        case SID_RallyResistance:
            SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_RES);
            break;

        case SID_RallyMovement:
            SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_MOV);
            break;

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
        }
    }
}

STATIC_DECLAR const struct ProcCmd ProcScr_ActionRally[] = {
    PROC_CALL(LockGame),
    PROC_CALL(MapAnim_CommonInit),
    PROC_CALL(EnsureCameraOntoActiveUnitPosition),
    PROC_YIELD,
    PROC_CALL(anim_init),
    PROC_YIELD,
    PROC_CALL(anim_act),
    PROC_SLEEP(30),
    PROC_CALL(anim_core),
    PROC_YIELD,
    PROC_CALL(exec),
    PROC_CALL(UnlockGame),
    PROC_CALL(MapAnim_CommonEnd),
    PROC_END
};

bool Action_Rally(ProcPtr parent)
{
    Proc_Start(ProcScr_ActionRally, PROC_TREE_3);
    return true;
}

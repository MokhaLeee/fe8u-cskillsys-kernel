#include "common-chax.h"
#include "status-getter.h"
#include "skill-system.h"
#include "constants/skills.h"

typedef int (*HealAmountGetterFunc_t)(int old, struct Unit *actor, struct Unit *target);
extern HealAmountGetterFunc_t const *const gpHealAmountGetters;

static int HealAmountGetter(int base, struct Unit *actor, struct Unit *target)
{
    const HealAmountGetterFunc_t *it;
    int status = base;

    for (it = gpHealAmountGetters; *it; it++)
        status = (*it)(status, actor, target);

    LIMIT_AREA(status, 0, 80);
    return status;
}

LYN_REPLACE_CHECK(GetUnitItemHealAmount);
int GetUnitItemHealAmount(struct Unit *unit, int item)
{
    int result = 0;

    switch (GetItemIndex(item))
    {
    case ITEM_STAFF_HEAL:
    case ITEM_STAFF_PHYSIC:
    case ITEM_STAFF_FORTIFY:
    case ITEM_VULNERARY:
    case ITEM_VULNERARY_2:
        result = 10;
        break;

    case ITEM_STAFF_MEND:
        result = 20;
        break;

    case ITEM_STAFF_RECOVER:
    case ITEM_ELIXIR:
        result = 80;
        break;

    } // switch (GetItemIndex(item))

    if (GetItemAttributes(item) & IA_STAFF)
        result += MagGetter(unit);

    if (result > 80)
        result = 80;

    return result;
}

LYN_REPLACE_CHECK(ExecStandardHeal);
void ExecStandardHeal(ProcPtr proc)
{
    int amount;

    struct Unit *unit_act = GetUnit(gActionData.subjectIndex);
    struct Unit *unit_tar = GetUnit(gActionData.targetIndex);

    BattleInitItemEffect(unit_act, gActionData.itemSlotIndex);

    amount = GetUnitItemHealAmount(
        unit_act,
        unit_act->items[gActionData.itemSlotIndex]);

#if CHAX
    amount = HealAmountGetter(amount, unit_act, unit_tar);
#endif

    BattleInitItemEffectTarget(unit_tar);

    AddUnitHp(unit_tar, amount);
    gBattleHitIterator->hpChange = gBattleTarget.unit.curHP - GetUnitCurrentHp(unit_tar);
    gBattleTarget.unit.curHP = GetUnitCurrentHp(unit_tar);

#if (defined(SID_LiveToServe) && (COMMON_SKILL_VALID(SID_LiveToServe)))
    if (SkillTester(unit_act, SID_LiveToServe))
    {
        //BattleInitItemEffectTarget(unit_act);
        AddUnitHp(unit_act, amount);
        gBattleHitIterator->hpChange = gBattleActor.unit.curHP - GetUnitCurrentHp(unit_act);
        gBattleActor.unit.curHP = GetUnitCurrentHp(unit_act);
    }
#endif

    BattleApplyItemEffect(proc);
    BeginBattleAnimations();
}

LYN_REPLACE_CHECK(ExecFortify);
void ExecFortify(ProcPtr proc)
{
    int i;
    int amount;
    int targetCount;

    struct Unit *unit_act = GetUnit(gActionData.subjectIndex);
    struct Unit *unit_tar = GetUnit(gActionData.targetIndex);

    BattleInitItemEffect(unit_act,
                         gActionData.itemSlotIndex);

    BattleInitItemEffectTarget(GetUnitFromCharId(GetPlayerLeaderUnitId()));
    MakeTargetListForRangedHeal(unit_act);

    amount = GetUnitItemHealAmount(
        unit_act,
        unit_act->items[gActionData.itemSlotIndex]);

    targetCount = GetSelectTargetCount();

    for (i = 0; i < targetCount; i++)
    {
#if CHAX
        int amound_real = HealAmountGetter(amount, unit_act, unit_tar);
        AddUnitHp(GetUnit(GetTarget(i)->uid), amound_real);
#else
        AddUnitHp(GetUnit(GetTarget(i)->uid), amount);
#endif
    }

    BattleApplyItemEffect(proc);
    BeginBattleAnimations();
}

LYN_REPLACE_CHECK(BattleInitItemEffectTarget);
void BattleInitItemEffectTarget(struct Unit *unit)
{
    InitBattleUnit(&gBattleTarget, unit);

    SetBattleUnitTerrainBonusesAuto(&gBattleTarget);
    ComputeBattleUnitBaseDefense(&gBattleTarget);
    ComputeBattleUnitSupportBonuses(&gBattleTarget, NULL);

    gBattleTarget.battleAttack = 0xFF;
    gBattleTarget.battleEffectiveHitRate = 0xFF;
    gBattleTarget.battleEffectiveCritRate = 0xFF;

    gBattleTarget.weaponBefore = 0;

    BattleUnitTargetSetEquippedWeapon(&gBattleTarget);

    gBattleActor.hasItemEffectTarget = TRUE;
}
#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"
#include "bwl.h"
#include "unit-expa.h"

int _GetUnitDefense(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = unit->def;

#ifdef CONFIG_STAT_SCREEN_TERRAIN_BONUS
	int terrainID = GetTrueTerrainAt(unit->xPos, unit->yPos);
	status += unit->pClassData->pTerrainDefenseLookup[terrainID];
#endif

#if defined(SID_Unaware) && (COMMON_SKILL_VALID(SID_Unaware))
    if (unit == GetUnit(gBattleActor.unit.index) && GetUnit(gBattleTarget.unit.index) && SkillTester(GetUnit(gBattleTarget.unit.index), SID_Unaware))
        return status;
    else if (unit == GetUnit(gBattleTarget.unit.index) && GetUnit(gBattleActor.unit.index) && SkillTester(GetUnit(gBattleActor.unit.index), SID_Unaware))
        return status;
#endif

    for (it = gpDefGetters; *it; it++)
        status = (*it)(status, unit);

    return status;
}

/* Hooks */
int DefGetterWeaponBonus(int status, struct Unit * unit)
{
    u16 weapon = GetUnitEquippedWeapon(unit);
    status += GetItemDefBonus(weapon);
    return status;
}

int DefGetterSkills(int status, struct Unit * unit)
{
    int cur_hp = GetUnitCurrentHp(unit);
    int max_hp = GetUnitMaxHp(unit);

#if defined(CONFIG_RESET_BWL_STATS_EACH_CHAPTER)
    struct NewBwl * bwl = GetNewBwl(UNIT_CHAR_ID(unit));
#endif

#if defined(SID_Sellsword) && (COMMON_SKILL_VALID(SID_Sellsword))
    if (SkillTester(unit, SID_Sellsword) && CheckBitUES(unit, UES_BIT_SELLSWORD_SKILL_USED))
        status += SKILL_EFF0(SID_Sellsword);
#endif

#if defined(SID_LifeAndDeath) && (COMMON_SKILL_VALID(SID_LifeAndDeath))
    if (SkillTester(unit, SID_LifeAndDeath))
        status += SKILL_EFF1(SID_LifeAndDeath);
#endif

#if defined(SID_DefBonus) && (COMMON_SKILL_VALID(SID_DefBonus))
    if (SkillTester(unit, SID_DefBonus))
        status += SKILL_EFF0(SID_DefBonus);
#endif

#if defined(SID_DefiantDef) && (COMMON_SKILL_VALID(SID_DefiantDef))
    if (SkillTester(unit, SID_DefiantDef))
        if ((cur_hp * 4) < max_hp)
            status += SKILL_EFF0(SID_DefiantDef);
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
    if (SkillTester(unit, SID_Fury))
        status += SKILL_EFF0(SID_Fury);
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
    if (SkillTester(unit, SID_FuryPlus))
        status += SKILL_EFF0(SID_FuryPlus);
#endif

#if defined(SID_FortressDef) && (COMMON_SKILL_VALID(SID_FortressDef))
    if (SkillTester(unit, SID_FortressDef))
        status += SKILL_EFF1(SID_FortressDef);
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_DEF)
        status += SKILL_EFF0(SID_LuckySeven);
#endif

#if defined(SID_DefenseBoost) && (COMMON_SKILL_VALID(SID_DefenseBoost))
    if (SkillTester(unit, SID_DefenseBoost))
    {
        int __buf = SKILL_EFF0(SID_DefenseBoost);
        if (gPlaySt.chapterTurnNumber >= __buf)
            status += __buf;
        else
            status += gPlaySt.chapterTurnNumber;
    }
#endif

#if defined(SID_SpectrumBoost) && (COMMON_SKILL_VALID(SID_SpectrumBoost))
    if (SkillTester(unit, SID_SpectrumBoost))
    {
        int __buf = SKILL_EFF0(SID_SpectrumBoost);
        if (gPlaySt.chapterTurnNumber >= __buf)
            status += __buf;
        else
            status += gPlaySt.chapterTurnNumber;
    }
#endif

#if defined(SID_StaffGuard) && (COMMON_SKILL_VALID(SID_StaffGuard))
    if (SkillTester(unit, SID_StaffGuard))
    {
        bool hasUsableStaff = false;

        for (int i = 0; i < 5; i++)
        {
            if (ITEM_INDEX(unit->items[i]) == 0)
                break;
            if (GetItemType(unit->items[i]) != ITYPE_STAFF)
                continue;
            if (GetItemRequiredExp(unit->items[i]) <= unit->ranks[ITYPE_STAFF])
            {
                hasUsableStaff = true;
                break;
            }
        }

        if(hasUsableStaff)
            status += SKILL_EFF0(SID_StaffGuard);
    }
#endif

#if (defined(SID_TakerDefense) && (COMMON_SKILL_VALID(SID_TakerDefense)) && defined(CONFIG_RESET_BWL_STATS_EACH_CHAPTER))
    if (SkillTester(unit, SID_TakerDefense))
    {
        int takerBoost = bwl->winAmt * SKILL_EFF0(SID_TakerDefense);
        
        if (takerBoost > 10)
            status += 10;
        else   
            status += takerBoost;
    }
#endif

#if (defined(SID_TakerSpectrum) && (COMMON_SKILL_VALID(SID_TakerSpectrum)) && defined(CONFIG_RESET_BWL_STATS_EACH_CHAPTER))
    if (SkillTester(unit, SID_TakerSpectrum))
    {
        int takerBoost = bwl->winAmt * SKILL_EFF0(SID_TakerSpectrum);
        
        if (takerBoost > 10)
            status += 10;
        else   
            status += takerBoost;
    }
#endif

    if (cur_hp == max_hp)
    {
#if defined(SID_PushDefense) && (COMMON_SKILL_VALID(SID_PushDefense))
        if (SkillTester(unit, SID_PushDefense))
            status += SKILL_EFF0(SID_PushDefense);
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
        if (SkillTester(unit, SID_PushSpectrum))
            status += SKILL_EFF0(SID_PushSpectrum);
#endif
    }

#if (defined(SID_PairUp) && (COMMON_SKILL_VALID(SID_PairUp)))
    if (SkillTester(unit, SID_PairUp))
        if (unit->state & US_RESCUING)
            status += Div(_GetUnitDefense(GetUnit(unit->rescue)) * SKILL_EFF0(SID_PairUp), 100);
#endif

#if defined(SID_SteadyRider) && (COMMON_SKILL_VALID(SID_SteadyRider))
        if (SkillTester(unit, SID_SteadyRider))
            status += GetItemWeight(GetUnitEquippedWeapon(unit)) > UNIT_CON(unit) ? GetItemWeight(GetUnitEquippedWeapon(unit)) - UNIT_CON(unit) : 0;
#endif

#if defined(SID_SteadyRiderPlus) && (COMMON_SKILL_VALID(SID_SteadyRiderPlus))
        if (SkillTester(unit, SID_SteadyRiderPlus))
            status += GetItemWeight(GetUnitEquippedWeapon(unit)) > UNIT_CON(unit) ? GetItemWeight(GetUnitEquippedWeapon(unit)) - UNIT_CON(unit) : 0;
#endif

    return status;
}

int DefPsychUpCheck(int status, struct Unit * unit)
{
    FORCE_DECLARE int stolen_status = 0;

#if (defined(SID_PsychUp) && (COMMON_SKILL_VALID(SID_PsychUp)))
    if (unit == GetUnit(gBattleActor.unit.index) && SkillTester(unit, SID_PsychUp))
    {
        stolen_status = DefGetterWeaponBonus(0, GetUnit(gBattleTarget.unit.index)) +
            DefGetterSkills(0, GetUnit(gBattleTarget.unit.index));
        return status + stolen_status;
    }
    else if (unit == GetUnit(gBattleTarget.unit.index) && SkillTester(unit, SID_PsychUp))
    {
        stolen_status = DefGetterWeaponBonus(0, GetUnit(gBattleActor.unit.index)) +
            DefGetterSkills(0, GetUnit(gBattleActor.unit.index));
        return status + stolen_status;
    }
#endif

    return status;
}
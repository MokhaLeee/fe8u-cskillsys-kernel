#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"
#include "bwl.h"
#include "unit-expa.h"

int _GetUnitResistance(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = unit->res;

#ifdef CONFIG_STAT_SCREEN_TERRAIN_BONUS
	int terrainID = GetTrueTerrainAt(unit->xPos, unit->yPos);
	status += unit->pClassData->pTerrainResistanceLookup[terrainID];
#endif

#if defined(SID_Unaware) && (COMMON_SKILL_VALID(SID_Unaware))
    if (unit == GetUnit(gBattleActor.unit.index) && GetUnit(gBattleTarget.unit.index) && SkillTester(GetUnit(gBattleTarget.unit.index), SID_Unaware))
        return status;
    else if (unit == GetUnit(gBattleTarget.unit.index) && GetUnit(gBattleActor.unit.index) && SkillTester(GetUnit(gBattleActor.unit.index), SID_Unaware))
        return status;
#endif

    for (it = gpResGetters; *it; it++)
        status = (*it)(status, unit);

    return status;
}

/* Hooks */
int ResGetterWeaponBonus(int status, struct Unit * unit)
{
    u16 weapon = GetUnitEquippedWeapon(unit);
    status += GetItemResBonus(weapon);
    return status;
}

int ResGetterSkills(int status, struct Unit * unit)
{
    int cur_hp = GetUnitCurrentHp(unit);
    int max_hp = GetUnitMaxHp(unit);

#if defined(CONFIG_RESET_BWL_STATS_EACH_CHAPTER)
    struct NewBwl * bwl = GetNewBwl(UNIT_CHAR_ID(unit));
#endif

#if defined(SID_LifeAndDeath) && (COMMON_SKILL_VALID(SID_LifeAndDeath))
    if (SkillTester(unit, SID_LifeAndDeath))
        status += SKILL_EFF1(SID_LifeAndDeath);
#endif

#if defined(SID_ResBonus) && (COMMON_SKILL_VALID(SID_ResBonus))
    if (SkillTester(unit, SID_ResBonus))
        status += SKILL_EFF0(SID_ResBonus);
#endif

#if defined(SID_DefiantRes) && (COMMON_SKILL_VALID(SID_DefiantRes))
    if (SkillTester(unit, SID_DefiantRes))
        if ((cur_hp * 4) < max_hp)
            status += SKILL_EFF0(SID_DefiantRes);
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
    if (SkillTester(unit, SID_Fury))
        status += SKILL_EFF0(SID_Fury);
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
    if (SkillTester(unit, SID_FuryPlus))
        status += SKILL_EFF0(SID_FuryPlus);
#endif

#if defined(SID_FortressRes) && (COMMON_SKILL_VALID(SID_FortressRes))
    if (SkillTester(unit, SID_FortressRes))
        status += SKILL_EFF1(SID_FortressRes);
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
    if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_RES)
        status += SKILL_EFF0(SID_LuckySeven);
#endif

#if defined(SID_ResistanceBoost) && (COMMON_SKILL_VALID(SID_ResistanceBoost))
    if (SkillTester(unit, SID_ResistanceBoost))
    {
        int __buf = SKILL_EFF0(SID_ResistanceBoost);
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

#if (defined(SID_TakerResistance) && (COMMON_SKILL_VALID(SID_TakerResistance)) && defined(CONFIG_RESET_BWL_STATS_EACH_CHAPTER))
    if (SkillTester(unit, SID_TakerResistance))
    {
        int takerBoost = bwl->winAmt * SKILL_EFF0(SID_TakerResistance);
        
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
#if defined(SID_PushResistance) && (COMMON_SKILL_VALID(SID_PushResistance))
        if (SkillTester(unit, SID_PushResistance))
            status += SKILL_EFF0(SID_PushResistance);
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
        if (SkillTester(unit, SID_PushSpectrum))
            status += SKILL_EFF0(SID_PushSpectrum);
#endif
    }

#if (defined(SID_PairUp) && (COMMON_SKILL_VALID(SID_PairUp)))
    if (SkillTester(unit, SID_PairUp))
        if (unit->state & US_RESCUING)
            status += Div(_GetUnitResistance(GetUnit(unit->rescue)) * SKILL_EFF0(SID_PairUp), 100);
#endif

#if defined(SID_SteadyRiderPlus) && (COMMON_SKILL_VALID(SID_SteadyRiderPlus))
        if (SkillTester(unit, SID_SteadyRiderPlus))
            status += GetItemWeight(GetUnitEquippedWeapon(unit)) > UNIT_CON(unit) ? GetItemWeight(GetUnitEquippedWeapon(unit)) - UNIT_CON(unit) : 0;
#endif

#if defined(SID_Sellsword) && (COMMON_SKILL_VALID(SID_Sellsword))
    if (SkillTester(unit, SID_Sellsword) && CheckBitUES(unit, UES_BIT_SELLSWORD_SKILL_USED))
        status += SKILL_EFF0(SID_Sellsword);
#endif

    return status;
}

int ResPsychUpCheck(int status, struct Unit *unit)
{
    FORCE_DECLARE int stolen_status = 0;

#if (defined(SID_PsychUp) && (COMMON_SKILL_VALID(SID_PsychUp)))
    if (unit == GetUnit(gBattleActor.unit.index) && SkillTester(unit, SID_PsychUp))
    {
        stolen_status = ResGetterWeaponBonus(0, GetUnit(gBattleTarget.unit.index)) + ResGetterSkills(0, GetUnit(gBattleTarget.unit.index));
        return status + stolen_status;
    }
    else if (unit == GetUnit(gBattleTarget.unit.index) && SkillTester(unit, SID_PsychUp))
    {
        stolen_status = ResGetterWeaponBonus(0, GetUnit(gBattleActor.unit.index)) + ResGetterSkills(0, GetUnit(gBattleActor.unit.index));
        return status + stolen_status;
    }
#endif

    return status;
}

int ResGetterPureWater(int status, struct Unit *unit)
{
	return status + unit->barrierDuration;
}
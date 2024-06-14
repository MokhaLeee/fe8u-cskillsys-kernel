#include "common-chax.h"
#include "skill-system.h"
#include "strmag.h"
#include "debuff.h"
#include "constants/skills.h"

STATIC_DECLAR void BattleCalcReal_ModifyBattleStatusSkills(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    /**
     * Here we need to put some calculation at the end of the pre-battle calc.
     * Thus the main part of calc should be positioned at berfore.
     */
    struct Unit * unit = GetUnit(attacker->unit.index);

#if (defined(SID_CatchingUp) && (SID_CatchingUp < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_CatchingUp))
        {
            /**
             * Check if the enemy unit doubles the skill holder
             * if there's any additional speed above the doubling threshold
             * add that to the skillholder's attack
             */
            if ((defender->battleSpeed - attacker->battleSpeed) > BATTLE_FOLLOWUP_SPEED_THRESHOLD)
                attacker->battleAttack += (defender->battleSpeed - attacker->battleSpeed);
        }
#endif

    if (attacker->battleAttack > defender->battleAttack)
    {
#if (defined(SID_HeavyBlade) && (SID_HeavyBlade < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_HeavyBlade))
            attacker->battleCritRate += 15;
#endif

#if (defined(SID_HeavyBladePlus) && (SID_HeavyBladePlus < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_HeavyBladePlus))
            attacker->battleCritRate += 25;
#endif
    }

    if (ConGetter(unit) < ConGetter(&defender->unit))
    {
#if (defined(SID_DancingBlade) && (SID_DancingBlade < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_DancingBlade))
        {
            attacker->battleSpeed += 4;
            attacker->battleDefense += 2;
        }
#endif
    }

    if (attacker->battleSpeed > defender->battleSpeed)
    {
#if (defined(SID_FlashingBlade) && (SID_FlashingBlade < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_FlashingBlade))
            attacker->battleCritRate += 15;
#endif

#if (defined(SID_FlashingBladePlus) && (SID_FlashingBladePlus < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_FlashingBladePlus))
            attacker->battleCritRate += 25;
#endif
    }
}

/* LynJump */
void ComputeBattleUnitSpecialWeaponStats(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    if (attacker->weaponAttributes & IA_MAGICDAMAGE) {
        switch (GetItemIndex(attacker->weapon)) {
        case ITEM_SWORD_LIGHTBRAND:
        case ITEM_SWORD_RUNESWORD:
        case ITEM_SWORD_WINDSWORD:
            attacker->battleAttack -= UNIT_MAG(&attacker->unit) / 2;
            attacker->battleCritRate = 0;
            attacker->battleEffectiveCritRate = 0;
            break;
        }
    }
    else
    {
        if (GetItemWeaponEffect(attacker->weaponBefore) == WPN_EFFECT_HPHALVE)
        {
            attacker->battleAttack = (defender->unit.curHP + 1) >> 1;

            if (attacker->battleAttack == 0)
                attacker->battleAttack = 1;

            defender->battleDefense = 0;
            attacker->battleCritRate = 0;
            attacker->battleEffectiveCritRate = 0;
        }

        if (attacker->weaponAttributes & IA_NEGATE_DEFENSE)
            defender->battleDefense = 0;

#ifdef CHAX
        if (GetUnitStatusIndex(&defender->unit) == UNIT_STATUS_PETRIFY || GetUnitStatusIndex(&defender->unit) == UNIT_STATUS_13)
#else
        if (defender->unit.statusIndex == UNIT_STATUS_PETRIFY || defender->unit.statusIndex == UNIT_STATUS_13)
#endif
        {
            attacker->battleEffectiveHitRate = 100;
            attacker->battleEffectiveCritRate += 30;

            if (attacker->battleEffectiveCritRate > 100)
                attacker->battleEffectiveCritRate = 100;
        }
    }
}

STATIC_DECLAR void BattleCalcReal_ComputSkills(struct BattleUnit * attacker, struct BattleUnit * defender)
{
#if (defined(SID_Hawkeye) && (SID_Hawkeye < MAX_SKILL_NUM))
        if (SkillTester(&attacker->unit, SID_Hawkeye))
           attacker->battleEffectiveHitRate = 100;
#endif

#if (defined(SID_Merciless) && (SID_Merciless < MAX_SKILL_NUM))
    if (SkillTester(&attacker->unit, SID_Merciless))
    {
        if (GetUnitStatusIndex(&defender->unit) == UNIT_STATUS_POISON)
            attacker->battleEffectiveCritRate = 100;
    }
#endif

#if (defined(SID_WonderGuard) && (SID_WonderGuard < MAX_SKILL_NUM))
    if (SkillTester(&defender->unit, SID_WonderGuard))
    {
        if (defender->weaponType == attacker->weaponType) 
            attacker->battleAttack = 0;
    }
#endif
}

/* LynJump */
void ComputeBattleUnitSilencerRate(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    return;
}

/* LynJump */
void ComputeBattleUnitEffectiveHitRate(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    attacker->battleEffectiveHitRate = attacker->battleHitRate - defender->battleAvoidRate;

    /* Distance +2, hit rate -20% */
    attacker->battleEffectiveHitRate -= Div(gBattleStats.range, 2) * 20;

    if (attacker->battleEffectiveHitRate > 100)
        attacker->battleEffectiveHitRate = 100;

    if (attacker->battleEffectiveHitRate < 0)
        attacker->battleEffectiveHitRate = 0;
}

/* LynJump */
void ComputeBattleUnitEffectiveStats(struct BattleUnit * attacker, struct BattleUnit * defender)
{
#if CHAX
    BattleCalcReal_ModifyBattleStatusSkills(attacker, defender);
#endif

    ComputeBattleUnitEffectiveHitRate(attacker, defender);
    ComputeBattleUnitEffectiveCritRate(attacker, defender);
    ComputeBattleUnitSilencerRate(attacker, defender);
    ComputeBattleUnitSpecialWeaponStats(attacker, defender);

#if CHAX
    BattleCalcReal_ComputSkills(attacker, defender);
#endif
}

#include "common-chax.h"
#include "skill-system.h"
#include "strmag.h"
#include "debuff.h"
#include "constants/skills.h"

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
           attacker->battleEffectiveHitRate = INT16_MAX;
#endif

#if (defined(SID_WonderGuard) && (SID_WonderGuard < MAX_SKILL_NUM))
    if (SkillTester(&attacker->unit, SID_WonderGuard))
    {
        if (defender->weaponType == attacker->weaponType) 
            attacker->battleDefense = INT16_MAX;
    }
#endif

#if (defined(SID_Merciless) && (SID_Merciless < MAX_SKILL_NUM))
    if (SkillTester(&attacker->unit, SID_Merciless))
    {
        if (GetUnitStatusIndex(&defender->unit) == UNIT_STATUS_POISON)
            attacker->battleEffectiveCritRate = INT16_MAX;
    }
#endif
}

/* LynJump */
void ComputeBattleUnitEffectiveStats(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    ComputeBattleUnitEffectiveHitRate(attacker, defender);
    ComputeBattleUnitEffectiveCritRate(attacker, defender);
    ComputeBattleUnitSilencerRate(attacker, defender);
    ComputeBattleUnitSpecialWeaponStats(attacker, defender);

#if CHAX
    BattleCalcReal_ComputSkills(attacker, defender);
#endif
}

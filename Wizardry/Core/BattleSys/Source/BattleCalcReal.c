#include "common-chax.h"
#include "skill-system.h"
#include "combat-art.h"
#include "strmag.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "kernel-tutorial.h"
#include "constants/combat-arts.h"
#include "constants/skills.h"
#include "weapon-range.h"

#include "jester_headers/custom-functions.h"

STATIC_DECLAR void BattleCalcReal_ModifyBattleStatusSkills(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    /**
     * Here we need to put some calculation at the end of the pre-battle calc.
     * Thus the main part of calc should be positioned at berfore.
     */

    if (attacker == &gBattleActor)
    {
        switch (GetCombatArtInForce(&attacker->unit))
        {
            case CID_Gamble:
                attacker->battleCritRate = attacker->battleCritRate * 2;
                attacker->battleHitRate = attacker->battleHitRate / 2;
                break;

            default:
                break;
        }
    }

#if (defined(SID_CatchingUp) && (COMMON_SKILL_VALID(SID_CatchingUp)))
    if (BattleFastSkillTester(attacker, SID_CatchingUp))
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
#if (defined(SID_HeavyBlade) && (COMMON_SKILL_VALID(SID_HeavyBlade)))
        if (BattleFastSkillTester(attacker, SID_HeavyBlade))
            attacker->battleCritRate += SKILL_EFF0(SID_HeavyBlade);
#endif

#if (defined(SID_HeavyBladePlus) && (COMMON_SKILL_VALID(SID_HeavyBladePlus)))
        if (BattleFastSkillTester(attacker, SID_HeavyBladePlus))
            attacker->battleCritRate += SKILL_EFF0(SID_HeavyBladePlus);
#endif
    }

    if (attacker->unit.conBonus < defender->unit.conBonus)
    {
#if (defined(SID_DancingBlade) && (COMMON_SKILL_VALID(SID_DancingBlade)))
        if (BattleFastSkillTester(attacker, SID_DancingBlade))
        {
            attacker->battleSpeed += SKILL_EFF0(SID_DancingBlade);
            attacker->battleDefense += SKILL_EFF1(SID_DancingBlade);
        }
#endif
    }

    if (attacker->battleSpeed > defender->battleSpeed)
    {
#if (defined(SID_FlashingBlade) && (COMMON_SKILL_VALID(SID_FlashingBlade)))
        if (BattleFastSkillTester(attacker, SID_FlashingBlade))
            attacker->battleCritRate += SKILL_EFF0(SID_HeavyBlade);
#endif

#if (defined(SID_FlashingBladePlus) && (COMMON_SKILL_VALID(SID_FlashingBladePlus)))
        if (BattleFastSkillTester(attacker, SID_FlashingBladePlus))
            attacker->battleCritRate += SKILL_EFF1(SID_FlashingBladePlus);
#endif

#if (defined(SID_Puissance) && (COMMON_SKILL_VALID(SID_Puissance)))
        if (BattleFastSkillTester(attacker, SID_Puissance))
            attacker->battleAttack += SKILL_EFF0(SID_Puissance);
#endif
    }
}

LYN_REPLACE_CHECK(ComputeBattleUnitSpecialWeaponStats);
void ComputeBattleUnitSpecialWeaponStats(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    if (attacker->weaponAttributes & IA_MAGICDAMAGE)
    {
        switch (GetItemIndex(attacker->weapon))
        {
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
        if (GetUnitStatusIndex(&defender->unit) == UNIT_STATUS_PETRIFY ||
            GetUnitStatusIndex(&defender->unit) == UNIT_STATUS_13)
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
#if (defined(SID_Hawkeye) && (COMMON_SKILL_VALID(SID_Hawkeye)))
    if (BattleFastSkillTester(attacker, SID_Hawkeye))
        attacker->battleEffectiveHitRate = 100;
#endif

#if (defined(SID_Merciless) && (COMMON_SKILL_VALID(SID_Merciless)))
    if (BattleFastSkillTester(attacker, SID_Merciless))
    {
        if (GetUnitStatusIndex(&defender->unit) == UNIT_STATUS_POISON)
            attacker->battleEffectiveCritRate = 100;
    }
#endif

#if (defined(SID_MoonBow) && (COMMON_SKILL_VALID(SID_MoonBow)))
    if (BattleFastSkillTester(attacker, SID_MoonBow))
        attacker->battleAttack += Div(defender->battleDefense * SKILL_EFF0(SID_MoonBow), 100);
#endif

#if (defined(SID_WonderGuard) && (COMMON_SKILL_VALID(SID_WonderGuard)))
    if (BattleFastSkillTester(defender, SID_WonderGuard))
    {
        if (defender->weaponType == attacker->weaponType)
            attacker->battleAttack = 0;
    }
#endif

#if (defined(SID_Mantle) && (COMMON_SKILL_VALID(SID_Mantle)))
    if (BattleFastSkillTester(defender, SID_Mantle))
    {
        if (GetItemIndex(attacker->weapon) != ITEM_AXE_IRON)
            attacker->battleAttack = 0;
    }
#endif

#if (defined(SID_NoGuard) && (COMMON_SKILL_VALID(SID_NoGuard)))
    if (BattleFastSkillTester(attacker, SID_NoGuard) || BattleFastSkillTester(defender, SID_NoGuard))
        attacker->battleEffectiveHitRate = 100;
#endif

#if (defined(SID_Seer) && (COMMON_SKILL_VALID(SID_Seer)))
    if (BattleFastSkillTester(attacker, SID_Seer))
    {
        attacker->battleEffectiveHitRate = 100;
        defender->battleEffectiveHitRate = 0;
    }  
    else if (BattleFastSkillTester(defender, SID_Seer))
    {
        defender->battleEffectiveHitRate = 100;
        attacker->battleEffectiveHitRate = 0;
    }   
#endif

#if (defined(SID_RiskItAll) && (COMMON_SKILL_VALID(SID_RiskItAll)))
    if (BattleFastSkillTester(attacker, SID_RiskItAll) || BattleFastSkillTester(defender, SID_RiskItAll))
        attacker->battleEffectiveCritRate = SKILL_EFF0(SID_RiskItAll);
#endif

#if (defined(SID_Adaptable) && (COMMON_SKILL_VALID(SID_Adaptable)))
    if (BattleFastSkillTester(defender, SID_Adaptable) && defender == &gBattleTarget)
    {
        u8 weapon_score[] = { 0, 0, 0, 0, 0 };
        u8 weapon_attack_values[] = { 0, 0, 0, 0, 0 };
        int weapon_strongest_position = 0;
        int weapon_strongest_details = 0;
        struct Unit * unit_attacker = GetUnit(attacker->unit.index);
        struct Unit * unit_defender = GetUnit(defender->unit.index);

        for (int i = 0; i < GetUnitItemCount(unit_defender); i++)
        {
            // Weapon range falls outside the combat range, so move on to the next item
            if (GetItemMinRangeRework(unit_defender->items[i], unit_defender) > gBattleStats.range ||
                GetItemMaxRangeRework(unit_defender->items[i], unit_defender) < gBattleStats.range)
                continue;

            if (isWeaponTriangleAdvantage(GetItemType(unit_defender->items[i]), GetItemType(GetUnitEquippedWeapon(unit_attacker))))
                weapon_score[i] += 4;
            if (IsItemEffectiveAgainst(unit_defender->items[i], unit_attacker))
                weapon_score[i] += 3;
            if (weaponHasSpecialEffect(GetItemAttributes(unit_defender->items[i])))
                weapon_score[i] += 2;

            weapon_attack_values[i] = GetItemMight(unit_defender->items[i]);
        }

        weapon_score[findMax(weapon_attack_values, 5)] += 1;

        weapon_strongest_position = findMax(weapon_score, 5);
        weapon_strongest_details = unit_defender->items[weapon_strongest_position];

        if (weapon_strongest_position != 0)
        {
            // Put the currently equipped weapon in the place of the strongest weapon
            unit_defender->items[weapon_strongest_position] = unit_defender->items[0];
            // Now equip the strongest weapon
            unit_defender->items[0] = weapon_strongest_details;
        }
    }
#endif
}

LYN_REPLACE_CHECK(ComputeBattleUnitSilencerRate);
void ComputeBattleUnitSilencerRate(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    return;
}

LYN_REPLACE_CHECK(ComputeBattleUnitEffectiveHitRate);
void ComputeBattleUnitEffectiveHitRate(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    attacker->battleEffectiveHitRate = attacker->battleHitRate - defender->battleAvoidRate;

    /* For non-ballista combat, Distance +2, hit rate -20% for actor */
    if (gpKernelDesigerConfig->hit_decrease_on_range && gBattleStats.range > 2 && attacker == &gBattleActor)
    {
        if (
#if defined(SID_MagicEye) && (COMMON_SKILL_VALID(SID_MagicEye))
            !BattleFastSkillTester(attacker, SID_MagicEye)
#else
            1
#endif
            && !(gBattleStats.config & BATTLE_CONFIG_BALLISTA))
        {
            attacker->battleEffectiveHitRate -= Div(gBattleStats.range, 2) * 20;

            if (gBattleStats.config & BATTLE_CONFIG_REAL)
                TriggerKtutorial(KTUTORIAL_RANGED_FAILOFF);
        }
    }

    if (attacker->battleEffectiveHitRate > 100)
        attacker->battleEffectiveHitRate = 100;

    if (attacker->battleEffectiveHitRate < 0)
        attacker->battleEffectiveHitRate = 0;

#if (defined(SID_FranticSwing) && (COMMON_SKILL_VALID(SID_FranticSwing)))
    if (BattleFastSkillTester(attacker, SID_FranticSwing))
    {
        if (attacker->battleEffectiveHitRate <= 50)
            attacker->battleCritRate += SKILL_EFF0(SID_FranticSwing);
    }
#endif
}

LYN_REPLACE_CHECK(ComputeBattleUnitEffectiveStats);
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

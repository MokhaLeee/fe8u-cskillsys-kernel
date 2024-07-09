#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "strmag.h"
#include "constants/skills.h"

#define LOCAL_TRACE 0

void PreBattleCalcWeaponTriangle(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    const struct WeaponTriangleConf * it;
    const struct WeaponTriangleItemConf * item_conf = &gpWeaponTriangleItemConf[ITEM_INDEX(attacker->weaponBefore)];
    int multiplier = 1;

#if (defined(SID_Nonconforming) && (COMMON_SKILL_VALID(SID_Nonconforming)))
    if (BattleSkillTester(attacker, SID_Nonconforming))
        multiplier *= -1;
    else if (BattleSkillTester(defender, SID_Nonconforming))
        multiplier *= -1;
#endif

    if (item_conf->valid && item_conf->wtype == defender->weaponType)
    {
        LTRACEF("Find item_conf (%p-%p, %d): item=%X atk=%d, def=%d, hit=%d, avo=%d, crt=%d, sil=%d",
            item_conf->wtype, attacker, defender, ITEM_INDEX(attacker->weaponBefore),
            item_conf->battle_status.atk, item_conf->battle_status.def, item_conf->battle_status.hit,
            item_conf->battle_status.avo, item_conf->battle_status.crit, item_conf->battle_status.silencer);

        attacker->battleAttack       += item_conf->battle_status.atk;
        attacker->battleDefense      += item_conf->battle_status.def;
        attacker->battleHitRate      += item_conf->battle_status.hit;
        attacker->battleAvoidRate    += item_conf->battle_status.avo;
        attacker->battleCritRate     += item_conf->battle_status.crit;
        attacker->battleSilencerRate += item_conf->battle_status.silencer;

        /* Just for UI */
        if (item_conf->is_buff)
        {
            attacker->wTriangleHitBonus += 15 * multiplier;
            attacker->wTriangleDmgBonus += 1 * multiplier;
            defender->wTriangleHitBonus -= 15 * multiplier;
            defender->wTriangleDmgBonus -= 1 * multiplier;
        }
        else
        {
            attacker->wTriangleHitBonus -= 15 * multiplier;
            attacker->wTriangleDmgBonus -= 1 * multiplier;
            defender->wTriangleHitBonus += 15 * multiplier;
            defender->wTriangleDmgBonus += 1 * multiplier;
        }
    }
    else
    {
        /* vanilla */
        const struct WeaponTriangleRule * it;
        for (it = sWeaponTriangleRules; it->attackerWeaponType >= 0; ++it)
        {
            if ((attacker->weaponType == it->attackerWeaponType) && (defender->weaponType == it->defenderWeaponType))
            {
                LTRACEF("Find vanilla (%p-%p, %d-%d): atk=%d, hit=%d",
                        attacker, defender, it->attackerWeaponType, it->defenderWeaponType, it->atkBonus, it->hitBonus);

                attacker->battleAttack  += it->atkBonus * multiplier;
                attacker->battleHitRate += it->hitBonus * multiplier;

                /* Just for UI */
                attacker->wTriangleHitBonus += it->atkBonus * multiplier;
                attacker->wTriangleDmgBonus += it->hitBonus * multiplier;
                defender->wTriangleHitBonus -= it->atkBonus * multiplier;
                defender->wTriangleDmgBonus -= it->hitBonus * multiplier;
                break;
            }
        }
    }

    for (it = gpWeaponTriangleConfs; it->wtype_a != it->wtype_b; it++)
    {
        if (it->wtype_a == attacker->weaponType && it->wtype_b == defender->weaponType)
        {
            LTRACEF("Find conf (%p-%p, %d-%d): atk=%d, def=%d, as=%d, hit=%d, avo=%d, crt=%d, dog=%d, sil=%d",
                    attacker, defender,
                    it->wtype_a, it->wtype_b,
                    it->bonus_atk, it->bonus_def, it->bonus_speed, it->bonus_hit,
                    it->bonus_avoid, it->bonus_crit, it->bonus_dodge, it->bonus_silencer);

            if (BattleSkillTester(attacker, it->sid))
            {
                attacker->battleAttack       += it->bonus_atk;
                attacker->battleDefense      += it->bonus_def;
                attacker->battleSpeed        += it->bonus_speed;
                attacker->battleHitRate      += it->bonus_hit;
                attacker->battleAvoidRate    += it->bonus_avoid;
                attacker->battleCritRate     += it->bonus_crit;
                attacker->battleDodgeRate    += it->bonus_dodge;
                attacker->battleSilencerRate += it->bonus_silencer;

                /* Just for UI */
                if (it->is_buff)
                {
                    attacker->wTriangleHitBonus += 15 * multiplier;
                    attacker->wTriangleDmgBonus += 1 * multiplier;
                    defender->wTriangleHitBonus -= 15 * multiplier;
                    defender->wTriangleDmgBonus -= 1 * multiplier;
                }
                else
                {
                    attacker->wTriangleHitBonus -= 15 * multiplier;
                    attacker->wTriangleDmgBonus -= 1 * multiplier;
                    defender->wTriangleHitBonus += 15 * multiplier;
                    defender->wTriangleDmgBonus += 1 * multiplier;
                }
            }
            break;
        }
    }
}

/* LynJump */
void BattleApplyWeaponTriangleEffect(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    /*
     * Idea:
     * Since vanilla WTA bonus can only get hit & dmg bonus.
     * So here we null the vanilla WTA bonus
     * But just calculate in Pre-Battle calc (PreBattleCalcWeaponTriangle)
     * now WTA-bonus in BattleUnit struct is just for BkSel UI.
     */
    return;
}

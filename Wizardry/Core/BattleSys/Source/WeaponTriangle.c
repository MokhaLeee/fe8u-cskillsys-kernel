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
    bool invert = false;

    int ui, atk, def, hit, avo, crt, sil;

    ui  = 0;
    atk = 0;
    def = 0;
    hit = 0;
    avo = 0;
    crt = 0;
    sil = 0;

// We run twice in case both units have nonconforming, so we revert back to the standard boosts
#if (defined(SID_Nonconforming) && (COMMON_SKILL_VALID(SID_Nonconforming)))
    if (BattleSkillTester(attacker, SID_Nonconforming))
        invert = !invert;
    if (BattleSkillTester(defender, SID_Nonconforming))
        invert = !invert;
#endif

    if (item_conf->valid && item_conf->wtype == defender->weaponType)
    {
        ui  -= item_conf->is_buff ? +1 : -1;

        atk -= item_conf->battle_status.atk;
        def -= item_conf->battle_status.def;
        hit -= item_conf->battle_status.hit;
        avo -= item_conf->battle_status.avo;
        crt -= item_conf->battle_status.crit;
        sil -= item_conf->battle_status.silencer;
    }
    else
    {
        /* vanilla */
        const struct WeaponTriangleRule * it;
        for (it = sWeaponTriangleRules; it->attackerWeaponType >= 0; ++it)
        {
            if ((attacker->weaponType == it->attackerWeaponType) && (defender->weaponType == it->defenderWeaponType))
            {
                ui = it->atkBonus > 0 ? +1 : -1;
                atk += it->atkBonus;
                hit += it->hitBonus;
                break;
            }
        }
    }

    for (it = gpWeaponTriangleConfs; it->wtype_a != it->wtype_b; it++)
    {
        if (it->wtype_a == attacker->weaponType && it->wtype_b == defender->weaponType)
        {
            if (BattleSkillTester(attacker, it->sid))
            {
                ui  -= item_conf->is_buff ? +1 : -1;

                atk -= item_conf->battle_status.atk;
                def -= item_conf->battle_status.def;
                hit -= item_conf->battle_status.hit;
                avo -= item_conf->battle_status.avo;
                crt -= item_conf->battle_status.crit;
                sil -= item_conf->battle_status.silencer;
            }
            break;
        }
    }

    if
    (
#if (defined(SID_TriangleAdept) && (COMMON_SKILL_VALID(SID_TriangleAdept)))
        BattleSkillTester(attacker, SID_TriangleAdept)
#else
        0
#endif
        ||
#if (defined(SID_TriangleAdeptPlus) && (COMMON_SKILL_VALID(SID_TriangleAdeptPlus)))
        BattleSkillTester(attacker, SID_TriangleAdeptPlus) || BattleSkillTester(defender, SID_TriangleAdeptPlus)
#else
        0
#endif
    )
    {
        ui  *= 2;

        atk *= 2;
        def *= 2;
        hit *= 2;
        avo *= 2;
        crt *= 2;
        sil *= 2;
    }

    if (invert)
    {
        attacker->battleAttack       += atk;
        attacker->battleDefense      += def;
        attacker->battleHitRate      += hit;
        attacker->battleAvoidRate    += avo;
        attacker->battleCritRate     += crt;
        attacker->battleSilencerRate += sil;

        attacker->wTriangleHitBonus  += ui;
        attacker->wTriangleDmgBonus  += ui;
        defender->wTriangleHitBonus  -= ui;
        defender->wTriangleDmgBonus  -= ui;
    }
    else
    {
        attacker->battleAttack       -= atk;
        attacker->battleDefense      -= def;
        attacker->battleHitRate      -= hit;
        attacker->battleAvoidRate    -= avo;
        attacker->battleCritRate     -= crt;
        attacker->battleSilencerRate -= sil;

        attacker->wTriangleHitBonus  -= ui;
        attacker->wTriangleDmgBonus  -= ui;
        defender->wTriangleHitBonus  += ui;
        defender->wTriangleDmgBonus  += ui;
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

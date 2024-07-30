#include "common-chax.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "battle-system.h"
#include "strmag.h"
#include "debuff.h"
#include "unit-expa.h"
#include "kernel-lib.h"
#include "combat-art.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"
#include "constants/combat-arts.h"

STATIC_DECLAR bool CheckSkillHpDrain(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    if (gBattleTemporaryFlag.skill_activated_aether)
        return true;

#if (defined(SID_Sol) && (COMMON_SKILL_VALID(SID_Sol)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Sol, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Sol);
        return true;
    }
#endif

    return false;
}

STATIC_DECLAR bool CheckSkillHpHalve(struct BattleUnit * attacker, struct BattleUnit * defender)
{

#if (defined(SID_Eclipse) && (COMMON_SKILL_VALID(SID_Eclipse)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Eclipse, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Eclipse);
        return true;
    }
#endif

    return false;
}

/* LynJump */
void BattleUpdateBattleStats(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int attack = attacker->battleAttack;
    int defense = defender->battleDefense;
    int hitRate = attacker->battleEffectiveHitRate;
    int critRate = attacker->battleEffectiveCritRate;
    int silencerRate = attacker->battleSilencerRate;

    /* Fasten simulation */
    if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
    {
        gBattleStats.attack = attack;
        gBattleStats.defense = defense;
        gBattleStats.hitRate = hitRate;
        gBattleStats.critRate = critRate;
        gBattleStats.silencerRate = silencerRate;
        return;
    }

#if defined(SID_AxeFaith) && (COMMON_SKILL_VALID(SID_AxeFaith))
    if (attacker->weaponType == ITYPE_AXE && CheckBattleSkillActivate(attacker, defender, SID_AxeFaith, attacker->battleAttack))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_AxeFaith);
        hitRate += attacker->battleAttack;
    }
#endif

    gBattleTemporaryFlag.skill_activated_sure_shoot = false;

#if (defined(SID_SureShot) && (COMMON_SKILL_VALID(SID_SureShot)))
    if (CheckBattleSkillActivate(attacker, defender, SID_SureShot, attacker->unit.skl))
    {
        gBattleTemporaryFlag.skill_activated_sure_shoot = true;
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_SureShot);
        hitRate = 100;
    }
#endif

    gBattleTemporaryFlag.skill_activated_dead_eye = false;

#if defined(SID_Deadeye) && (COMMON_SKILL_VALID(SID_Deadeye))
    if (CheckBattleSkillActivate(attacker, defender, SID_Deadeye, attacker->unit.skl))
    {
        gBattleTemporaryFlag.skill_activated_dead_eye = true;
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Deadeye);
        hitRate *= 2;
    }
#endif

    LIMIT_AREA(gBattleStats.attack, 0, 255);
    LIMIT_AREA(gBattleStats.defense, 0, 255);
    LIMIT_AREA(gBattleStats.hitRate, 0, 100);
    LIMIT_AREA(gBattleStats.critRate, 0, 100);
    LIMIT_AREA(gBattleStats.silencerRate, 0, 100);

    gBattleStats.attack = attack;
    gBattleStats.defense = defense;
    gBattleStats.hitRate = hitRate;
    gBattleStats.critRate = critRate;
    gBattleStats.silencerRate = silencerRate;
}

int CalcBattleRealDamage(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int damage = 0;

#if defined(SID_RuinedBlade) && (COMMON_SKILL_VALID(SID_RuinedBlade))
    if (BattleSkillTester(attacker, SID_RuinedBlade))
        damage += SKILL_EFF2(SID_RuinedBlade);
#endif

#if defined(SID_RuinedBladePlus) && (COMMON_SKILL_VALID(SID_RuinedBladePlus))
    if (BattleSkillTester(attacker, SID_RuinedBladePlus))
        damage += SKILL_EFF1(SID_RuinedBladePlus);
#endif

#if defined(SID_LunaAttack) && (COMMON_SKILL_VALID(SID_LunaAttack))
    if (BattleSkillTester(attacker, SID_LunaAttack))
        damage += defender->battleDefense / 4;
#endif

#if defined(SID_Bushido) && (COMMON_SKILL_VALID(SID_Bushido))
    if (BattleSkillTester(attacker, SID_Bushido))
        damage += SKILL_EFF1(SID_Bushido);
#endif

    return damage;
}

STATIC_DECLAR int BattleHit_CalcDamage(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    bool crit_atk;
    int result, damage_base, attack, defense;
    int correction, real_damage, increase, decrease, crit_correction;

    FORCE_DECLARE struct BattleGlobalFlags * act_flags, * tar_flags;

    /**
     * result = ([atk + correction - def])
     *              * (100% + increase%)
     *              * (100% + crit_correction%)
     *              / (100% + decrease%)
     *              + real_damage
     */

    if (attacker == &gBattleActor)
    {
        act_flags = &gBattleActorGlobalFlag;
        tar_flags = &gBattleTargetGlobalFlag;
    }
    else
    {
        act_flags = &gBattleTargetGlobalFlag;
        tar_flags = &gBattleActorGlobalFlag;
    }

    /**
     * Step0: Roll critical and silencer attack
     */
    crit_atk = false;

    if (
#if defined(SID_Fortune) && (COMMON_SKILL_VALID(SID_Fortune))
        !BattleSkillTester(defender, SID_Fortune)
#else
        (1)
#endif
        &&
#if defined(SID_Foresight) && (COMMON_SKILL_VALID(SID_Foresight))
        !BattleSkillTester(defender, SID_Foresight)
#else
        (1)
#endif
    )
    {
        if (BattleRoll1RN(gBattleStats.critRate, false))
        {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
            crit_atk = true;

            if (BattleRoll1RN(gBattleStats.silencerRate, false))
            {
                /* Directly return on silencer attack to fasten calc */
                gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_SILENCER;
                return BATTLE_MAX_DAMAGE;
            }
        }
    }

    /**
     * Step1: Calculate real damage
     */
    real_damage = CalcBattleRealDamage(attacker, defender);
    if (real_damage > 0 && gBattleStats.config & BATTLE_CONFIG_REAL)
        TriggerKtutorial(KTUTORIAL_REAL_DAMAGE);

    /**
     * Step2: Calculate base damage (atk + correction - def)
     */
    attack = gBattleStats.attack;
    defense = gBattleStats.defense;
    correction = 0;

    if (attacker == &gBattleActor)
    {
        /**
         * I don't think it's a good idea to put calculation here.
         *
         * Also for the combat-art, it is better to judge on round count,
         * so that it can only take effect on first strike.
         *
         * Later we may optimize it.
         */
        switch (GetCombatArtInForce(&attacker->unit)) {
        case CID_Detonate:
            if (!(GetItemAttributes(attacker->weapon) & IA_UNBREAKABLE))
                defense = 0;

            break;

        default:
            break;
        }
    }

#if (defined(SID_Flare) && (COMMON_SKILL_VALID(SID_Flare)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Flare, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Flare);
        defense = defense / 2;
    }
#endif

    if (IsMagicAttack(attacker))
    {
#if (defined(SID_Corona) && (COMMON_SKILL_VALID(SID_Corona)))
        if (CheckBattleSkillActivate(attacker, defender, SID_Corona, attacker->unit.skl))
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Corona);
            defense = 0;
        }
#endif
    }
    else
    {
#if (defined(SID_Luna) && (COMMON_SKILL_VALID(SID_Luna)))
        if (CheckBattleSkillActivate(attacker, defender, SID_Luna, attacker->unit.skl))
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Luna);
            defense = 0;
        }
#endif
    }

#if (defined(SID_Ignis) && (COMMON_SKILL_VALID(SID_Ignis)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Ignis, attacker->unit.skl))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Ignis);
        switch (attacker->weaponType) {
        case ITYPE_SWORD:
        case ITYPE_LANCE:
        case ITYPE_AXE:
        case ITYPE_BOW:
            correction += attacker->unit.def / 2;
            break;

        case ITYPE_ANIMA:
        case ITYPE_LIGHT:
        case ITYPE_DARK:
            correction += attacker->unit.res / 2;
            break;
        default:
            break;
        }
    }
#endif

    gBattleTemporaryFlag.skill_activated_aether = false;

#if (defined(SID_Aether) && (COMMON_SKILL_VALID(SID_Aether)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Aether, attacker->unit.skl))
    {
        gBattleTemporaryFlag.skill_activated_aether = true;
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Aether);
        correction += defender->battleDefense * 4 / 5;
    }
#endif

#if defined(SID_Glacies) && (COMMON_SKILL_VALID(SID_Glacies))
    if (CheckBattleSkillActivate(attacker, defender, SID_Glacies, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Glacies);
        correction += attacker->unit.res;
    }
#endif

#if defined(SID_Vengeance) && (COMMON_SKILL_VALID(SID_Vengeance))
    if (CheckBattleSkillActivate(attacker, defender, SID_Vengeance, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Vengeance);
        correction += (attacker->unit.maxHP - attacker->unit.curHP);
    }
#endif

#if (defined(SID_Spurn) && (COMMON_SKILL_VALID(SID_Spurn)))
        if (BattleSkillTester(attacker, SID_Spurn) && crit_atk && (attacker->hpInitial * 4) < (attacker->unit.maxHP * 3))
            correction += SKILL_EFF0(SID_Spurn);
#endif

#if (defined(SID_DragonWarth) && (COMMON_SKILL_VALID(SID_DragonWarth)))
    if (BattleSkillTester(attacker, SID_DragonWarth) && act_flags->round_cnt_hit == 1)
        correction += attack * SKILL_EFF0(SID_DragonWarth) / 100;
#endif

    /**
     * Step2.1: fasten calculation!
     */
    damage_base = attack + correction - defense;
    if (damage_base <= 0)
        damage_base = 0;

    if (damage_base <= 0 && real_damage <= 0)
    {
        /* If no damage taken, directly end the damage calculation */
        return 0;
    }
    else
    {
#if (defined(SID_GreatShield) && (COMMON_SKILL_VALID(SID_GreatShield)))
        if (CheckBattleSkillActivate(defender, attacker, SID_GreatShield, defender->unit.skl))
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_GreatShield);
            return 0;
        }
#endif

#if (defined(SID_TowerShieldPlus) && (COMMON_SKILL_VALID(SID_TowerShieldPlus)))
    if (BattleSkillTester(defender, SID_TowerShieldPlus))
    {
        if(gBattleStats.range > 1)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_TowerShieldPlus); 
            return 0;
        }
    }
#endif
        if (IsMagicAttack(attacker))
        {
#if (defined(SID_Aegis) && (COMMON_SKILL_VALID(SID_Aegis)))
            if (CheckBattleSkillActivate(defender, attacker, SID_Aegis, defender->unit.skl))
            {
                RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Aegis);
                return 0;
            }
#endif
        }
        else
        {
#if (defined(SID_Pavise) && (COMMON_SKILL_VALID(SID_Pavise)))
            if (CheckBattleSkillActivate(defender, attacker, SID_Pavise, defender->unit.skl))
            {
                RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Pavise);
                return 0;
            }
#endif
        }
    }

    /**
     * Step3: Calculate damage increase amplifier (100% + increase%)
     */
    increase = 100;

#if defined(SID_DragonFang) && (COMMON_SKILL_VALID(SID_DragonFang))
    if (CheckBattleSkillActivate(attacker, defender, SID_DragonFang, attacker->unit.skl * 2))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DragonFang);
        increase += SKILL_EFF0(SID_DragonFang);
    }
#endif

#if (defined(SID_Colossus) && (COMMON_SKILL_VALID(SID_Colossus)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Colossus, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Colossus);
        increase += 10 * SKILL_EFF0(SID_Colossus);
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
    }
#endif

#if defined(SID_Impale) && (COMMON_SKILL_VALID(SID_Impale))
    if (CheckBattleSkillActivate(attacker, defender, SID_Impale, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Impale);
        increase += 10 * SKILL_EFF0(SID_Impale);
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
    }
#endif


#if defined(SID_SolarPower) && (COMMON_SKILL_VALID(SID_SolarPower))
    if (BattleSkillTester(attacker, SID_SolarPower))
        if(gPlaySt.chapterWeatherId == WEATHER_FLAMES && IsMagicAttack(attacker))
            increase += SKILL_EFF0(SID_SolarPower);
#endif

    if (gBattleTemporaryFlag.skill_activated_sure_shoot)
        increase += 50;

    /**
     * Step4: Calculate critial damage amplifier (200%  + crit_correction%)
     */
    crit_correction = 100;
    if (crit_atk)
    {
        crit_correction = 300;

#if defined(SID_InfinityEdge) && (COMMON_SKILL_VALID(SID_InfinityEdge))
        if (BattleSkillTester(attacker, SID_InfinityEdge))
            crit_correction += SKILL_EFF0(SID_InfinityEdge);
#endif

#if (defined(SID_Gambit) && (COMMON_SKILL_VALID(SID_Gambit)))
        if (BattleSkillTester(defender, SID_Gambit) && gBattleStats.range == 1)
            crit_correction -= SKILL_EFF0(SID_Gambit);
#endif

#if (defined(SID_MagicGambit) && (COMMON_SKILL_VALID(SID_MagicGambit)))
        if (BattleSkillTester(defender, SID_MagicGambit) && gBattleStats.range > 1)
            crit_correction -= SKILL_EFF0(SID_MagicGambit);
#endif
    }

    /**
     * Step5: Calculate damage decrease amplifier (100% + decrease%)
     */
    decrease = 0x100;

#if defined(SID_Astra) && (COMMON_SKILL_VALID(SID_Astra))
    if (attacker == &gBattleActor && BattleSkillTester(attacker, SID_Astra) && gBattleActorGlobalFlag.skill_activated_astra)
        decrease += DAMAGE_DECREASE(SKILL_EFF1(SID_Astra));
#endif

#if (defined(SID_DragonSkin) && (COMMON_SKILL_VALID(SID_DragonSkin)))
    if (BattleSkillTester(defender, SID_DragonSkin))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DragonSkin);
        decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_DragonSkin));
    }
#endif

#if (defined(SID_Multiscale) && (COMMON_SKILL_VALID(SID_Multiscale)))
    if (BattleSkillTester(defender, SID_Multiscale))
    {
        if(defender->unit.curHP == defender->unit.maxHP)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Multiscale);
            decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_Multiscale));
        }
    }
#endif

#if defined(SID_Expertise) && (COMMON_SKILL_VALID(SID_Expertise))
    if (BattleSkillTester(defender, SID_Expertise) && crit_atk)
        decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_Expertise));
#endif

#if (defined(SID_KeenFighter) && (COMMON_SKILL_VALID(SID_KeenFighter)))
    if (BattleSkillTester(defender, SID_KeenFighter) && CheckCanTwiceAttackOrder(attacker, defender))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_KeenFighter);
        decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_KeenFighter));
    }
#endif

#if defined(SID_GuardBearing) && (COMMON_SKILL_VALID(SID_GuardBearing))
    if (BattleSkillTester(defender, SID_GuardBearing))
    {
        if (!AreUnitsAllied(defender->unit.index, gPlaySt.faction) &&
            act_flags->round_cnt_hit == 1 &&
            !CheckBitUES(&defender->unit, UES_BIT_GUARDBEAR_SKILL_USED))
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_GuardBearing);
            SetBitUES(&defender->unit, UES_BIT_GUARDBEAR_SKILL_USED);
            decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_GuardBearing));
        }
    }
#endif

#if (defined(SID_BeastAssault) && (COMMON_SKILL_VALID(SID_BeastAssault)))
    if (BattleSkillTester(defender, SID_BeastAssault))
        decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_BeastAssault));
#endif

#if (defined(SID_Spurn) && (COMMON_SKILL_VALID(SID_Spurn)))
    if (BattleSkillTester(defender, SID_Spurn))
    {
        int _diff = defender->battleSpeed - attacker->battleSpeed;
        LIMIT_AREA(_diff, 0, 10);

        decrease += DAMAGE_DECREASE(_diff * SKILL_EFF1(SID_Spurn));
    }
#endif

#if (defined(SID_Bushido) && (COMMON_SKILL_VALID(SID_Bushido)))
    if (BattleSkillTester(defender, SID_Bushido))
    {
        int _diff = defender->battleSpeed - attacker->battleSpeed;
        LIMIT_AREA(_diff, 0, 10);

        decrease += DAMAGE_DECREASE(_diff * SKILL_EFF0(SID_Bushido));
    }
#endif

#if (defined(SID_DragonWall) && (COMMON_SKILL_VALID(SID_DragonWall)))
    if (BattleSkillTester(defender, SID_DragonWall))
    {
        int _diff = defender->unit.res - attacker->unit.res;
        LIMIT_AREA(_diff, 0, 10);

        decrease += DAMAGE_DECREASE(_diff * SKILL_EFF0(SID_DragonWall));
    }
#endif

#if (defined(SID_BlueLionRule) && (COMMON_SKILL_VALID(SID_BlueLionRule)))
    if (BattleSkillTester(defender, SID_BlueLionRule))
    {
        int _diff = defender->unit.def - attacker->unit.def;
        LIMIT_AREA(_diff, 0, 10);

        decrease += DAMAGE_DECREASE(_diff * SKILL_EFF0(SID_BlueLionRule));
    }
#endif

#if (defined(SID_CounterRoar) && (COMMON_SKILL_VALID(SID_CounterRoar)))
    if (BattleSkillTester(defender, SID_CounterRoar))
    {
        if (act_flags->round_cnt_hit == 1)
            decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_CounterRoar));
        else
            decrease += DAMAGE_DECREASE(SKILL_EFF1(SID_CounterRoar));
    }
#endif

#if (defined(SID_DragonWarth) && (COMMON_SKILL_VALID(SID_DragonWarth)))
    if (BattleSkillTester(defender, SID_DragonWarth) && tar_flags->round_cnt_hit == 1)
        decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_DragonWarth));
#endif

#if (defined(SID_CrusaderWard) && (COMMON_SKILL_VALID(SID_CrusaderWard)))
    if (BattleSkillTester(defender, SID_CrusaderWard) &&
        tar_flags->round_cnt_hit > 1 &&
        gBattleStats.range > 1)
        decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_CrusaderWard));
#endif

    /**
     * Step6: Calculate result
     */
    {
        u32 dividend, divisor, quotient;

        dividend = damage_base * increase * crit_correction * 0x100;
        divisor  = 100 * 100 * decrease;

        quotient = DIV_ROUND_CLOSEST(dividend, divisor);
        Debugf("dividend=%ld, divisor=%ld, quotient=%ld", dividend, divisor, quotient);
        result = quotient;
    }

    if (result == 0 && damage_base > 0)
        result = 1; // at least 1 damage left.

    result += real_damage;

    Printf("[round %d] dmg=%d: base=%d (atk=%d, def=%d, cor=%d), inc=%d, crt=%d, dec=%d, real=%d",
                    GetBattleHitRound(gBattleHitIterator), result, damage_base,
                    attack, defense, correction, increase, crit_correction, decrease, real_damage);

    if (result > BATTLE_MAX_DAMAGE)
        result = BATTLE_MAX_DAMAGE;

    return result;
}

STATIC_DECLAR bool CheckDevilAttack(struct BattleUnit * attacker, struct BattleUnit * defender)
{

#if (defined(SID_Counter) && (COMMON_SKILL_VALID(SID_Counter)))
    if (BattleSkillTester(defender, SID_Counter) && gBattleStats.range == 1 && !IsMagicAttack(attacker))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Counter);
        return true;
    }
#endif

#if (defined(SID_CounterMagic) && (COMMON_SKILL_VALID(SID_CounterMagic)))
    if (BattleSkillTester(defender, SID_CounterMagic) && gBattleStats.range >= 2 && IsMagicAttack(attacker))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_CounterMagic);
        return true;
    }
#endif

    if (!BattleRoll1RN(31 - attacker->unit.lck, FALSE))
    {
        /* Lucky */
        return false;
    }

#if (defined(SID_DevilsLuck) && (COMMON_SKILL_VALID(SID_DevilsLuck)))
    if (BattleSkillTester(defender, SID_DevilsLuck) && GetItemWeaponEffect(defender->weapon) == WPN_EFFECT_DEVIL)
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DevilsLuck);
        return true;
    }
    if (BattleSkillTester(attacker, SID_DevilsLuck) && GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_DEVIL)
        return false;
#endif

    if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_DEVIL)
        return true;

#if (defined(SID_DevilsPact) && (COMMON_SKILL_VALID(SID_DevilsPact)))
    if (BattleSkillTester(defender, SID_DevilsPact))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DevilsPact);
        return true;
    }
#endif

#if (defined(SID_DevilsWhim) && (COMMON_SKILL_VALID(SID_DevilsWhim)))
    if (BattleSkillTester(defender, SID_DevilsWhim))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DevilsWhim);
        return true;
    }

    if (BattleSkillTester(attacker, SID_DevilsWhim))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DevilsWhim);
        return true;
    }
#endif

    return false;
}

/* LynJump */
void BattleGenerateHitAttributes(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    gBattleStats.damage = 0;

    /* Fasten simulation */
    if (!BattleRoll2RN(gBattleStats.hitRate, FALSE))
    {
#if (defined(SID_DivinePulse) && (COMMON_SKILL_VALID(SID_DivinePulse)))
        if (BattleRoll2RN(gBattleStats.hitRate, FALSE) &&
            CheckBattleSkillActivate(attacker, defender, SID_DivinePulse, SKILL_EFF0(SID_DivinePulse) + attacker->unit.lck))
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DivinePulse);
        }
        else
        {
            RegisterHitCnt(attacker, true);
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_MISS;
            return;
        }
#else
        RegisterHitCnt(attacker, true);
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_MISS;
        return;
#endif
    }

    RegisterHitCnt(attacker, false);

    /* Judge whether in combat-art attack */
    if (!!(gBattleStats.config & BATTLE_CONFIG_REAL) && attacker == &gBattleActor && COMBART_VALID(GetCombatArtInForce(&gBattleActor.unit)))
        TriggerKtutorial(KTUTORIAL_COMBATART_MENU);

    gBattleStats.damage = BattleHit_CalcDamage(attacker, defender);

    BattleCheckPetrify(attacker, defender);

    if (gBattleStats.damage != 0)
        attacker->nonZeroDamage = TRUE;
}

/* LynJump */
void BattleGenerateHitEffects(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int debuff;
    bool weapon_cost;

#if (defined(SID_Discipline) && (COMMON_SKILL_VALID(SID_Discipline)))
        if (BattleSkillTester(attacker, SID_Discipline))
            attacker->wexpMultiplier += 2;
        else
            attacker->wexpMultiplier++;
#else
        attacker->wexpMultiplier++;
#endif

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS))
    {
        if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPHALVE || CheckSkillHpHalve(attacker, defender))
        {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPHALVE;
            gBattleStats.damage = defender->unit.curHP / 2;
        }

        if (CheckDevilAttack(attacker, defender))
        {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_DEVIL;

            attacker->unit.curHP -= gBattleStats.damage;

            if (attacker->unit.curHP < 0)
                attacker->unit.curHP = 0;
        }
        else
        {
            if (gBattleStats.damage > defender->unit.curHP)
                gBattleStats.damage = defender->unit.curHP;

#if defined(SID_Bane) && (COMMON_SKILL_VALID(SID_Bane))
            if (gBattleStats.damage < (defender->unit.curHP - 1))
            {
                if (CheckBattleSkillActivate(attacker, defender, SID_Bane, attacker->unit.skl))
                {
                    RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Bane);
                    gBattleStats.damage = defender->unit.curHP - 1;
                    gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
                }
            }
#endif
            defender->unit.curHP -= gBattleStats.damage;

            if (defender->unit.curHP < 0)
                defender->unit.curHP = 0;
        }

#ifdef CHAX
        if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPDRAIN || CheckSkillHpDrain(attacker, defender))
#else
        if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPDRAIN)
#endif
        {
            if (attacker->unit.maxHP < (attacker->unit.curHP + gBattleStats.damage))
                attacker->unit.curHP = attacker->unit.maxHP;
            else
                attacker->unit.curHP += gBattleStats.damage;

            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPSTEAL;
        }

        if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_PETRIFY)
        {
            switch (gPlaySt.faction) {
            case FACTION_BLUE:
                if (UNIT_FACTION(&defender->unit) == FACTION_BLUE)
                    defender->statusOut = UNIT_STATUS_13;
                else
                    defender->statusOut = UNIT_STATUS_PETRIFY;
                break;

            case FACTION_RED:
                if (UNIT_FACTION(&defender->unit) == FACTION_RED)
                    defender->statusOut = UNIT_STATUS_13;
                else
                    defender->statusOut = UNIT_STATUS_PETRIFY;
                break;

            case FACTION_GREEN:
                if (UNIT_FACTION(&defender->unit) == FACTION_GREEN)
                    defender->statusOut = UNIT_STATUS_13;
                else
                    defender->statusOut = UNIT_STATUS_PETRIFY;
                break;
            }
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_PETRIFY;
        }
#if (defined(SID_Petrify) && (COMMON_SKILL_VALID(SID_Petrify)))
        else if (CheckBattleSkillActivate(attacker, defender, SID_Petrify, attacker->unit.skl))
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Petrify);
            defender->statusOut = UNIT_STATUS_PETRIFY;
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_PETRIFY;
        }
#endif
        else if (gBattleTemporaryFlag.skill_activated_dead_eye)
        {
            defender->statusOut = UNIT_STATUS_SLEEP;
        }
        else if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_POISON)
        {
            defender->statusOut = UNIT_STATUS_POISON;
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_POISON;

            // "Ungray" defender if it was petrified (as it won't be anymore)
            debuff = GetUnitStatusIndex(&defender->unit);
            if (debuff == UNIT_STATUS_PETRIFY || debuff == UNIT_STATUS_13)
                defender->unit.state = defender->unit.state &~ US_UNSELECTABLE;
        }
#if (defined(SID_PoisonPoint) && (COMMON_SKILL_VALID(SID_PoisonPoint)))
        else if (BattleSkillTester(attacker, SID_PoisonPoint))
        {
            defender->statusOut = UNIT_STATUS_POISON;
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_POISON;

            // "Ungray" defender if it was petrified (as it won't be anymore)
            debuff = GetUnitStatusIndex(&defender->unit);
            if (debuff == UNIT_STATUS_PETRIFY || debuff == UNIT_STATUS_13)
                defender->unit.state = defender->unit.state &~ US_UNSELECTABLE;
        }
#endif
#if (defined(SID_Enrage) && (COMMON_SKILL_VALID(SID_Enrage)))
        else if (CheckBattleSkillActivate(attacker, defender, SID_Enrage, attacker->unit.skl))
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Enrage);
            defender->statusOut = UNIT_STATUS_BERSERK;
        }
#endif
    }

    gBattleHitIterator->hpChange = gBattleStats.damage;

    /**
     * Consume enemy weapons
     */
#if (defined(SID_Corrosion) && (COMMON_SKILL_VALID(SID_Corrosion)))
    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS) && CheckBattleSkillActivate(attacker, defender, SID_Corrosion, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Corrosion);
        int cost = attacker->levelPrevious;
    
        while (cost-- > 0)
        {
            u16 weapon = GetItemAfterUse(defender->weapon);
            defender->weapon = weapon;

            if (!weapon)
                break;
        }

        if (!defender->weapon)
            defender->weaponBroke = TRUE;
    }
#endif

    /**
     * Consumes the durability of the own weapon
     */
    weapon_cost = false;
    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS))
        weapon_cost = true;
    else if (attacker->weaponAttributes & (IA_UNCOUNTERABLE | IA_MAGIC))
        weapon_cost = true;

#if defined(SID_Armsthrift) && (COMMON_SKILL_VALID(SID_Armsthrift))
    if (CheckBattleSkillActivate(attacker, defender, SID_Armsthrift, attacker->unit.lck))
    {
        weapon_cost = false;
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Armsthrift);
    }
#endif

    if (weapon_cost)
    {
#ifdef CHAX
        /* Check on combat-art */
        int cost = GetWeaponCost(attacker, attacker->weapon);
        while (cost-- > 0)
        {
            u16 weapon = GetItemAfterUse(attacker->weapon);
            attacker->weapon = weapon;

            if (!weapon)
                break;
        }
#else
        attacker->weapon = GetItemAfterUse(attacker->weapon);
#endif
        if (!attacker->weapon)
            attacker->weaponBroke = TRUE;
    }
}

STATIC_DECLAR bool InoriCheck(struct BattleUnit * attacker, struct BattleUnit * defender)
{
#if (defined(SID_Mercy) && (COMMON_SKILL_VALID(SID_Mercy)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Mercy, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Mercy);
        return true;
    }
#endif

#if (defined(SID_Inori) && (COMMON_SKILL_VALID(SID_Inori)))
    if (CheckBattleSkillActivate(defender, attacker, SID_Inori, defender->unit.lck))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Inori);
        return true;
    }
#endif

#if (defined(SID_LEGEND_InoriAtk) && (COMMON_SKILL_VALID(SID_LEGEND_InoriAtk)))
    if (CheckBattleSkillActivate(defender, attacker, SID_LEGEND_InoriAtk, 100))
    {
        if (TryActivateLegendSkill(&defender->unit, SID_LEGEND_InoriAtk) == 0)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LEGEND_InoriAtk);
            return true;
        }
    }
#endif

#if (defined(SID_LEGEND_InoriAvo) && (COMMON_SKILL_VALID(SID_LEGEND_InoriAvo)))
    if (CheckBattleSkillActivate(defender, attacker, SID_LEGEND_InoriAvo, 100))
    {
        if (TryActivateLegendSkill(&defender->unit, SID_LEGEND_InoriAvo) == 0)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LEGEND_InoriAvo);
            return true;
        }
    }
#endif

#if (defined(SID_LEGEND_InoriDef) && (COMMON_SKILL_VALID(SID_LEGEND_InoriDef)))
    if (CheckBattleSkillActivate(defender, attacker, SID_LEGEND_InoriDef, 100))
    {
        if (TryActivateLegendSkill(&defender->unit, SID_LEGEND_InoriDef) == 0)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LEGEND_InoriDef);
            return true;
        }
    }
#endif

    return false;
}

/* LynJump */
bool BattleGenerateHit(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    if (attacker == &gBattleTarget)
        gBattleHitIterator->info |= BATTLE_HIT_INFO_RETALIATION;

    BattleUpdateBattleStats(attacker, defender);

    BattleGenerateHitTriangleAttack(attacker, defender);
    BattleGenerateHitAttributes(attacker, defender);
    BattleGenerateHitEffects(attacker, defender);

    if (attacker->unit.curHP == 0 || defender->unit.curHP == 0)
    {
#if (defined(SID_Discipline) && (COMMON_SKILL_VALID(SID_Discipline)))
        if (BattleSkillTester(attacker, SID_Discipline))
            attacker->wexpMultiplier += 2;
        else
            attacker->wexpMultiplier++;
#else
        attacker->wexpMultiplier++;
#endif

        gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;

#if CHAX
        if (defender->unit.curHP == 0)
        {
            if (InoriCheck(attacker, defender))
            {
                gBattleStats.damage = gBattleStats.damage - 1;
                gBattleHitIterator->hpChange = gBattleStats.damage;
                defender->unit.curHP = 1;

                gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;
                gBattleHitIterator++;
                return true;
            }
        }
#endif

        if (gBattleTarget.unit.curHP == 0)
        {
            gBattleActorGlobalFlag.enimy_defeated = true;

#if (defined(SID_Galeforce) && (COMMON_SKILL_VALID(SID_Galeforce)))
            if (CheckBattleSkillActivate(&gBattleActor, &gBattleTarget, SID_Galeforce, gBattleActor.unit.skl))
                gBattleActorGlobalFlag.skill_activated_galeforce = true;
#endif

#if (defined(SID_Pickup) && (COMMON_SKILL_VALID(SID_Pickup)))
            if (CheckBattleSkillActivate(&gBattleActor, &gBattleTarget, SID_Pickup, gBattleActor.unit.lck))
            {
                struct Unit * unit_tar = &gBattleTarget.unit;
                unit_tar->state |= US_DROP_ITEM;
            }
#endif
            gBattleHitIterator->info |= BATTLE_HIT_INFO_KILLS_TARGET;
        }

        gBattleHitIterator++;
        return true;
    }
    else if (defender->statusOut == UNIT_STATUS_PETRIFY || defender->statusOut == UNIT_STATUS_13 || defender->statusOut == UNIT_STATUS_SLEEP)
    {
        gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;
        gBattleHitIterator++;
        return true;
    }

    gBattleHitIterator++;
    return false;
}

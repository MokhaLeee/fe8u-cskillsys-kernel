#include "common-chax.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "battle-system.h"
#include "strmag.h"
#include "debuff.h"
#include "combat-art.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"

STATIC_DECLAR bool CheckSkillHpDrain(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int unit_power  = GetUnitPower(GetUnit(attacker->unit.index));
    int unit_magic = GetUnitMagic(GetUnit(attacker->unit.index));
    int unit_skill = GetUnitSkill(GetUnit(attacker->unit.index));
    int unit_speed = GetUnitSpeed(GetUnit(attacker->unit.index));
    int unit_luck = GetUnitLuck(GetUnit(attacker->unit.index));
    int unit_defense = GetUnitDefense(GetUnit(attacker->unit.index));
    int unit_resistance = GetUnitResistance(GetUnit(attacker->unit.index));

    int activation_rates [7] = {unit_power, unit_magic, unit_skill, unit_speed, unit_luck, unit_defense, unit_resistance};

#if (defined(SID_Aether) && (SID_Aether < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(attacker, defender, SID_Aether, activation_rates[2]))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Aether);
        return true;
    }
#endif /* SID_Aether */

#if (defined(SID_Sol) && (SID_Sol < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(attacker, defender, SID_Sol, activation_rates[2]))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Sol);
        return true;
    }
#endif /* SID_Aether */

    return false;
}

int CalcBattleRealDamage(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    struct Unit * unit = GetUnit(attacker->unit.index);
    int damage = 0;

#if defined(SID_RuinedBlade) && (SID_RuinedBlade < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_RuinedBlade))
        damage += 5;
#endif // SID_RuinedBlade

#if defined(SID_RuinedBladePlus) && (SID_RuinedBladePlus < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_RuinedBladePlus))
        damage += 5;
#endif // SID_RuinedBladePlus

#if defined(SID_LunaAttack) && (SID_LunaAttack < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_LunaAttack))
        damage += defender->battleDefense / 4;
#endif // SID_LunaAttack
    return damage;
}

/* LynJump */
void BattleGenerateHitAttributes(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int attack, defense;

    int unit_power  = GetUnitPower(GetUnit(attacker->unit.index));
    int unit_magic = GetUnitMagic(GetUnit(attacker->unit.index));
    int unit_skill = GetUnitSkill(GetUnit(attacker->unit.index));
    int unit_speed = GetUnitSpeed(GetUnit(attacker->unit.index));
    int unit_luck = GetUnitLuck(GetUnit(attacker->unit.index));
    int unit_defense = GetUnitDefense(GetUnit(attacker->unit.index));
    int unit_resistance = GetUnitResistance(GetUnit(attacker->unit.index));

    int activation_rates [7] = {unit_power, unit_magic, unit_skill, unit_speed, unit_luck, unit_defense, unit_resistance};

    bool in_art_atk = false;

    struct Unit * unit = GetUnit(attacker->unit.index);
    struct Unit * unit_def = GetUnit(defender->unit.index);

    gBattleStats.damage = 0;

    /* Judge whether in combat-art attack */
    if (!!(gBattleStats.config & BATTLE_CONFIG_REAL) && attacker == &gBattleActor && COMBART_VALID(GetCombatArtInForce(&gBattleActor.unit)))
    {
        TriggerKtutorial(KTUTORIAL_COMBATART_MENU);
        in_art_atk = true;
    }

    if (!BattleRoll2RN(gBattleStats.hitRate, TRUE))
    {
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_MISS;
        return;
    }

    if (gBattleStats.config & BATTLE_CONFIG_REAL)
    {
        /**
         * Register hitted
         */
        if (attacker == &gBattleActor)
        {
            gBattleActorGlobalFlag.hitted = true;
        }
        else
        {
            gBattleTargetGlobalFlag.hitted = true;
        }

        /**
         * Well it's true that it seems no need to introduce an extra function
         * for combat-art,
         *
         * I admit this is part of shit Mountain....
         */
        if (in_art_atk)
        {
            RegisterCombatArtHitted();
        }
    }

    attack = gBattleStats.attack;
    defense = gBattleStats.defense;

#if (defined(SID_RightfulKing) && (SID_RightfulKing < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_RightfulKing))
            for (int i = 0; i < 7; i++)
            {
                activation_rates[i] += 10;
            }
#endif

#if (defined(SID_RightfulGod) && (SID_RightfulGod < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_RightfulGod))
            for (int i = 0; i < 7; i++)
            {
                activation_rates[i] += 30;
            }
#endif

#if (defined(SID_RightfulArch) && (SID_RightfulArch < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_RightfulArch))
            for (int i = 0; i < 7; i++)
            {
                activation_rates[i] = 100;
            }
#endif

#if (defined(SID_Flare) && (SID_Flare < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(attacker, defender, SID_Flare, activation_rates[2]))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Flare);
        defense = defense / 2;
    }
#endif

    if (IsMagicAttack(attacker))
    {
#if (defined(SID_Corona) && (SID_Corona < MAX_SKILL_NUM))
        if (CheckBattleSkillActivte(attacker, defender, SID_Corona, activation_rates[2]))
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Corona);
            defense = 0;
        }
#endif
    }
    else
    {
#if (defined(SID_Luna) && (SID_Luna < MAX_SKILL_NUM))
        if (CheckBattleSkillActivte(attacker, defender, SID_Luna, activation_rates[2]))
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Luna);
            defense = 0;
        }
#endif

#if (defined(SID_Colossus) && (SID_Colossus < MAX_SKILL_NUM))
        if (CheckBattleSkillActivte(attacker, defender, SID_Colossus, activation_rates[2]))
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Colossus);
            attack *= 3;
        }
#endif

#if (defined(SID_Corrosion) && (SID_Corrosion < MAX_SKILL_NUM))
        if (CheckBattleSkillActivte(attacker, defender, SID_Corrosion, activation_rates[2]))
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Corrosion);
            int cost = unit->level;
        
            while (cost-- > 0)
            {
                u16 weapon = GetItemAfterUse(defender->weapon);
                defender->weapon = weapon;

                if (!weapon)
                    break;
            }

            if (!attacker->weapon)
                attacker->weaponBroke = TRUE;
        }
#endif
    }

    gBattleStats.damage = attack - defense;

#if defined(SID_FlashingBladePlus) && (SID_FlashingBladePlus < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_FlashingBladePlus))
        gBattleStats.damage += 3;
#endif

#if defined(SID_DragonFang) && (SID_DragonFang < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_DragonFang))
        gBattleStats.damage = gBattleStats.damage * 3 / 2;
#endif

    if (BattleRoll1RN(gBattleStats.critRate, FALSE) == TRUE &&
#if defined(SID_Foresight) && (SID_Foresight < MAX_SKILL_NUM)
        !SkillTester(&defender->unit, SID_Foresight) &&
#else
        0 &&
#endif // SID_Foresight
#if defined(SID_Fortune) && (SID_Fortune < MAX_SKILL_NUM)
        !SkillTester(&defender->unit, SID_Fortune)
#else
        0
#endif // SID_Fortune
    )
    {
        if (BattleRoll1RN(gBattleStats.silencerRate, FALSE))
        {
            /* Silencer */
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_SILENCER;
            gBattleStats.damage = BATTLE_MAX_DAMAGE;
        }
        else
        {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;

#if defined(SID_InfinityEdge) && (SID_InfinityEdge < MAX_SKILL_NUM)
            /* InfinityEdge */
            if (SkillTester(unit, SID_InfinityEdge))
#else
            if (0)
#endif // SID_InfinityEdge
                gBattleStats.damage = gBattleStats.damage * 3;
            else
                gBattleStats.damage = gBattleStats.damage * 2;
        }
    }

#if defined(SID_Astra) && (SID_Astra < MAX_SKILL_NUM)
    /* SID_Astra may half the damage */
    if (attacker == &gBattleActor)
    {
        if (SkillTester(unit, SID_Astra) && gBattleActorGlobalFlag.skill_activated_astra)
            gBattleStats.damage = gBattleStats.damage / 2;
    }
#endif // SID_Astra

    /* Minus zero */
    if (gBattleStats.damage < 0)
        gBattleStats.damage = 0;

    /* Real damage */
    gBattleStats.damage += CalcBattleRealDamage(attacker, defender);

    if (gBattleStats.damage > 0)
    {
        if (IsMagicAttack(attacker))
        {
#if (defined(SID_Aegis) && (SID_Aegis < MAX_SKILL_NUM))
            if (CheckBattleSkillActivte(defender, attacker, SID_Aegis, activation_rates[2]))
            {
                RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Aegis);
                gBattleStats.damage = 0;
            }
#endif
        }
        else
        {
#if (defined(SID_Pavise) && (SID_Pavise < MAX_SKILL_NUM))
            if (CheckBattleSkillActivte(defender, attacker, SID_Pavise, activation_rates[2]))
            {
                RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Pavise);
                gBattleStats.damage = 0;
            }
#endif
        }
    }
    if (gBattleStats.damage > 0)
    {
#if (defined(SID_DragonSkin) && (SID_DragonSkin < MAX_SKILL_NUM))
        if (SkillTester(unit_def, SID_DragonSkin))
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DragonSkin);
            gBattleStats.damage = gBattleStats.damage / 2;
        }
#else
        if (0)
#endif
        else
        {
#if (defined(SID_KeenFighter) && (SID_KeenFighter < MAX_SKILL_NUM))
            if (SkillTester(unit_def, SID_KeenFighter))
            {
                if (CheckCanTwiceAttackOrder(attacker,defender))
                {
                    RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_KeenFighter);
                    gBattleStats.damage = gBattleStats.damage * 3 / 4;
                }
            }
#endif
        }
    }
    /* Post calc */
    if (gBattleStats.damage > BATTLE_MAX_DAMAGE)
        gBattleStats.damage = BATTLE_MAX_DAMAGE;

    BattleCheckPetrify(attacker, defender);

    if (gBattleStats.damage != 0)
        attacker->nonZeroDamage = TRUE;
}

/* LynJump */
void BattleGenerateHitEffects(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int debuff;

    attacker->wexpMultiplier++;

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS))
    {
        switch (GetItemWeaponEffect(attacker->weapon)) {
        case WPN_EFFECT_POISON:
            // Poison defender
            defender->statusOut = UNIT_STATUS_POISON;
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_POISON;

            // "Ungray" defender if it was petrified (as it won't be anymore)
            debuff = GetUnitStatusIndex(&defender->unit);
            if (debuff == UNIT_STATUS_PETRIFY || debuff == UNIT_STATUS_13)
                defender->unit.state = defender->unit.state &~ US_UNSELECTABLE;

            break;

        case WPN_EFFECT_HPHALVE:
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPHALVE;
            break;

        }

        if ((GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_DEVIL) && (BattleRoll1RN(31 - attacker->unit.lck, FALSE)))
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

#if (defined(SID_PoisonPoint) && (SID_PoisonPoint < MAX_SKILL_NUM))
        if (defender->statusOut == UNIT_STATUS_NONE && SkillTester(&attacker->unit, SID_PoisonPoint))
            defender->statusOut = UNIT_STATUS_POISON;
#endif
    }

    gBattleHitIterator->hpChange = gBattleStats.damage;

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS) || attacker->weaponAttributes & (IA_UNCOUNTERABLE | IA_MAGIC))
    {
#ifdef CHAX
        /* Check on combat-art */
        int cost = 1;
        if (attacker == &gBattleActor)
        {
            int cid = GetCombatArtInForce(&attacker->unit);
            if (COMBART_VALID(cid) && gpCombatArtInfos[cid].cost > 0)
                cost = gpCombatArtInfos[cid].cost;
        }

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
    int ret;
    struct Unit * uact = GetUnit(attacker->unit.index);
    struct Unit * utar = GetUnit(defender->unit.index);

    int unit_power  = GetUnitPower(GetUnit(attacker->unit.index));
    int unit_magic = GetUnitMagic(GetUnit(attacker->unit.index));
    int unit_skill = GetUnitSkill(GetUnit(attacker->unit.index));
    int unit_speed = GetUnitSpeed(GetUnit(attacker->unit.index));
    int unit_luck = GetUnitLuck(GetUnit(attacker->unit.index));
    int unit_defense = GetUnitDefense(GetUnit(attacker->unit.index));
    int unit_resistance = GetUnitResistance(GetUnit(attacker->unit.index));

    int activation_rates [7] = {unit_power, unit_magic, unit_skill, unit_speed, unit_luck, unit_defense, unit_resistance};

#if (defined(SID_Bane) && (SID_Bane < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(attacker, defender, SID_Bane, activation_rates[2]))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Bane);
        return true;
    }
#endif

#if (defined(SID_Inori) && (SID_Inori < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(defender, attacker, SID_Inori, activation_rates[2]))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Inori);
        return true;
    }
#endif

#if (defined(SID_LEGEND_InoriAtk) && (SID_LEGEND_InoriAtk < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(defender, attacker, SID_LEGEND_InoriAtk, 100))
    {
        ret = TryActivateLegendSkill(&defender->unit, SID_LEGEND_InoriAtk);
        if (ret == 0)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LEGEND_InoriAtk);
            return true;
        }
    }
#endif

#if (defined(SID_LEGEND_InoriAvo) && (SID_LEGEND_InoriAvo < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(defender, attacker, SID_LEGEND_InoriAvo, 100))
    {
        ret = TryActivateLegendSkill(&defender->unit, SID_LEGEND_InoriAvo);
        if (ret == 0)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LEGEND_InoriAvo);
            return true;
        }
    }
#endif

#if (defined(SID_LEGEND_InoriDef) && (SID_LEGEND_InoriDef < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(defender, attacker, SID_LEGEND_InoriDef, 100))
    {
        ret = TryActivateLegendSkill(&defender->unit, SID_LEGEND_InoriDef);
        if (ret == 0)
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
    int unit_power  = GetUnitPower(GetUnit(attacker->unit.index));
    int unit_magic = GetUnitMagic(GetUnit(attacker->unit.index));
    int unit_skill = GetUnitSkill(GetUnit(attacker->unit.index));
    int unit_speed = GetUnitSpeed(GetUnit(attacker->unit.index));
    int unit_luck = GetUnitLuck(GetUnit(attacker->unit.index));
    int unit_defense = GetUnitDefense(GetUnit(attacker->unit.index));
    int unit_resistance = GetUnitResistance(GetUnit(attacker->unit.index));

    int activation_rates [7] = {unit_power, unit_magic, unit_skill, unit_speed, unit_luck, unit_defense, unit_resistance};

    if (attacker == &gBattleTarget)
        gBattleHitIterator->info |= BATTLE_HIT_INFO_RETALIATION;

    BattleUpdateBattleStats(attacker, defender);

    BattleGenerateHitTriangleAttack(attacker, defender);
    BattleGenerateHitAttributes(attacker, defender);
    BattleGenerateHitEffects(attacker, defender);

    if (attacker->unit.curHP == 0 || defender->unit.curHP == 0)
    {
        attacker->wexpMultiplier++;

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
#if CHAX
            gBattleActorGlobalFlag.enimy_defeated = true;

#if (defined(SID_Galeforce) && (SID_Galeforce < MAX_SKILL_NUM))
            if (CheckBattleSkillActivte(&gBattleActor, &gBattleTarget, SID_Galeforce, activation_rates[2]))
                gBattleActorGlobalFlag.skill_activated_galeforce = true;
#endif
#endif
            gBattleHitIterator->info |= BATTLE_HIT_INFO_KILLS_TARGET;
        }

        gBattleHitIterator++;
        return true;
    }
    else if (defender->statusOut == UNIT_STATUS_PETRIFY || defender->statusOut == UNIT_STATUS_13)
    {
        gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;
        gBattleHitIterator++;
        return true;
    }

    gBattleHitIterator++;
    return false;
}

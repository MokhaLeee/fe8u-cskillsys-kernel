#include "common-chax.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "battle-system.h"
#include "strmag.h"
#include "debuff.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"

#define LOCAL_TRACE 0

void BattleHit_CalcHpDrain(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	int drain, percentage = 0;

	/**
	 * Step 1: calculate drain percentage
	 */
	if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPDRAIN) {
		percentage += gpKernelBattleDesignerConfig->nosferatu_hpdrain_perc;

		/**
		 * If the weapon itself is set as hpdrain,
		 * then it may directly call EfxHpBarResire() in banim,
		 * at which time we must set hp-steal flag for battle-parse.
		 */
		gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPSTEAL;
	}

	if (gBattleTemporaryFlag.skill_activated_aether)
		percentage += 100;

#if (defined(SID_Sol) && (COMMON_SKILL_VALID(SID_Sol)))
	if (CheckBattleSkillActivate(attacker, defender, SID_Sol, attacker->unit.skl)) {
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Sol);
		percentage += 100;
	}
#endif

	if (percentage == 0)
		return;

	/**
	 * Step 2: calculate real amount
	 */
	drain = Div(gBattleStats.damage * percentage, 100);

	LTRACEF("hpdrain: dmg=%d, perc=%d, drain=%d, cur=%d, max=%d",
			gBattleStats.damage, percentage, drain, attacker->unit.curHP, attacker->unit.maxHP);

	/**
	 * Step 3: detect overflow
	 */
	if (attacker->unit.maxHP < (attacker->unit.curHP + drain))
		drain = attacker->unit.maxHP - attacker->unit.curHP;

	if (drain > 0) {
		GetCurrentExtBattleHit()->hp_drain += drain;
		attacker->unit.curHP += drain;
		gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPSTEAL;
	}
}

bool CheckBattleHpHalve(struct BattleUnit *attacker, struct BattleUnit *defender)
{
    if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPHALVE)
        return true;

#if (defined(SID_Eclipse) && (COMMON_SKILL_VALID(SID_Eclipse)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Eclipse, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Eclipse);
        return true;
    }
#endif

    return false;
}

bool CheckDevilAttack(struct BattleUnit *attacker, struct BattleUnit *defender)
{

#if (defined(SID_Counter) && (COMMON_SKILL_VALID(SID_Counter)))
    if (BattleFastSkillTester(defender, SID_Counter) && gBattleStats.range == 1 && !IsMagicAttack(attacker))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Counter);
        return true;
    }
#endif

#if (defined(SID_CounterMagic) && (COMMON_SKILL_VALID(SID_CounterMagic)))
    if (BattleFastSkillTester(defender, SID_CounterMagic) && gBattleStats.range >= 2 && IsMagicAttack(attacker))
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
    if (BattleFastSkillTester(defender, SID_DevilsLuck) && GetItemWeaponEffect(defender->weapon) == WPN_EFFECT_DEVIL)
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DevilsLuck);
        return true;
    }
    if (BattleFastSkillTester(attacker, SID_DevilsLuck) && GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_DEVIL)
        return false;
#endif

    if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_DEVIL)
        return true;

#if (defined(SID_DevilsPact) && (COMMON_SKILL_VALID(SID_DevilsPact)))
    if (BattleFastSkillTester(defender, SID_DevilsPact))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DevilsPact);
        return true;
    }
#endif

#if (defined(SID_DevilsWhim) && (COMMON_SKILL_VALID(SID_DevilsWhim)))
    if (BattleFastSkillTester(defender, SID_DevilsWhim))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DevilsWhim);
        return true;
    }

    if (BattleFastSkillTester(attacker, SID_DevilsWhim))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DevilsWhim);
        return true;
    }
#endif

    return false;
}

bool CheckBattleInori(struct BattleUnit *attacker, struct BattleUnit *defender)
{
#if (defined(SID_Mercy) && (COMMON_SKILL_VALID(SID_Mercy)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Mercy, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Mercy);
        return true;
    }
#endif

#if (defined(SID_MercyPlus) && (COMMON_SKILL_VALID(SID_MercyPlus)))
    if (BattleFastSkillTester(attacker, SID_MercyPlus))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_MercyPlus);
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

#if (defined(SID_Sturdy) && (COMMON_SKILL_VALID(SID_Sturdy)))
    if (BattleFastSkillTester(defender, SID_Sturdy))
    {
        if (defender->unit.maxHP == defender->hpInitial)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Sturdy);
            return true;
        }
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

void BattleHit_InjectNegativeStatus(struct BattleUnit *attacker, struct BattleUnit *defender)
{
    int debuff;

    if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_PETRIFY)
    {
        switch (gPlaySt.faction)
        {
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

#if (defined(SID_Break) && (COMMON_SKILL_VALID(SID_Break)))
    else if (BattleFastSkillTester(attacker, SID_Break))
    {
        if (GetUnit(defender->unit.index)->statusIndex == UNIT_STATUS_NONE)
            SetUnitStatusIndex(GetUnit(defender->unit.index), NEW_UNIT_STATUS_BREAK);
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
            defender->unit.state = defender->unit.state & ~US_UNSELECTABLE;
    }
#if (defined(SID_PoisonPoint) && (COMMON_SKILL_VALID(SID_PoisonPoint)))
    else if (BattleFastSkillTester(attacker, SID_PoisonPoint))
    {
        defender->statusOut = UNIT_STATUS_POISON;
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_POISON;

        // "Ungray" defender if it was petrified (as it won't be anymore)
        debuff = GetUnitStatusIndex(&defender->unit);
        if (debuff == UNIT_STATUS_PETRIFY || debuff == UNIT_STATUS_13)
            defender->unit.state = defender->unit.state & ~US_UNSELECTABLE;
    }
#endif

#if (defined(SID_Enrage) && (COMMON_SKILL_VALID(SID_Enrage)))
    else if (CheckBattleSkillActivate(attacker, defender, SID_Enrage, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Enrage);
        defender->statusOut = UNIT_STATUS_BERSERK;
    }
#endif

#if (defined(SID_EffectSpore) && (COMMON_SKILL_VALID(SID_EffectSpore)))
    else if (CheckBattleSkillActivate(defender, attacker, SID_EffectSpore, SKILL_EFF0(SID_EffectSpore)))
    {
        if (!IsDebuff(GetUnitStatusIndex(&attacker->unit)) && !IsDebuff(attacker->statusOut))
        {
            static const u8 _debuffs[3] = {UNIT_STATUS_POISON, UNIT_STATUS_SILENCED, UNIT_STATUS_SLEEP};
            attacker->statusOut = _debuffs[NextRN_N(ARRAY_COUNT(_debuffs))];

            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_EffectSpore);
        }
    }
#endif

#if (defined(SID_BlackMagic) && (COMMON_SKILL_VALID(SID_BlackMagic)))
    else if (BattleFastSkillTester(defender, SID_BlackMagic))
    {
        if (!IsDebuff(GetUnitStatusIndex(&attacker->unit)) && !IsDebuff(attacker->statusOut))
        {
            static const u8 _debuffs[5] = {UNIT_STATUS_POISON, UNIT_STATUS_SILENCED, UNIT_STATUS_SLEEP, UNIT_STATUS_BERSERK, UNIT_STATUS_PETRIFY};
            attacker->statusOut = _debuffs[NextRN_N(ARRAY_COUNT(_debuffs))];

            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_BlackMagic);
        }
    }
#endif

#if (defined(SID_MagicBounce) && (COMMON_SKILL_VALID(SID_MagicBounce)))
    if (BattleFastSkillTester(defender, SID_MagicBounce))
    {
        static const u8 _debuffs[9] = {
            UNIT_STATUS_POISON,
            UNIT_STATUS_SLEEP,
            UNIT_STATUS_SILENCED,
            UNIT_STATUS_BERSERK,
            UNIT_STATUS_PETRIFY,
            NEW_UNIT_STATUS_HEAVY_GRAVITY,
            NEW_UNIT_STATUS_WEAKEN,
            NEW_UNIT_STATUS_PANIC,
            NEW_UNIT_STATUS_BREAK,
        };

        for (int i = 0; i < 9; i++)
        {
            if (defender->statusOut == _debuffs[i])
            {
                defender->statusOut = UNIT_STATUS_NONE;
                attacker->statusOut = _debuffs[i];
                break;
            }
        }

        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_MagicBounce);
    }
#endif

#if (defined(SID_Insomnia) && (COMMON_SKILL_VALID(SID_Insomnia)))
    if (BattleFastSkillTester(defender, SID_Insomnia))
        if (defender->statusOut == UNIT_STATUS_SLEEP)
            defender->statusOut = UNIT_STATUS_NONE;
#endif

#if (defined(SID_GoodAsGold) && (COMMON_SKILL_VALID(SID_GoodAsGold)))
    if (BattleFastSkillTester(defender, SID_GoodAsGold))
    {
        static const u8 _debuffs[9] = {
            UNIT_STATUS_POISON,
            UNIT_STATUS_SLEEP,
            UNIT_STATUS_SILENCED,
            UNIT_STATUS_BERSERK,
            UNIT_STATUS_PETRIFY,
            NEW_UNIT_STATUS_HEAVY_GRAVITY,
            NEW_UNIT_STATUS_WEAKEN,
            NEW_UNIT_STATUS_PANIC
            NEW_UNIT_STATUS_BREAK,
            };

        for (int i = 0; i < 9; i++)
        {
            if (defender->statusOut == _debuffs[i])
            {
                defender->statusOut = UNIT_STATUS_NONE;
                break;
            }
        }
    }
#endif

#if (defined(SID_PastelVeil) && (COMMON_SKILL_VALID(SID_PastelVeil)))
    if (defender->statusOut == UNIT_STATUS_POISON)
    {
        for (int i = 0; i < ARRAY_COUNT_RANGE3x3; i++)
        {
            int _x = GetUnit(defender->unit.index)->xPos + gVecs_3x3[i].x;
            int _y = GetUnit(defender->unit.index)->yPos + gVecs_3x3[i].y;

            struct Unit *unit_ally = GetUnitAtPosition(_x, _y);

            if (!UNIT_IS_VALID(unit_ally))
                continue;

            if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;

            if (!AreUnitsAllied(defender->unit.index, unit_ally->index))
                continue;

            if (SkillTester(unit_ally, SID_PastelVeil))
            {
                defender->statusOut = UNIT_STATUS_NONE;
                gBattleHitIterator->attributes &= ~BATTLE_HIT_ATTR_POISON;
                break;
            }
        }
    }
#endif
}

void BattleHit_ConsumeWeapon(struct BattleUnit *attacker, struct BattleUnit *defender)
{
    bool weapon_cost;

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
    else if (CheckWeaponCostForMissedBowAttack(attacker) == true)
        weapon_cost = true;

#ifdef CONFIG_INFINITE_DURABILITY
    weapon_cost = false;
#endif

#if defined(SID_Blessing) && (COMMON_SKILL_VALID(SID_Blessing))
    if (CheckBattleSkillActivate(attacker, defender, SID_Blessing, 100))
    {
        weapon_cost = false;
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Blessing);
    }
#endif

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

#include "common-chax.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "battle-system.h"
#include "strmag.h"
#include "debuff.h"
#include "combat-art.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"
#include "unit-expa.h"
#include "jester_headers/custom-structs.h"

extern u16 sBEXP[CONFIG_BEXP];

LYN_REPLACE_CHECK(BattleUpdateBattleStats);
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
    if (attacker->weaponType == ITYPE_AXE &&
        CheckBattleSkillActivate(attacker, defender, SID_AxeFaith, attacker->battleAttack))
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

LYN_REPLACE_CHECK(BattleCheckTriangleAttack);
s8 BattleCheckTriangleAttack(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    s8 adjacentLookup[] = { -1, 0, 0, -1, +1, 0, 0, +1 };

    int i, count = 0;

    int triangleAttackAttr = CA_TRIANGLEATTACK_ANY & UNIT_CATTRIBUTES(&attacker->unit);

    int x = defender->unit.xPos;
    int y = defender->unit.yPos;

    int faction = UNIT_FACTION(&attacker->unit);

    gBattleStats.taUnitA = NULL;
    gBattleStats.taUnitB = NULL;

    for (i = 0; i < 4; ++i)
    {
        int uId = gBmMapUnit[adjacentLookup[i * 2 + 1] + y][adjacentLookup[i * 2 + 0] + x];
        struct Unit * unit;

        if (!uId)
            continue;

        unit = GetUnit(uId);

        if ((uId & 0xC0) != faction)
            continue;

#if (defined(SID_TriangleAttack) && (COMMON_SKILL_VALID(SID_TriangleAttack)))
        if (SkillTester(unit, SID_TriangleAttack))
        {
            ++count;

            if (!gBattleStats.taUnitA)
                gBattleStats.taUnitA = unit;
            else if (!gBattleStats.taUnitB)
                gBattleStats.taUnitB = unit;

            continue;
        }
#endif

        if (unit->statusIndex == UNIT_STATUS_SLEEP)
            continue;

        if (unit->statusIndex == UNIT_STATUS_PETRIFY)
            continue;

        if (unit->statusIndex == UNIT_STATUS_13)
            continue;

        if (unit->pClassData->number == CLASS_WYVERN_KNIGHT_F)
            continue;

        if (UNIT_CATTRIBUTES(unit) & triangleAttackAttr)
        {
            ++count;

            if (!gBattleStats.taUnitA)
                gBattleStats.taUnitA = unit;
            else if (!gBattleStats.taUnitB)
                gBattleStats.taUnitB = unit;
        }
    }
    return count >= 2 ? TRUE : FALSE;
}

LYN_REPLACE_CHECK(BattleGenerateHitTriangleAttack);
void BattleGenerateHitTriangleAttack(struct BattleUnit * attacker, struct BattleUnit * defender)
{

    /**
     * Since we're no longer limiting ourselves to just checking a
     * hardcoded attribute we can turn this off.
     */
    // if (!(UNIT_CATTRIBUTES(&attacker->unit) & CA_TRIANGLEATTACK_ANY))
    //    return;

    if (gBattleStats.range != 1)
        return;

    if (!(gBattleHitIterator->info & BATTLE_HIT_INFO_BEGIN))
        return;

    if (attacker->unit.statusIndex == UNIT_STATUS_BERSERK)
        return;

    if (gBattleStats.config & BATTLE_CONFIG_ARENA)
        return;

    if (!BattleCheckTriangleAttack(attacker, defender))
        return;

    gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_TATTACK;

    gBattleStats.critRate = 100;
    gBattleStats.hitRate = 100;
}

LYN_REPLACE_CHECK(BattleGenerateHitAttributes);
void BattleGenerateHitAttributes(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    gBattleStats.damage = 0;

    /* Fasten simulation */
    if (!BattleRoll2RN(gBattleStats.hitRate, FALSE))
    {
#if (defined(SID_DivinePulse) && (COMMON_SKILL_VALID(SID_DivinePulse)))
        if (BattleRoll2RN(gBattleStats.hitRate, FALSE) &&
            CheckBattleSkillActivate(
                attacker, defender, SID_DivinePulse, SKILL_EFF0(SID_DivinePulse) + attacker->unit.lck))
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

    gBattleStats.damage = BattleHit_CalcDamage(attacker, defender);

    if (gBattleStats.config & BATTLE_CONFIG_REAL)
    {
        if (gDmg.real_damage > 0)
            TriggerKtutorial(KTUTORIAL_REAL_DAMAGE);
    }

#if defined(SID_Debilitator) && (COMMON_SKILL_VALID(SID_Debilitator))
    if (BattleSkillTester(attacker, SID_Debilitator) && gBattleStats.damage == 0 && !gBattleActorGlobalFlag.skill_activated_debilitator)
        gBattleActorGlobalFlag.skill_activated_debilitator = true;
#endif

    BattleCheckPetrify(attacker, defender);

    if (gBattleStats.damage != 0)
        attacker->nonZeroDamage = TRUE;
}

LYN_REPLACE_CHECK(BattleGenerateHitEffects);
void BattleGenerateHitEffects(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    FORCE_DECLARE bool gainWEXP = true;

#if (defined(SID_ShadowgiftPlus) && (COMMON_SKILL_VALID(SID_ShadowgiftPlus)))
    if (BattleSkillTester(attacker, SID_ShadowgiftPlus))
        if (GetItemType(GetUnitEquippedWeapon(GetUnit(attacker->unit.index))) == ITYPE_DARK)
            if (GetUnit(attacker->unit.index)->ranks[ITYPE_DARK] == 0)
                gainWEXP = false;
#endif

#if (defined(SID_Shadowgift) && (COMMON_SKILL_VALID(SID_Shadowgift)))
    if (BattleSkillTester(attacker, SID_Shadowgift))
        if (GetItemType(GetUnitEquippedWeapon(GetUnit(attacker->unit.index))) == ITYPE_DARK)
            if (GetUnit(attacker->unit.index)->ranks[ITYPE_DARK] == 0)
                gainWEXP = false;
#endif

#if (defined(SID_LuminaPlus) && (COMMON_SKILL_VALID(SID_LuminaPlus)))
    if (BattleSkillTester(attacker, SID_LuminaPlus))
        if (GetItemType(GetUnitEquippedWeapon(GetUnit(attacker->unit.index))) == ITYPE_LIGHT)
            if (GetUnit(attacker->unit.index)->ranks[ITYPE_LIGHT] == 0)
                gainWEXP = false;
#endif

#if (defined(SID_Lumina) && (COMMON_SKILL_VALID(SID_Lumina)))
    if (BattleSkillTester(attacker, SID_Lumina))
        if (GetItemType(GetUnitEquippedWeapon(GetUnit(attacker->unit.index))) == ITYPE_LIGHT)
            if (GetUnit(attacker->unit.index)->ranks[ITYPE_LIGHT] == 0)
                gainWEXP = false;
#endif

#if (defined(SID_StormgiftPlus) && (COMMON_SKILL_VALID(SID_StormgiftPlus)))
    if (BattleSkillTester(attacker, SID_StormgiftPlus))
        if (GetItemType(GetUnitEquippedWeapon(GetUnit(attacker->unit.index))) == ITYPE_ANIMA)
            if (GetUnit(attacker->unit.index)->ranks[ITYPE_ANIMA] == 0)
                gainWEXP = false;
#endif

#if (defined(SID_Stormgift) && (COMMON_SKILL_VALID(SID_Stormgift)))
    if (BattleSkillTester(attacker, SID_Stormgift))
        if (GetItemType(GetUnitEquippedWeapon(GetUnit(attacker->unit.index))) == ITYPE_ANIMA)
            if (GetUnit(attacker->unit.index)->ranks[ITYPE_ANIMA] == 0)
                gainWEXP = false;
#endif

#if (defined(SID_BladegiftPlus) && (COMMON_SKILL_VALID(SID_BladegiftPlus)))
    if (BattleSkillTester(attacker, SID_BladegiftPlus))
        if (GetItemType(GetUnitEquippedWeapon(GetUnit(attacker->unit.index))) == ITYPE_SWORD)
            if (GetUnit(attacker->unit.index)->ranks[ITYPE_SWORD] == 0)
                gainWEXP = false;
#endif

#if (defined(SID_Bladegift) && (COMMON_SKILL_VALID(SID_Bladegisft)))
    if (BattleSkillTester(attacker, SID_Bladegift))
        if (GetItemType(GetUnitEquippedWeapon(GetUnit(attacker->unit.index))) == ITYPE_SWORD)
            if (GetUnit(attacker->unit.index)->ranks[ITYPE_SWORD] == 0)
                gainWEXP = false;
#endif

#if (defined(SID_PiercegiftPlus) && (COMMON_SKILL_VALID(SID_PiercegiftPlus)))
    if (BattleSkillTester(attacker, SID_PiercegiftPlus))
        if (GetItemType(GetUnitEquippedWeapon(GetUnit(attacker->unit.index))) == ITYPE_LANCE)
            if (GetUnit(attacker->unit.index)->ranks[ITYPE_LANCE] == 0)
                gainWEXP = false;
#endif

#if (defined(SID_Piercegift) && (COMMON_SKILL_VALID(SID_Piercegisft)))
    if (BattleSkillTester(attacker, SID_Piercegift))
        if (GetItemType(GetUnitEquippedWeapon(GetUnit(attacker->unit.index))) == ITYPE_LANCE)
            if (GetUnit(attacker->unit.index)->ranks[ITYPE_LANCE] == 0)
                gainWEXP = false;
#endif

#if (defined(SID_HackgiftPlus) && (COMMON_SKILL_VALID(SID_HackgiftPlus)))
    if (BattleSkillTester(attacker, SID_HackgiftPlus))
        if (GetItemType(GetUnitEquippedWeapon(GetUnit(attacker->unit.index))) == ITYPE_AXE)
            if (GetUnit(attacker->unit.index)->ranks[ITYPE_AXE] == 0)
                gainWEXP = false;
#endif

#if (defined(SID_Hackgift) && (COMMON_SKILL_VALID(SID_Hackgisft)))
    if (BattleSkillTester(attacker, SID_Hackgift))
        if (GetItemType(GetUnitEquippedWeapon(GetUnit(attacker->unit.index))) == ITYPE_AXE)
            if (GetUnit(attacker->unit.index)->ranks[ITYPE_AXE] == 0)
                gainWEXP = false;
#endif

#if (defined(SID_ArcgiftPlus) && (COMMON_SKILL_VALID(SID_ArcgiftPlus)))
    if (BattleSkillTester(attacker, SID_ArcgiftPlus))
        if (GetItemType(GetUnitEquippedWeapon(GetUnit(attacker->unit.index))) == ITYPE_BOW)
            if (GetUnit(attacker->unit.index)->ranks[ITYPE_BOW] == 0)
                gainWEXP = false;
#endif

#if (defined(SID_Arcgift) && (COMMON_SKILL_VALID(SID_Arcgift)))
    if (BattleSkillTester(attacker, SID_Arcgift))
        if (GetItemType(GetUnitEquippedWeapon(GetUnit(attacker->unit.index))) == ITYPE_BOW)
            if (GetUnit(attacker->unit.index)->ranks[ITYPE_BOW] == 0)
                gainWEXP = false;
#endif

    if (gainWEXP)
    {
#if (defined(SID_Discipline) && (COMMON_SKILL_VALID(SID_Discipline)))
        if (BattleSkillTester(attacker, SID_Discipline))
            attacker->wexpMultiplier += 2;
        else
            attacker->wexpMultiplier++;
#else
        attacker->wexpMultiplier++;
#endif
    }

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS))
    {
        if (CheckBattleHpHalve(attacker, defender))
        {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPHALVE;
            gBattleStats.damage = defender->unit.curHP / 2;
        }

        if (CheckDevilAttack(attacker, defender))
        {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_DEVIL;
            if (gBattleStats.damage > attacker->unit.curHP)
                gBattleStats.damage = attacker->unit.curHP;

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

#if defined(SID_DownWithArch) && (COMMON_SKILL_VALID(SID_DownWithArch))
            if (BattleSkillTester(attacker, SID_DownWithArch))
            {
                char name[] = "Arch";
                if (strcmp(GetStringFromIndex(GetUnit(defender->unit.index)->pCharacterData->nameTextId), name) == 0)
                {
                    RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DownWithArch);
                    gBattleStats.damage = defender->unit.curHP;
                    gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
                }
            }
#endif
            defender->unit.curHP -= gBattleStats.damage;

            if (defender->unit.curHP < 0)
                defender->unit.curHP = 0;
        }

#ifdef CHAX
        if (CheckBattleHpDrain(attacker, defender))
#else
        if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPDRAIN)
#endif
        {
#if (defined(SID_LiquidOoze) && (COMMON_SKILL_VALID(SID_LiquidOoze)))
            if (BattleSkillTester(defender, SID_LiquidOoze))
            {
                if ((attacker->unit.curHP - gBattleStats.damage) <= 0)
                    attacker->unit.curHP = 1;
                else
                {
                    attacker->unit.curHP -= gBattleStats.damage;
                    defender->unit.curHP += gBattleStats.damage;
                }

                /**
                 * I tried every trick in the book, but there's a visual bug with this skill
                 * where the skill holder's HP will keep ticking up to the byte limit when it's triggered.
                 * So as a band aid fix, I force battle animations off in this instance.
                 * This does come with the caveat of forcing off everyone's animations
                 * and reversing what I did is a pain without storing the previous configuration, sorry :(
                 */
                SetGameOption(0, 2);
                gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_DEVIL;
            }
            else
            {
                if (attacker->unit.maxHP < (attacker->unit.curHP + gBattleStats.damage))
                    attacker->unit.curHP = attacker->unit.maxHP;
                else
                    attacker->unit.curHP += gBattleStats.damage;

                gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPSTEAL;
            }
#else
            if (attacker->unit.maxHP < (attacker->unit.curHP + gBattleStats.damage))
                attacker->unit.curHP = attacker->unit.maxHP;
            else
                attacker->unit.curHP += gBattleStats.damage;

            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPSTEAL;
#endif
        }

        BattleHit_InjectNegativeStatus(attacker, defender);
    }

    gBattleHitIterator->hpChange = gBattleStats.damage;

    BattleHit_ConsumeWeapon(attacker, defender);
}

LYN_REPLACE_CHECK(BattleGenerateHit);
bool BattleGenerateHit(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    if (attacker == &gBattleTarget)
        gBattleHitIterator->info |= BATTLE_HIT_INFO_RETALIATION;

    BattleUpdateBattleStats(attacker, defender);

    BattleGenerateHitTriangleAttack(attacker, defender);
    BattleGenerateHitAttributes(attacker, defender);
    BattleGenerateHitEffects(attacker, defender);

/* This is a test, can be removed when the feature fully wors */
#ifdef CONFIG_BEXP
    sBEXP[0] += 50;
    NoCashGBAPrintf("BEXP level right now is: %d", sBEXP[0]);
#endif

    // gPlaySt.bexp += 1;

    // NoCashGBAPrintf("BEXP is %d", gPlaySt.bexp);

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

/* 
** Set the ballista byte in the unit's unit struct to the chapter number they died on  to check when using arise 
** Since we're only concerned with player units using his skill, we can use an AI byte to check if they've already been revived
*/
#if (defined(SID_Arise) && (COMMON_SKILL_VALID(SID_Arise)))
    if (GetUnit(attacker->unit.index)->ai1 != 0xFF)
        if (attacker->unit.curHP == 0)
        {
            GetUnit(attacker->unit.index)->ai1 = 0xFF;
            GetUnit(attacker->unit.index)->ballistaIndex = gPlaySt.chapterIndex;
        }

    if (GetUnit(defender->unit.index)->ai1 != 0xFF)
        if (defender->unit.curHP == 0)
        {
            GetUnit(attacker->unit.index)->ai1 = 0xFF;
            GetUnit(defender->unit.index)->ballistaIndex = gPlaySt.chapterIndex;
        }
#endif

        gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;

#if CHAX
        if (defender->unit.curHP == 0)
        {
            if (CheckBattleInori(attacker, defender))
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

#if (defined(SID_LeadByExample) && (COMMON_SKILL_VALID(SID_LeadByExample)))
            if (CheckBattleSkillActivate(&gBattleActor, &gBattleTarget, SID_LeadByExample, 100))
                gBattleActorGlobalFlag.skill_activated_lead_by_example = true;
#endif

#if (defined(SID_Pickup) && (COMMON_SKILL_VALID(SID_Pickup)))
            if (CheckBattleSkillActivate(&gBattleActor, &gBattleTarget, SID_Pickup, gBattleActor.unit.lck))
            {
                struct Unit * unit_tar = &gBattleTarget.unit;
                unit_tar->state |= US_DROP_ITEM;
            }
#endif

#if (defined(SID_Resurrection) && (COMMON_SKILL_VALID(SID_Resurrection)))
            if (BattleSkillTester(&gBattleTarget, SID_Resurrection) && GetUnit(gBattleTarget.unit.index)->_u3A != UES_BIT_RESURRECTION_SKILL_USED)
            {
                gBattleTargetGlobalFlag.skill_activated_resurrection = true;
                gBattleTarget.unit.curHP = 1;
                gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;
                gBattleHitIterator++;
                return true;
            }
#endif
            gBattleHitIterator->info |= BATTLE_HIT_INFO_KILLS_TARGET;
        }

        gBattleHitIterator++;
        return true;
    }
    else if (
        defender->statusOut == UNIT_STATUS_PETRIFY || defender->statusOut == UNIT_STATUS_13 ||
        defender->statusOut == UNIT_STATUS_SLEEP || (GetUnitStatusIndex(GetUnit(defender->unit.index)) == NEW_UNIT_STATUS_BREAK && defender->canCounter == true))
    {
        gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;
        gBattleHitIterator++;
        return true;
    }

    gBattleHitIterator++;
    return false;
}

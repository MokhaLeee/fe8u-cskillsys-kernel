#include "common-chax.h"
#include "skill-system.h"
#include "strmag.h"
#include "debuff.h"
#include "bwl.h"
#include "kernel-lib.h"
#include "class-types.h"
#include "combat-art.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"

typedef void (* PreBattleCalcFunc) (struct BattleUnit * buA, struct BattleUnit * buB);
extern PreBattleCalcFunc const * const gpPreBattleCalcFuncs;
void PreBattleCalcWeaponTriangle(struct BattleUnit * attacker, struct BattleUnit * defender);

/* LynJump */
void ComputeBattleUnitAttack(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int status;
    status = GetItemMight(attacker->weapon);

    if (IsUnitEffectiveAgainst(&attacker->unit, &defender->unit) || IsItemEffectiveAgainst(attacker->weapon, &defender->unit))
    {
        status = status * 2;

#if (defined(SID_Resourceful) && (COMMON_SKILL_VALID(SID_Resourceful)))
        if (SkillTester(&attacker->unit, SID_Resourceful))
            status = status * 2;
#endif
    }

    if (IsMagicAttack(attacker))
        status = status + UNIT_MAG(&attacker->unit);
    else
        status = status + attacker->unit.pow;

    attacker->battleAttack = status;
}

/* LynJump */
void ComputeBattleUnitDefense(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int status, def, res;

    def = attacker->unit.def + attacker->terrainDefense;
    res = attacker->unit.res + attacker->terrainResistance;

    if (IsMagicAttack(defender))
        status = res;
    else
        status = def;

#if (defined(SID_MysticBoost) && COMMON_SKILL_VALID(SID_MysticBoost))
    if (!SkillTester(&attacker->unit, SID_MysticBoost))
#else
    if (1)
#endif
    {
#if (defined(SID_SorceryBlade) && (COMMON_SKILL_VALID(SID_SorceryBlade)))
        if (SkillTester(&defender->unit, SID_SorceryBlade))
            status = def < res ? def : res;
#endif
    }
    attacker->battleDefense = status;
}

/* LynJump */
void ComputeBattleUnitCritRate(struct BattleUnit * bu)
{
    int status;
    status = bu->unit.skl / 2;

#if defined(SID_SuperLuck) && (COMMON_SKILL_VALID(SID_SuperLuck))
    if (SkillTester(&bu->unit, SID_SuperLuck))
        status = bu->unit.lck;
#endif

    status += GetItemCrit(bu->weapon);

    if (UNIT_CATTRIBUTES(&bu->unit) & CA_CRITBONUS)
        bu->battleCritRate += 15;
    
    bu->battleCritRate = status;
}

void PreBattleCalcInit(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    ComputeBattleUnitDefense(attacker, defender);
    ComputeBattleUnitAttack(attacker, defender);
    ComputeBattleUnitSpeed(attacker);
    ComputeBattleUnitHitRate(attacker);
    ComputeBattleUnitAvoidRate(attacker);
    ComputeBattleUnitCritRate(attacker);
    ComputeBattleUnitDodgeRate(attacker);
    ComputeBattleUnitSupportBonuses(attacker, defender);
    ComputeBattleUnitWeaponRankBonuses(attacker);
    ComputeBattleUnitStatusBonuses(attacker);

    /* Calc silencer rate at pre-battle-calc rather than battle-calc-real */
    attacker->battleSilencerRate = 0;
}

void PreBattleCalcEnd(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    if (attacker->battleAttack < 0)
        attacker->battleAttack = 0;

    if (attacker->battleDefense < 0)
        attacker->battleDefense = 0;

    if (attacker->battleSpeed < 0)
        attacker->battleSpeed = 0;

    if (attacker->battleHitRate < 0)
        attacker->battleHitRate = 0;

    if (attacker->battleAvoidRate < 0)
        attacker->battleAvoidRate = 0;

    if (attacker->battleCritRate < 0)
        attacker->battleCritRate = 0;

    if (attacker->battleDodgeRate < 0)
        attacker->battleDodgeRate = 0;

    /* Some special effects */

#if (defined(SID_RuinedBladePlus) && (COMMON_SKILL_VALID(SID_RuinedBladePlus)))
    /* RuinedBladePlus cannot crit attack */
    if (SkillTester(&attacker->unit, SID_RuinedBladePlus))
    {
        attacker->battleCritRate = 0;
        attacker->battleSilencerRate = 0;
    }
#endif

#if (defined(SID_CriticalPierce) && (COMMON_SKILL_VALID(SID_CriticalPierce)))
    if (SkillTester(&defender->unit, SID_CriticalPierce))
        attacker->battleDodgeRate = 0;
#endif

    /* If defender cannot get silencer */
    if (UNIT_CATTRIBUTES(&defender->unit) & CA_NEGATE_LETHALITY)
        attacker->battleSilencerRate = 0;

    if (attacker->battleSilencerRate < 0)
        attacker->battleSilencerRate = 0;
}

void PreBattleCalcSkills(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    FORCE_DECLARE int tmp, i;
    int _skill_list_cnt;
    struct SkillList * list;

    /**
     * Skip arena judgement
     */
    if (gBattleStats.config & BATTLE_CONFIG_ARENA)
        return;

    list = GetUnitSkillList(&attacker->unit);
    for (_skill_list_cnt = 0; _skill_list_cnt < list->amt; _skill_list_cnt++)
    {
        switch (list->sid[_skill_list_cnt]) {
#if (defined(SID_DefiantCrit) && (COMMON_SKILL_VALID(SID_DefiantCrit)))
        case SID_DefiantCrit:
            if ((attacker->hpInitial * 4) < attacker->unit.maxHP)
                attacker->battleCritRate += 50;

            break;
#endif

#if (defined(SID_DefiantAvoid) && (COMMON_SKILL_VALID(SID_DefiantAvoid)))
        case SID_DefiantAvoid:
            if ((attacker->hpInitial * 4) < attacker->unit.maxHP)
                attacker->battleAvoidRate += 50; 

            break;
#endif

        /* Blow skills */
#if (defined(SID_BlowDarting) && (COMMON_SKILL_VALID(SID_BlowDarting)))
        case SID_BlowDarting:
            if (attacker == &gBattleActor)
                attacker->battleSpeed += 6;

            break;
#endif

#if (defined(SID_BlowDeath) && (COMMON_SKILL_VALID(SID_BlowDeath)))
        case SID_BlowDeath:
            if (attacker == &gBattleActor && !IsMagicAttack(attacker))
                attacker->battleAttack += 6;

            break;
#endif

#if (defined(SID_BlowFiendish) && (COMMON_SKILL_VALID(SID_BlowFiendish)))
        case SID_BlowFiendish:
            if (attacker == &gBattleActor && IsMagicAttack(defender))
                attacker->battleAttack += 6;

            break;
#endif

#if (defined(SID_BlowArmored) && (COMMON_SKILL_VALID(SID_BlowArmored)))
        case SID_BlowArmored:
            if (attacker == &gBattleActor && !IsMagicAttack(defender))
                attacker->battleDefense += 6;

            break;
#endif

#if (defined(SID_BlowWarding) && (COMMON_SKILL_VALID(SID_BlowWarding)))
        case SID_BlowWarding:
            if (attacker == &gBattleActor && IsMagicAttack(defender))
                attacker->battleDefense += 6;

            break;
#endif

#if (defined(SID_BlowDuelist) && (COMMON_SKILL_VALID(SID_BlowDuelist)))
        case SID_BlowDuelist:
            if (attacker == &gBattleActor)
                attacker->battleAvoidRate += 20;

            break;
#endif

#if (defined(SID_BlowUncanny) && (COMMON_SKILL_VALID(SID_BlowUncanny)))
        case SID_BlowUncanny:
            if (attacker == &gBattleActor)
                attacker->battleHitRate += 30;

            break;
#endif

#if (defined(SID_BlowKilling) && (COMMON_SKILL_VALID(SID_BlowKilling)))
        case SID_BlowKilling:
            if (attacker == &gBattleActor)
                attacker->battleCritRate += 20;

            break;
#endif

#if (defined(SID_ArcaneBlade) && (COMMON_SKILL_VALID(SID_ArcaneBlade)))
        case SID_ArcaneBlade:
            if (attacker == &gBattleActor && gBattleStats.range == 1)
            {
                attacker->battleCritRate += 3 + UNIT_MAG(&attacker->unit) / 2;
                attacker->battleHitRate += 3 + UNIT_MAG(&attacker->unit) / 2;
            }
            break;
#endif

#if (defined(SID_Prescience) && (COMMON_SKILL_VALID(SID_Prescience)))
        case SID_Prescience:
            if (attacker == &gBattleActor && gBattleStats.range == 1)
            {
                attacker->battleHitRate += 15;
                attacker->battleAvoidRate += 15;
            }
            break;
#endif

        /* Stance skills */
#if (defined(SID_StanceBracing) && (COMMON_SKILL_VALID(SID_StanceBracing)))
        case SID_StanceBracing:
            if (attacker == &gBattleTarget)
                attacker->battleDefense += 4;

            break;
#endif

#if (defined(SID_StanceDarting) && (COMMON_SKILL_VALID(SID_StanceDarting)))
        case SID_StanceDarting:
            if (attacker == &gBattleTarget)
                attacker->battleSpeed += 6;

            break;
#endif

#if (defined(SID_StanceFierce) && (COMMON_SKILL_VALID(SID_StanceFierce)))
        case SID_StanceFierce:
            if (attacker == &gBattleTarget)
                attacker->battleAttack += 6;

            break;
#endif

#if (defined(SID_StanceKestrel) && (COMMON_SKILL_VALID(SID_StanceKestrel)))
        case SID_StanceKestrel:
            if (attacker == &gBattleTarget)
            {
                attacker->battleAttack += 4;
                attacker->battleSpeed += 4;
            }
            break;
#endif

#if (defined(SID_StanceMirror) && (COMMON_SKILL_VALID(SID_StanceMirror)))
        case SID_StanceMirror:
            if (attacker == &gBattleTarget)
            {
                attacker->battleAttack += 4;
                if (IsMagicAttack(defender))
                    attacker->battleDefense += 4;
            }
            break;
#endif

#if (defined(SID_StanceSwift) && (COMMON_SKILL_VALID(SID_StanceSwift)))
        case SID_StanceSwift:
            if (attacker == &gBattleTarget)
            {
                attacker->battleSpeed += 4;
                if (IsMagicAttack(defender))
                    attacker->battleDefense += 4;
            }
            break;
#endif

#if (defined(SID_StanceReady) && (COMMON_SKILL_VALID(SID_StanceReady)))
        case SID_StanceReady:
            if (attacker == &gBattleTarget)
            {
                attacker->battleSpeed += 4;
                if (!IsMagicAttack(defender))
                    attacker->battleDefense += 4;
            }
            break;
#endif

#if (defined(SID_StanceSturdy) && (COMMON_SKILL_VALID(SID_StanceSturdy)))
        case SID_StanceSturdy:
            if (attacker == &gBattleTarget)
            {
                attacker->battleAttack += 4;
                if (!IsMagicAttack(defender))
                    attacker->battleDefense += 4;
            }
            break;
#endif

#if (defined(SID_StanceSteady) && (COMMON_SKILL_VALID(SID_StanceSteady)))
        case SID_StanceSteady:
            if (attacker == &gBattleTarget && !IsMagicAttack(defender))
                attacker->battleDefense += 6;

            break;
#endif

#if (defined(SID_StanceWarding) && (COMMON_SKILL_VALID(SID_StanceWarding)))
        case SID_StanceWarding:
            if (attacker == &gBattleTarget && IsMagicAttack(defender))
                attacker->battleDefense += 6;

            break;
#endif

#if (defined(SID_StanceSpectrum) && (COMMON_SKILL_VALID(SID_StanceSpectrum)))
        case SID_StanceSpectrum:
            if (attacker == &gBattleTarget)
            {
                attacker->battleAttack += 2;
                attacker->battleSpeed += 2;
                attacker->battleDefense += 2;
            }
            break;
#endif

#if (defined(SID_Patience) && (COMMON_SKILL_VALID(SID_Patience)))
        case SID_Patience:
            if (attacker == &gBattleTarget)
                attacker->battleAvoidRate += 10;

            break;
#endif

#if (defined(SID_Pursuit) && (COMMON_SKILL_VALID(SID_Pursuit)))
        case SID_Pursuit:
            if (attacker == &gBattleTarget)
                attacker->battleSpeed += 2;

            break;
#endif

        /* Misc */
#if (defined(SID_Lethality) && (COMMON_SKILL_VALID(SID_Lethality)))
        case SID_Lethality:
            attacker->battleSilencerRate += attacker->unit.skl;
            break;
#endif

#if (defined(SID_WatchfulEye) && (COMMON_SKILL_VALID(SID_WatchfulEye)))
        case SID_WatchfulEye:
            attacker->battleHitRate += 20;
            break;
#endif

#if (defined(SID_Crit) && (COMMON_SKILL_VALID(SID_Crit)))
        case SID_Crit:
            attacker->battleCritRate += 15;
            break;
#endif

#if (defined(SID_Avoid) && (COMMON_SKILL_VALID(SID_Avoid)))
        case SID_Avoid:
            attacker->battleAvoidRate += 10;
            break;
#endif

        /* wType related */
#if (defined(SID_CritSword) && (COMMON_SKILL_VALID(SID_CritSword)))
        case SID_CritSword:
            if (attacker->weaponType == ITYPE_SWORD)
                attacker->battleCritRate += 10;

            break;
#endif

#if (defined(SID_FaireSword) && (COMMON_SKILL_VALID(SID_FaireSword)))
        case SID_FaireSword:
            if (attacker->weaponType == ITYPE_SWORD)
                attacker->battleAttack += 5;

            break;
#endif

#if (defined(SID_AvoidSword) && (COMMON_SKILL_VALID(SID_AvoidSword)))
        case SID_AvoidSword:
            if (attacker->weaponType == ITYPE_SWORD)
                attacker->battleAvoidRate += 20;

            break;
#endif

#if (defined(SID_CritLance) && (COMMON_SKILL_VALID(SID_CritLance)))
        case SID_CritLance:
            if (attacker->weaponType == ITYPE_LANCE)
                attacker->battleCritRate += 10;

            break;
#endif

#if (defined(SID_FaireLance) && (COMMON_SKILL_VALID(SID_FaireLance)))
        case SID_FaireLance:
            if (attacker->weaponType == ITYPE_LANCE)
                attacker->battleAttack += 5;

            break;
#endif

#if (defined(SID_CritAxe) && (COMMON_SKILL_VALID(SID_CritAxe)))
        case SID_CritAxe:
            if (attacker->weaponType == ITYPE_AXE)
                attacker->battleCritRate += 10;

            break;
#endif

#if (defined(SID_FaireAxe) && (COMMON_SKILL_VALID(SID_FaireAxe)))
        case SID_FaireAxe:
            if (attacker->weaponType == ITYPE_AXE)
                attacker->battleAttack += 5;

            break;
#endif

#if (defined(SID_CritBow) && (COMMON_SKILL_VALID(SID_CritBow)))
        case SID_CritBow:
            if (attacker->weaponType == ITYPE_BOW)
                attacker->battleCritRate += 10;

            break;
#endif

#if (defined(SID_FaireBow) && (COMMON_SKILL_VALID(SID_FaireBow)))
        case SID_FaireBow:
            if (attacker->weaponType == ITYPE_BOW)
                attacker->battleAttack += 5;

            break;
#endif

#if (defined(SID_FaireBMag) && (COMMON_SKILL_VALID(SID_FaireBMag)))
        case SID_FaireBMag:
            switch (attacker->weaponType) {
            case ITYPE_ANIMA:
            case ITYPE_LIGHT:
            case ITYPE_DARK:
                attacker->battleAttack += 5;
                break;
            }
            break;
#endif

#if defined(SID_HolyAura) && (COMMON_SKILL_VALID(SID_HolyAura))
        case SID_HolyAura:
            if (attacker->weaponType == ITYPE_LIGHT)
            {
                attacker->battleAttack += 1;
                attacker->battleCritRate += 5;
                attacker->battleHitRate += 5;
                attacker->battleAvoidRate += 5;
            }
            break;
#endif

#if (defined(SID_BlueFlame) && (COMMON_SKILL_VALID(SID_BlueFlame)))
        case SID_BlueFlame:
            attacker->battleAttack += 2;
            break;
#endif

#if (defined(SID_Frenzy) && (COMMON_SKILL_VALID(SID_Frenzy)))
        case SID_Frenzy:
            if ((attacker->unit.maxHP - attacker->hpInitial) >= 4)
                attacker->battleAttack += (attacker->unit.maxHP - attacker->hpInitial) / 4;

            break;
#endif

#if (defined(SID_KillingMachine) && (COMMON_SKILL_VALID(SID_KillingMachine)))
        case SID_KillingMachine:
            attacker->battleCritRate *= 2;
            break;
#endif

#if (defined(SID_HeavyStrikes) && (COMMON_SKILL_VALID(SID_HeavyStrikes)))
        case SID_HeavyStrikes:
            attacker->battleCritRate += GetItemWeight(attacker->weapon);
            break;
#endif

#if (defined(SID_Technician) && (COMMON_SKILL_VALID(SID_Technician)))
        case SID_Technician:
            if (GetItemRequiredExp(attacker->weapon) < WPN_EXP_D)
                attacker->battleAttack += GetItemMight(attacker->weapon) / 2;
            break;
#endif

#if (defined(SID_BattleVeteran) && (COMMON_SKILL_VALID(SID_BattleVeteran)))
        case SID_BattleVeteran:
            tmp = simple_div(attacker->levelPrevious + GetUnitHiddenLevel(&attacker->unit), 10);
            attacker->battleHitRate += 5 * tmp;
            attacker->battleAttack += tmp;
            break;
#endif

#if (defined(SID_StoneBody) && (COMMON_SKILL_VALID(SID_StoneBody)))
        case SID_StoneBody:
            tmp = attacker->unit.conBonus - defender->unit.conBonus;
            if (tmp > 0)
                attacker->battleAttack += tmp;

            break;
#endif

#if (defined(SID_CriticalForce) && (COMMON_SKILL_VALID(SID_CriticalForce)))
        case SID_CriticalForce:
            attacker->battleCritRate += attacker->unit.skl;
            break;
#endif

#if (defined(SID_Chivalry) && (COMMON_SKILL_VALID(SID_Chivalry)))
        case SID_Chivalry:
            if (defender->hpInitial == defender->unit.maxHP)
            {
                attacker->battleDefense += 2;
                attacker->battleAttack += 2;
            }
            break;
#endif

#if (defined(SID_Pragmatic) && (COMMON_SKILL_VALID(SID_Pragmatic)))
        case SID_Pragmatic:
            if (defender->hpInitial < defender->unit.maxHP)
            {
                attacker->battleDefense += 1;
                attacker->battleAttack += 3;
            }
            break;
#endif

#if (defined(SID_Perfectionist) && (COMMON_SKILL_VALID(SID_Perfectionist)))
        case SID_Perfectionist:
            if (attacker->hpInitial == attacker->unit.maxHP)
            {
                attacker->battleHitRate += 15;
                attacker->battleAvoidRate += 15;
            }
            break;
#endif

#if (defined(SID_WindDisciple) && (COMMON_SKILL_VALID(SID_WindDisciple)))
        case SID_WindDisciple:
            if (attacker->hpInitial <= attacker->unit.maxHP)
            {
                attacker->battleHitRate += 10;
                attacker->battleAvoidRate += 10;
            }
            break;
#endif

#if (defined(SID_FieryBlood) && (COMMON_SKILL_VALID(SID_FieryBlood)))
        case SID_FieryBlood:
            if (attacker->hpInitial <= attacker->unit.maxHP)
                attacker->battleAttack += 4;

            break;
#endif

#if (defined(SID_FireBoost) && (COMMON_SKILL_VALID(SID_FireBoost)))
        case SID_FireBoost:
            if (attacker->hpInitial >= (defender->hpInitial + 3))
                attacker->battleAttack += 6;

            break;
#endif

#if (defined(SID_WindBoost) && (COMMON_SKILL_VALID(SID_WindBoost)))
        case SID_WindBoost:
            if (attacker->hpInitial >= (defender->hpInitial + 3))
                attacker->battleSpeed += 6;

            break;
#endif

#if (defined(SID_EarthBoost) && (COMMON_SKILL_VALID(SID_EarthBoost)))
        case SID_EarthBoost:
            if (attacker->hpInitial >= (defender->hpInitial + 3) && !IsMagicAttack(defender))
                attacker->battleDefense += 6;

            break;
#endif

#if (defined(SID_Wrath) && (COMMON_SKILL_VALID(SID_Wrath)))
        case SID_Wrath:
            if (attacker->hpInitial > (attacker->hpInitial * 2))
                attacker->battleCritRate += 20;

            break;
#endif

#if (defined(SID_WaterBoost) && (COMMON_SKILL_VALID(SID_WaterBoost)))
        case SID_WaterBoost:
            if (attacker->hpInitial >= (defender->hpInitial + 3) && IsMagicAttack(defender))
                attacker->battleDefense += 6;

            break;
#endif

#if (defined(SID_ChaosStyle) && (COMMON_SKILL_VALID(SID_ChaosStyle)))
        case SID_ChaosStyle:
            if (IsMagicAttack(attacker) != IsMagicAttack(defender))
                attacker->battleSpeed += 6;

            break;
#endif

#if (defined(SID_Charge) && (COMMON_SKILL_VALID(SID_Charge)))
        case SID_Charge:
            attacker->battleAttack += gActionData.moveCount / 2;
            break;
#endif

#if (defined(SID_NaturalCover) && (COMMON_SKILL_VALID(SID_NaturalCover))) 
        case SID_NaturalCover:
            if (attacker->terrainDefense > 0 || attacker->terrainAvoid > 0 || attacker->terrainResistance > 0)
                attacker->battleDefense += 3;

            break;
#endif

#if (defined(SID_ElbowRoom) && (COMMON_SKILL_VALID(SID_ElbowRoom))) 
        case SID_ElbowRoom:
            if (attacker->terrainDefense == 0 && attacker->terrainAvoid == 0 && attacker->terrainResistance == 0)
                attacker->battleAttack += 3;

            break;
#endif

#if (defined(SID_Vigilance) && (COMMON_SKILL_VALID(SID_Vigilance)))
        case SID_Vigilance:
            attacker->battleAvoidRate += 20;
            break;
#endif

#if (defined(SID_OutdoorFighter) && (COMMON_SKILL_VALID(SID_OutdoorFighter)))
        case SID_OutdoorFighter:
            switch (gBmMapTerrain[attacker->unit.yPos][attacker->unit.xPos]) {
            case TERRAIN_PLAINS:
            case TERRAIN_ROAD:
            case TERRAIN_VILLAGE_03:
            case TERRAIN_VILLAGE_04:
            case TERRIAN_HOUSE:
            case TERRAIN_ARMORY:
            case TERRAIN_VENDOR:
            case TERRAIN_ARENA_08:
            case TERRAIN_C_ROOM_09:
            case TERRAIN_GATE_0B:
            case TERRAIN_FOREST:
            case TERRAIN_THICKET:
            case TERRAIN_SAND:
            case TERRAIN_DESERT:
            case TERRAIN_RIVER:
            case TERRAIN_MOUNTAIN:
            case TERRAIN_PEAK:
            case TERRAIN_BRIDGE_13:
            case TERRAIN_BRIDGE_14:
            case TERRAIN_SEA:
            case TERRAIN_LAKE:
            case TERRAIN_GATE_23:
            case TERRAIN_CHURCH:
            case TERRAIN_RUINS_25:
            case TERRAIN_CLIFF:
            case TERRAIN_BALLISTA_REGULAR:
            case TERRAIN_BALLISTA_LONG:
            case TERRAIN_BALLISTA_KILLER:
            case TERRAIN_SHIP_FLAT:
            case TERRAIN_SHIP_WRECK:
            case TERRAIN_TILE_2C:
            case TERRAIN_ARENA_30:
            case TERRAIN_VALLEY:
            case TERRAIN_FENCE_32:
            case TERRAIN_SNAG:
            case TERRAIN_BRIDGE_34:
            case TERRAIN_SKY:
            case TERRAIN_DEEPS:
            case TERRAIN_RUINS_37:
            case TERRAIN_INN:
            case TERRAIN_BARREL:
            case TERRAIN_BONE:
            case TERRAIN_DARK:
            case TERRAIN_WATER:
            case TERRAIN_DECK:
            case TERRAIN_BRACE:
            case TERRAIN_MAST:
                attacker->battleHitRate += 10;
                attacker->battleAvoidRate += 10;
                break;

            default:
                break;
            }
            break;
#endif

#if (defined(SID_KnightAspirant) && (COMMON_SKILL_VALID(SID_KnightAspirant)))
        case SID_KnightAspirant:
            if ((attacker->hpInitial * 4) > (attacker->unit.maxHP * 3))
            {
                attacker->battleAttack += 2;
                attacker->battleAvoidRate += 15;
            }
            break;
#endif

#if (defined(SID_Outrider) && (COMMON_SKILL_VALID(SID_Outrider)))
        case SID_Outrider:
            attacker->battleDefense += gActionData.moveCount;
            attacker->battleCritRate += (gActionData.moveCount * 3);
            break;
#endif

#if (defined(SID_SilentPride) && (COMMON_SKILL_VALID(SID_SilentPride)))
        case SID_SilentPride:
            tmp = simple_div(attacker->unit.maxHP * 4, attacker->hpInitial);
            attacker->battleAttack += 2 * tmp;
            attacker->battleDefense += 2 * tmp;
            break;
#endif

#if (defined(SID_Guts) && (COMMON_SKILL_VALID(SID_Guts)))
        case SID_Guts:
            if (GetUnitStatusIndex(&attacker->unit) != UNIT_STATUS_NONE)
                attacker->battleAttack += 5;

            break;
#endif

#if (defined(SID_TowerShield) && (COMMON_SKILL_VALID(SID_TowerShield)))
        case SID_TowerShield:
            if (gBattleStats.range >= 2)
                attacker->battleDefense += 6;

            break;
#endif

#if (defined(SID_ShortShield) && (COMMON_SKILL_VALID(SID_ShortShield)))
        case SID_ShortShield:
            if (gBattleStats.range == 1)
                attacker->battleDefense += 6;

            break;
#endif

#if (defined(SID_StunningSmile) && (COMMON_SKILL_VALID(SID_StunningSmile)))
        case SID_StunningSmile:
            if (!(UNIT_CATTRIBUTES(&defender->unit) & CA_FEMALE))
                attacker->battleDefense += 6;

            break;
#endif

#if defined(SID_Trample) && (COMMON_SKILL_VALID(SID_Trample))
        case SID_Trample:
            if (!(UNIT_CATTRIBUTES(&defender->unit) & CA_MOUNTEDAID))
                attacker->battleAttack += 5;

            break;
#endif

#if defined(SID_Opportunist) && (COMMON_SKILL_VALID(SID_Opportunist))
        case SID_Opportunist:
            if (!defender->canCounter)
                attacker->battleAttack += 4;

            break;
#endif

#if defined(SID_Vanity) && (COMMON_SKILL_VALID(SID_Vanity))
        case SID_Vanity:
            if (gBattleStats.range == 2)
            {
                attacker->battleAttack += 2;
                attacker->battleHitRate += 10;
            }
            break;
#endif

#if (defined(SID_EvenRhythm) && (COMMON_SKILL_VALID(SID_EvenRhythm)))
        case SID_EvenRhythm:
            if ((gPlaySt.chapterTurnNumber % 2) == 0)
            {
                attacker->battleAvoidRate += 10;
                attacker->battleHitRate += 10;
            }
            break;
#endif

#if (defined(SID_OddRhythm) && (COMMON_SKILL_VALID(SID_OddRhythm)))
        case SID_OddRhythm:
            if ((gPlaySt.chapterTurnNumber % 2) == 1)
            {
                attacker->battleAvoidRate += 10;
                attacker->battleHitRate += 10;
            }
            break;
#endif

#if (defined(SID_QuickBurn) && (COMMON_SKILL_VALID(SID_QuickBurn)))
        case SID_QuickBurn:
            tmp = gPlaySt.chapterTurnNumber > 15
                ? 0
                : 15 - gPlaySt.chapterTurnNumber;

            attacker->battleHitRate += tmp;
            attacker->battleAvoidRate += tmp;
            break;
#endif

#if (defined(SID_SlowBurn) && (COMMON_SKILL_VALID(SID_SlowBurn)))
        case SID_SlowBurn:
            tmp = gPlaySt.chapterTurnNumber > 15
                ? 15
                : gPlaySt.chapterTurnNumber;

            attacker->battleHitRate += tmp;
            attacker->battleAvoidRate += tmp;
            break;
#endif

#if (defined(SID_Skyguard) && COMMON_SKILL_VALID(SID_Skyguard))
        case SID_Skyguard:
            if (CheckClassFlier(UNIT_CLASS_ID(&defender->unit)))
            {
                for (i = 0; i < ARRAY_COUNT_RANGE3x3; i++)
                {
                    int _x = attacker->unit.xPos + gVecs_3x3[i].x;
                    int _y = attacker->unit.yPos + gVecs_3x3[i].y;

                    struct Unit * unit_ally = GetUnitAtPosition(_x, _y);
                    if (!UNIT_IS_VALID(unit_ally))
                        continue;

                    if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                        continue;

                    if (AreUnitsAllied(attacker->unit.index, unit_ally->index) && CheckClassFlier(UNIT_CLASS_ID(unit_ally)))
                    {
                        attacker->battleAttack += 4;
                        break;
                    }
                }
            }
            break;
#endif

#if (defined(SID_Horseguard) && COMMON_SKILL_VALID(SID_Horseguard))
        case SID_Horseguard:
            if (CheckClassCavalry(UNIT_CLASS_ID(&defender->unit)))
            {
                for (i = 0; i < ARRAY_COUNT_RANGE3x3; i++)
                {
                    int _x = attacker->unit.xPos + gVecs_3x3[i].x;
                    int _y = attacker->unit.yPos + gVecs_3x3[i].y;

                    struct Unit * unit_ally = GetUnitAtPosition(_x, _y);
                    if (!UNIT_IS_VALID(unit_ally))
                        continue;

                    if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                        continue;

                    if (AreUnitsAllied(attacker->unit.index, unit_ally->index) && CheckClassCavalry(UNIT_CLASS_ID(unit_ally)))
                    {
                        attacker->battleAttack += 4;
                        break;
                    }
                }
            }
            break;
#endif

#if (defined(SID_Armorboost) && COMMON_SKILL_VALID(SID_Armorboost))
        case SID_Armorboost:
            if (CheckClassArmor(UNIT_CLASS_ID(&defender->unit)))
            {
                for (i = 0; i < ARRAY_COUNT_RANGE3x3; i++)
                {
                    int _x = attacker->unit.xPos + gVecs_3x3[i].x;
                    int _y = attacker->unit.yPos + gVecs_3x3[i].y;

                    struct Unit * unit_ally = GetUnitAtPosition(_x, _y);
                    if (!UNIT_IS_VALID(unit_ally))
                        continue;

                    if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                        continue;

                    if (AreUnitsAllied(attacker->unit.index, unit_ally->index) && CheckClassArmor(UNIT_CLASS_ID(unit_ally)))
                    {
                        attacker->battleAttack += 4;

                        if (!IsMagicAttack(defender))
                            attacker->battleDefense += 4;

                        break;
                    }
                }
            }
            break;
#endif

#if (defined(SID_Admiration) && COMMON_SKILL_VALID(SID_Admiration))
        case SID_Admiration:
            tmp = 0;
            for (i = 0; i < 4; i++)
            {
                int _j;

                int _x = attacker->unit.xPos + gVecs_1x1[i].x;
                int _y = attacker->unit.yPos + gVecs_1x1[i].y;
                struct Unit * _unit = GetUnitAtPosition(_x, _y);
                if (!_unit)
                    continue;

                if (!AreUnitsAllied(attacker->unit.index, _unit->index))
                    continue;

                for (_j = 0; _j < 4; _j++)
                {
                    int _x2 = _unit->xPos + gVecs_1x1[i].x;
                    int _y2 = _unit->yPos + gVecs_1x1[i].y;

                    struct Unit * _unit2 = GetUnitAtPosition(_x2, _y2);
                    if (!_unit2)
                        continue;

                    if (!AreUnitsAllied(_unit2->index, _unit->index))
                        continue;

                    if (!!(UNIT_CATTRIBUTES(_unit2) & CA_FEMALE) == !!(UNIT_CATTRIBUTES(_unit) & CA_FEMALE))
                    {
                        tmp = 1;
                        goto L_SID_Admiration_done;
                    }
                }
            }

            L_SID_Admiration_done:
            if (tmp == 1)
                attacker->battleDefense += 2;

            break;
#endif

#if (defined(SID_FairyTaleFolk) && COMMON_SKILL_VALID(SID_FairyTaleFolk))
        case SID_FairyTaleFolk:
            tmp = 0;
            for (i = 0; i < 4; i++)
            {
                int _j;

                int _x = attacker->unit.xPos + gVecs_1x1[i].x;
                int _y = attacker->unit.yPos + gVecs_1x1[i].y;
                struct Unit * _unit = GetUnitAtPosition(_x, _y);
                if (!_unit)
                    continue;

                if (!AreUnitsAllied(attacker->unit.index, _unit->index))
                    continue;

                for (_j = 0; _j < 4; _j++)
                {
                    int _x2 = _unit->xPos + gVecs_1x1[i].x;
                    int _y2 = _unit->yPos + gVecs_1x1[i].y;

                    struct Unit * _unit2 = GetUnitAtPosition(_x2, _y2);
                    if (!_unit2)
                        continue;

                    if (!AreUnitsAllied(_unit2->index, _unit->index))
                        continue;

                    if (!!(UNIT_CATTRIBUTES(_unit2) & CA_FEMALE) != !!(UNIT_CATTRIBUTES(_unit) & CA_FEMALE))
                    {
                        tmp = 1;
                        goto L_FairyTaleFolk_done;
                    }
                }
            }

            L_FairyTaleFolk_done:
            if (tmp == 1)
                attacker->battleAttack += 2;

            break;
#endif

#if (defined(SID_Analytic) && (COMMON_SKILL_VALID(SID_Analytic)))
        case SID_Analytic:
            tmp = defender->battleSpeed - attacker->battleSpeed;
            if (tmp >= 4)
                attacker->battleAttack += 10;
            else if (tmp > 0)
                attacker->battleAttack += 5;

            break;
#endif
        }
    }

#if (defined(SID_StunningSmile) && (COMMON_SKILL_VALID(SID_StunningSmile)))
    /* This is judging on defender */
    if (SkillTester(&defender->unit, SID_StunningSmile) && !(UNIT_CATTRIBUTES(&attacker->unit) & CA_FEMALE))
        attacker->battleAvoidRate -= 20;
#endif
}

void PreBattleCalcAuraEffect(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    u32 i, _x, _y;
    struct Unit * unit;

    int allies_gRange3_In3x3 = 0;
    int allies_gRange2_In3x3 = 0;
    int allies_gRange1_In3x3 = 0;

    int enmies_gRange3_In3x3 = 0;
    int enmies_gRange2_In3x3 = 0;
    int enmies_gRange1_In3x3 = 0;

    bool lord_gRange2_In3x3 = false;

    if (gBattleStats.config & BATTLE_CONFIG_ARENA)
        return;

    for (i = 0; i < ARRAY_COUNT_RANGE3x3; i++)
    {
        _x = attacker->unit.xPos + gVecs_3x3[i].x;
        _y = attacker->unit.yPos + gVecs_3x3[i].y;

        unit = GetUnitAtPosition(_x, _y);
        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;

        if (AreUnitsAllied(attacker->unit.index, unit->index))
        {
            /* Buffs */
            if (gRange1_In3x3[i] == 1)
            {
#if (defined(SID_DivinelyInspiring) && (COMMON_SKILL_VALID(SID_DivinelyInspiring)))
                if (SkillTester(unit, SID_DivinelyInspiring))
                {
                    attacker->battleAttack += 3;
                    attacker->battleDefense += 1;
                }
#endif

#if (defined(SID_LilysPoise) && (COMMON_SKILL_VALID(SID_LilysPoise)))
                if (SkillTester(unit, SID_LilysPoise))
                {
                    attacker->battleAttack += 1;
                    attacker->battleDefense += 3;
                }
#endif

#if (defined(SID_BloodTide) && (COMMON_SKILL_VALID(SID_BloodTide)))
                if (SkillTester(unit, SID_BloodTide))
                {
                    attacker->battleAttack += 5;
                    attacker->battleHitRate += 5;
                }
#endif

#if (defined(SID_WhitePool) && (COMMON_SKILL_VALID(SID_WhitePool)))
                if (SkillTester(unit, SID_WhitePool))
                {
                    attacker->battleAttack += 5;
                    attacker->battleSpeed += 5;
                }
#endif

#if (defined(SID_NightTide) && (COMMON_SKILL_VALID(SID_NightTide)))
                if (SkillTester(unit, SID_NightTide))
                    attacker->battleDefense += 5;
#endif

#if (defined(SID_SpurStr) && (COMMON_SKILL_VALID(SID_SpurStr)))
                if (SkillTester(unit, SID_SpurStr))
                {
                    if (!IsMagicAttack(attacker))
                        attacker->battleAttack  += 4;
                }
#endif

#if (defined(SID_SpurMag) && (COMMON_SKILL_VALID(SID_SpurMag)))
                if (SkillTester(unit, SID_SpurMag))
                {
                    if (IsMagicAttack(attacker))
                        attacker->battleAttack  += 4;
                }
#endif

#if (defined(SID_SpurDef) && (COMMON_SKILL_VALID(SID_SpurDef)))
                if (SkillTester(unit, SID_SpurDef))
                {
                    if (!IsMagicAttack(defender))
                        attacker->battleDefense+= 4;
                }
#endif

#if (defined(SID_SpurRes) && (COMMON_SKILL_VALID(SID_SpurRes)))
                if (SkillTester(unit, SID_SpurRes))
                {
                    if (IsMagicAttack(defender))
                        attacker->battleDefense+= 4;
                }
#endif

#if (defined(SID_SpurSpd) && (COMMON_SKILL_VALID(SID_SpurSpd)))
                if (SkillTester(unit, SID_SpurSpd))
                    attacker->battleSpeed += 4;
#endif

#if (defined(SID_Solidarity) && (COMMON_SKILL_VALID(SID_Solidarity)))
                if (SkillTester(unit, SID_Solidarity))
                {
                    attacker->battleCritRate += 10;
                    attacker->battleDodgeRate += 10;
                }
#endif
            }
            if (gRange2_In3x3[i] == 1)
            {
#if (defined(SID_Charm) && (COMMON_SKILL_VALID(SID_Charm)))
                if (SkillTester(unit, SID_Charm) )
                    attacker->battleAttack  += 3;
#endif

#if (defined(SID_DriveStr) && (COMMON_SKILL_VALID(SID_DriveStr)))
                if (SkillTester(unit, SID_DriveStr))
                {
                    if (!IsMagicAttack(attacker))
                        attacker->battleAttack  += 4;
                }
#endif

#if (defined(SID_DriveMag) && (COMMON_SKILL_VALID(SID_DriveMag)))
                if (SkillTester(unit, SID_DriveMag))
                {
                    if (IsMagicAttack(attacker))
                        attacker->battleAttack  += 4;
                }
#endif

#if (defined(SID_DriveDef) && (COMMON_SKILL_VALID(SID_DriveDef)))
                if (SkillTester(unit, SID_DriveDef))
                {
                    if (!IsMagicAttack(defender))
                        attacker->battleDefense+= 4;
                }
#endif

#if (defined(SID_DriveRes) && (COMMON_SKILL_VALID(SID_DriveRes)))
                if (SkillTester(unit, SID_DriveRes))
                {
                    if (IsMagicAttack(defender))
                        attacker->battleDefense+= 4;
                }
#endif

#if (defined(SID_DriveSpd) && (COMMON_SKILL_VALID(SID_DriveSpd)))
                if (SkillTester(unit, SID_DriveSpd))
                    attacker->battleSpeed += 4;
#endif

#if (defined(SID_Inspiration) && (COMMON_SKILL_VALID(SID_Inspiration)))
                if (SkillTester(unit, SID_Inspiration))
                {
                    attacker->battleAttack += 2;
                    attacker->battleDefense += 2;
                }
#endif

#if (defined(SID_Peacebringer) && (COMMON_SKILL_VALID(SID_Peacebringer)))
                if (SkillTester(unit, SID_Peacebringer) )
                    attacker->battleAttack -= 2;
#endif

#if (defined(SID_Gentilhomme) && (COMMON_SKILL_VALID(SID_Gentilhomme)))
                if (SkillTester(unit, SID_Gentilhomme)  && (UNIT_CATTRIBUTES(&attacker->unit) && CA_FEMALE))
                    attacker->battleDefense += 2;
#endif

#if (defined(SID_Demoiselle) && (COMMON_SKILL_VALID(SID_Demoiselle)))
                if (SkillTester(unit, SID_Demoiselle)  && !(UNIT_CATTRIBUTES(&attacker->unit) && CA_FEMALE))
                    attacker->battleDefense += 2;
#endif
            }

            /* Since we just calc in 3x3, so here is always true */             
#if (defined(SID_Bond) && (COMMON_SKILL_VALID(SID_Bond)))
            if (SkillTester(unit, SID_Bond))
            {
                attacker->battleHitRate += 10;
                attacker->battleAttack  += 2;
            }
#endif

#if (defined(SID_Charisma) && (COMMON_SKILL_VALID(SID_Charisma)))
            if (SkillTester(unit, SID_Charisma))
            {
                attacker->battleHitRate += 10;
                attacker->battleAvoidRate += 10;
            }
#endif

            /* Since we just calc in 3x3, so here is always true */
            allies_gRange3_In3x3++;

            if (gRange2_In3x3[i])
            {
                allies_gRange2_In3x3++;
                if (UNIT_CATTRIBUTES(unit) && CA_LORD)
                    lord_gRange2_In3x3 = true;
            }
            if (gRange1_In3x3[i])
                allies_gRange1_In3x3++;
        }
        else
        {
            if (gRange1_In3x3[i] == 1)
            {
#if (defined(SID_Hex) && (COMMON_SKILL_VALID(SID_Hex)))
            if (SkillTester(unit, SID_Hex))
                attacker->battleAvoidRate -= 10;
#endif
            }
            if (gRange2_In3x3[i] == 1)
            {
#if (defined(SID_Intimidate) && (COMMON_SKILL_VALID(SID_Intimidate)))
                if (SkillTester(unit, SID_Intimidate) )
                    attacker->battleAvoidRate -= 10;
#endif

#if (defined(SID_VoiceOfPeace) && (COMMON_SKILL_VALID(SID_VoiceOfPeace)))
                if (SkillTester(unit, SID_VoiceOfPeace) )
                    attacker->battleAttack -= 2;
#endif

#if (defined(SID_Peacebringer) && (COMMON_SKILL_VALID(SID_Peacebringer)))
                if (SkillTester(unit, SID_Peacebringer) )
                    attacker->battleAttack -= 2;
#endif
            }

#if (defined(SID_Anathema) && (COMMON_SKILL_VALID(SID_Anathema)))
            if (SkillTester(unit, SID_Anathema))
            {
                attacker->battleAvoidRate -= 10;
                attacker->battleDodgeRate -= 10;
            }
#endif

#if (defined(SID_Daunt) && (COMMON_SKILL_VALID(SID_Daunt)))
            if (SkillTester(unit, SID_Daunt))
            {
                attacker->battleHitRate -= 5;
                attacker->battleCritRate -= 5;
            }
#endif

            /* Since we just calc in 3x3, so here is always true */
            enmies_gRange3_In3x3++;

            if (gRange2_In3x3[i])
                enmies_gRange2_In3x3++;

            if (gRange1_In3x3[i])
                enmies_gRange1_In3x3++;
        }
    }

    if (allies_gRange3_In3x3 != 0)
    {
        /* Todo */
    }
    else
    {
#if (defined(SID_Tantivy) && (COMMON_SKILL_VALID(SID_Tantivy)))
        if (SkillTester(&attacker->unit, SID_Tantivy))
        {
            attacker->battleHitRate += 10;
            attacker->battleAvoidRate += 10;
        }
#endif

#if (defined(SID_Focus) && (COMMON_SKILL_VALID(SID_Focus)))
        if (SkillTester(&attacker->unit, SID_Focus))
            attacker->battleCritRate += 10;
#endif
    }

    if (enmies_gRange3_In3x3 != 0)
    {
        /* Todo */
    }
    else
    {
        /* Todo */
    }

    if (enmies_gRange2_In3x3 >= 2)
    {
#if (defined(SID_Infiltrator) && (COMMON_SKILL_VALID(SID_Infiltrator)))
        if (SkillTester(&attacker->unit, SID_Infiltrator))
        {
            attacker->battleAttack += 3;
            attacker->battleHitRate += 15;
        }
#endif
    }

    if (allies_gRange1_In3x3 > 0)
    {
#if (defined(SID_BlueFlame) && (COMMON_SKILL_VALID(SID_BlueFlame)))
        if (SkillTester(&attacker->unit, SID_BlueFlame))
            attacker->battleAttack += 2;
#endif
    }

    /* AND skills */
    if (allies_gRange3_In3x3 == 0)
    {
#if (defined(SID_BattleRange_Todo1) && (COMMON_SKILL_VALID(SID_BattleRange_Todo1)))
        if (SkillTester(&attacker->unit, SID_BattleRange_Todo1))
            attacker->battleAttack += 10;
#endif
    }
    else if (allies_gRange2_In3x3 == 0)
    {
#if (defined(SID_BattleRange_Todo2) && (COMMON_SKILL_VALID(SID_BattleRange_Todo2)))
        if (SkillTester(&attacker->unit, SID_BattleRange_Todo2))
            attacker->battleAttack += 7;
#endif
    }
    else if (allies_gRange1_In3x3 == 0)
    {
#if (defined(SID_BattleRange_Todo3) && (COMMON_SKILL_VALID(SID_BattleRange_Todo3)))
        if (SkillTester(&attacker->unit, SID_BattleRange_Todo3))
            attacker->battleAttack += 5;
#endif
    }
    else
    {}

    if (lord_gRange2_In3x3)
    {
#if (defined(SID_Loyalty) && (COMMON_SKILL_VALID(SID_Loyalty)))
        if (SkillTester(&attacker->unit, SID_Loyalty))
        {   attacker->battleDefense += 3;
            attacker->battleHitRate += 15;
        }
#endif   
    }
    
#ifdef CONFIG_BATTLE_SURROUND
    if (attacker == &gBattleTarget && (gBattleStats.config & BATTLE_CONFIG_REAL))
    {
        /* Flyer in outdoor environments are not affected by this effect (todo) */
        if (!(UNIT_CATTRIBUTES(&attacker->unit) & CA_FLYER) || (0))
        {
            int surround_enemies = enmies_gRange1_In3x3 - 1;

            /**
             * When a unit is attacked and adjacent to the enemy,
             * each side with enemy may cause unit avoid -10%
             */
            if (surround_enemies > 0)
            {
                TriggerKtutorial(KTUTORIAL_BATTLE_SURROUNDER);
                attacker->battleAvoidRate -= 10 * surround_enemies;
            }

            /**
             * If unit have been completely surrounded,
             * unit may cause def-5 additionally.
             */
            if (surround_enemies == 3)
                attacker->battleDefense -= 5;
        }
    }
#endif /* CONFIG_BATTLE_SURROUND */
}

void PreBattleCalcSilencerRate(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    if (UNIT_CATTRIBUTES(&defender->unit) & CA_BOSS)
        attacker->battleSilencerRate -= 25;
}

void PreBattleCalcPad(struct BattleUnit * attacker, struct BattleUnit * defender) {}

/* LynJump */
void ComputeBattleUnitStats(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    const PreBattleCalcFunc * it;
    for (it = gpPreBattleCalcFuncs; *it; it++)
        (*it)(attacker, defender);
}

#include "common-chax.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "battle-system.h"
#include "unit-expa.h"

extern void ForEachAdjacentUnit(int x, int y, void (*)(struct Unit *));

extern void GenerateFireTileTrapTargets(int x, int y, int damage);
extern void GenerateArrowTrapTargets(int x, int y, int damage);
extern void GenerateGasTrapTargets(int x, int y, int damage, int facing);

//! FE8U: 0x080B1F64
LYN_REPLACE_CHECK(SetGameOption);
void SetGameOption(u8 index, u8 newValue)
{

    switch (index)
    {
        case GAME_OPTION_ANIMATION:
            switch (newValue)
            {
                case 0:
                    gPlaySt.config.animationType = 0;
                    return;

                case 1:
                    gPlaySt.config.animationType = 3;
                    return;

                case 2:
                    gPlaySt.config.animationType = 1;
                    return;

                case 3:
                    gPlaySt.config.animationType = 2;
                    return;
            }

            // fallthrough

        case GAME_OPTION_TERRAIN:
            gPlaySt.config.disableTerrainDisplay = newValue;

            break;

        case GAME_OPTION_UNIT:
            gPlaySt.config.unitDisplayType = newValue;

            break;

        case GAME_OPTION_AUTOCURSOR:
            gPlaySt.config.autoCursor = newValue;

            break;

        case GAME_OPTION_TEXT_SPEED:
            gPlaySt.config.textSpeed = newValue;

            break;

        case GAME_OPTION_GAME_SPEED:
            gPlaySt.config.gameSpeed = newValue;

            break;

        case GAME_OPTION_MUSIC:
            gPlaySt.config.disableBgm = newValue;

            break;

        case GAME_OPTION_SOUND_EFFECTS:
            gPlaySt.config.disableSoundEffects = newValue;

            break;

        case GAME_OPTION_WINDOW_COLOR:
            gPlaySt.config.windowColor = newValue;

            break;

        case GAME_OPTION_COMBAT:
            gPlaySt.config.battleForecastType = newValue;

            break;

        case GAME_OPTION_SUBTITLE_HELP:
            gPlaySt.config.noSubtitleHelp = newValue;

            break;

        case GAME_OPTION_AUTOEND_TURNS:
            gPlaySt.config.disableAutoEndTurns = newValue;

            break;

        case GAME_OPTION_UNIT_COLOR:
            gPlaySt.config.unitColor = newValue;

            break;

        case GAME_OPTION_OBJECTIVE:
            gPlaySt.config.disableGoalDisplay = newValue;

            break;

        case GAME_OPTION_CONTROLLER:
            gPlaySt.config.controller = newValue;

            break;

        case GAME_OPTION_RANK_DISPLAY:
            gPlaySt.config.rankDisplay = newValue;

            break;
    }

    return;
}

LYN_REPLACE_CHECK(TryAddUnitToTradeTargetList);
void TryAddUnitToTradeTargetList(struct Unit * unit)
{
    FORCE_DECLARE bool capture_active = false;

    /**
     *  With capture, a unit should be able to trade with rescued enemies
     */
#if (defined(SID_Capture) && (COMMON_SKILL_VALID(SID_Capture)))
    if (!SkillTester(gSubjectUnit, SID_Capture))
        capture_active = true;
#endif

    if (!IsSameAllegiance(gSubjectUnit->index, unit->index) && capture_active)
        return;

    if (gSubjectUnit->pClassData->number == CLASS_PHANTOM || unit->pClassData->number == CLASS_PHANTOM)
        return;

    if (unit->statusIndex != UNIT_STATUS_BERSERK)
    {

        if (gSubjectUnit->items[0] != 0 || unit->items[0] != 0)
        {

            if (!(UNIT_CATTRIBUTES(unit) & CA_SUPPLY))
            {
                AddTarget(unit->xPos, unit->yPos, unit->index, 0);
            }
        }
    }

    if (unit->state & US_RESCUING)
    {
        struct Unit * rescue = GetUnit(unit->rescue);

        if (UNIT_FACTION(rescue) != FACTION_BLUE)
        {
            return;
        }

        if (gSubjectUnit->items[0] == 0 && rescue->items[0] == 0)
        {
            return;
        }

        AddTarget(unit->xPos, unit->yPos, rescue->index, 0);
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

LYN_REPLACE_CHECK(MakeTradeTargetList);
void MakeTradeTargetList(struct Unit * unit)
{
    int x = unit->xPos;
    int y = unit->yPos;

    gSubjectUnit = unit;

    BmMapFill(gBmMapRange, 0);
    ForEachAdjacentUnit(x, y, TryAddUnitToTradeTargetList);

    if (gSubjectUnit->state & US_RESCUING)
    {
        int count = GetSelectTargetCount();
        TryAddUnitToTradeTargetList(GetUnit(gSubjectUnit->rescue));

        if (count != GetSelectTargetCount())
        {
            GetTarget(count)->x = gSubjectUnit->xPos;
            GetTarget(count)->y = gSubjectUnit->yPos;
        }
    }

    return;
}

//! FE8U = 0x08032728
LYN_REPLACE_CHECK(KillUnitOnCombatDeath);
void KillUnitOnCombatDeath(struct Unit * unitA, struct Unit * unitB)
{
    if (GetUnitCurrentHp(unitA) != 0)
    {
        return;
    }

#if (defined(SID_Capture) && (COMMON_SKILL_VALID(SID_Capture)))
    // CheckBitUES(unitB, UES_BIT_CAPTURE_SKILL_USED)
    if (SkillTester(unitB, SID_Capture) && unitB->_u3A == 8)
    {
        UnitRescue(unitB, unitA);
        HideUnitSprite(unitA);
        // ClearBitUES(unitB, UES_BIT_CAPTURE_SKILL_USED);
        unitB->_u3A = 0;
        return;
    }
#endif

// Can still be viewed in the stat screen, but eh it's fine.
#if defined(SID_Casual) && (COMMON_SKILL_VALID(SID_Casual))
    if (SkillTester(unitA, SID_Casual))
    {
        unitA->state |= US_HIDDEN;
        return;
    }
#endif

#if defined(SID_DestinyBond) && (COMMON_SKILL_VALID(SID_DestinyBond))
    if (SkillTester(unitA, SID_DestinyBond))
        UnitKill(unitB);
#endif

    PidStatsRecordDefeatInfo(unitA->pCharacterData->number, unitB->pCharacterData->number, DEFEAT_CAUSE_COMBAT);

    UnitKill(unitA);

    return;
}

LYN_REPLACE_CHECK(TryAddUnitToRescueTargetList);
void TryAddUnitToRescueTargetList(struct Unit * unit)
{

    if (!AreUnitsAllied(gSubjectUnit->index, unit->index))
    {
        return;
    }

    if (gSubjectUnit->pClassData->number == CLASS_PHANTOM || unit->pClassData->number == CLASS_PHANTOM)
    {
        return;
    }

    if (unit->statusIndex == UNIT_STATUS_BERSERK)
    {
        return;
    }

    if (unit->state & (US_RESCUING | US_RESCUED))
    {
        return;
    }

    if (!CanUnitRescue(gSubjectUnit, unit))
    {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

LYN_REPLACE_CHECK(UnitDrop);
void UnitDrop(struct Unit * actor, int xTarget, int yTarget)
{
    struct Unit * target = GetUnit(actor->rescue);

    actor->state = actor->state & ~(US_RESCUING | US_RESCUED);
    target->state = target->state & ~(US_RESCUING | US_RESCUED | US_HIDDEN);

    if (UNIT_FACTION(target) == gPlaySt.faction)
        target->state |= US_UNSELECTABLE; // TODO: US_GRAYED

    actor->rescue = 0;
    target->rescue = 0;

    target->xPos = xTarget;
    target->yPos = yTarget;

    /**
     * If we've captured an enemy, dropping them will kill them immediately
     */
    if (target->curHP == 0)
    {
        PidStatsRecordDefeatInfo(target->pCharacterData->number, actor->pCharacterData->number, DEFEAT_CAUSE_COMBAT);
        UnitKill(target);
    }
}

// use vanilla version so we don't lag by using hooked versions that accounts for pass etc
s8 Vanilla_CanUnitCrossTerrain(struct Unit * unit, int terrain)
{
    const s8 * lookup = (s8 *)GetUnitMovementCost(unit);
    return (lookup[terrain] > 0) ? TRUE : FALSE;
}

bool Generic_CanUnitBeOnPos(struct Unit * unit, s8 x, s8 y, int x2, int y2)
{
    if (x < 0 || y < 0)
        return 0; // position out of bounds
    if (x >= gBmMapSize.x || y >= gBmMapSize.y)
        return 0; // position out of bounds
    if (gBmMapUnit[y][x])
        return 0;
    if (gBmMapHidden[y][x] & 1)
        return 0; // a hidden unit is occupying this position
    if ((x2 == x) && (y2 == y))
        return 0;                                                  // exception / a battle unit is on this tile
    return Vanilla_CanUnitCrossTerrain(unit, gBmMapTerrain[y][x]); // CanUnitCrossTerrain(unit, gMapTerrain[y][x]);
}

//! FE8U = 0x0801538C
LYN_REPLACE_CHECK(SwitchPhases);
void SwitchPhases(void)
{
    switch (gPlaySt.faction)
    {
        case FACTION_BLUE:

            /**
             * There's probably a more efficient way to do this,
             * but this is all I've found to work right now.
             * I change back the unit faction for a 'turncoat' unit
             * if they haven't moved after switching factions initially.
             */
#if defined(SID_Turncoat) && (COMMON_SKILL_VALID(SID_Turncoat))
            for (int uid = gPlaySt.faction + 1; uid <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); uid++)
            {
                struct Unit * unit = GetUnit(uid);

                if (unit->_u3A == UES_BIT_TURNCOAT)
                    UnitChangeFaction(unit, FACTION_RED);
            }
#endif
            gPlaySt.faction = FACTION_RED;

            break;

        case FACTION_RED:
            gPlaySt.faction = FACTION_GREEN;
#if defined(SID_Turncoat) && (COMMON_SKILL_VALID(SID_Turncoat))
            for (int uid = gPlaySt.faction + 1; uid <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); uid++)
            {
                struct Unit * unit = GetUnit(uid);

                if (unit->_u3A == UES_BIT_TURNCOAT)
                    UnitChangeFaction(unit, FACTION_BLUE);
            }
#endif
            break;

        case FACTION_GREEN:
            gPlaySt.faction = FACTION_BLUE;

#if defined(SID_Turncoat) && (COMMON_SKILL_VALID(SID_Turncoat))
            for (int uid = gPlaySt.faction + 1; uid <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); uid++)
            {
                struct Unit * unit = GetUnit(uid);

                if (unit->_u3A == UES_BIT_TURNCOAT)
                    UnitChangeFaction(unit, FACTION_RED);
            }
#endif

            if (gPlaySt.chapterTurnNumber < 999)
                gPlaySt.chapterTurnNumber++;

            ProcessTurnSupportExp();
    }
}

struct EvCheck0A
{
    u32 unk0;
    u32 script;
    u8 x;
    u8 y;
    u16 tileCommand;
};

//! FE8U = 0x08083A58
LYN_REPLACE_CHECK(EvCheck0A_SHOP);
int EvCheck0A_SHOP(struct EventInfo * info)
{
    struct EvCheck0A * listScript = (void *)info->listScript;

    int x = listScript->x;
    int y = listScript->y;

    int tileCommand = listScript->tileCommand;

#if defined(SID_Secret) && (COMMON_SKILL_VALID(SID_Secret))
    if (SkillTester(gActiveUnit, SID_Secret))
    {
        info->script = listScript->script;
        info->flag = listScript->unk0 >> 16;
        info->commandId = tileCommand;
        return 1;
    }
#endif

    if ((x == info->xPos) && (y == info->yPos))
    {
        if ((tileCommand != TILE_COMMAND_SECRET || (GetUnitItemSlot(gActiveUnit, ITEM_MEMBERCARD) != -1)))
        {
            info->script = listScript->script;
            info->flag = listScript->unk0 >> 16;
            info->commandId = tileCommand;
            return 1;
        }
    }

    return 0;
}

LYN_REPLACE_CHECK(BeginUnitPoisonDamageAnim);
void BeginUnitPoisonDamageAnim(struct Unit * unit, int damage)
{

#if (defined(SID_PoisonHeal) && COMMON_SKILL_VALID(SID_PoisonHeal))
    if (SkillTester(gActiveUnit, SID_PoisonHeal))
    {
        BeginUnitHealAnim(gActiveUnit, damage);
        AddUnitHp(gActiveUnit, damage * 2); // A quick fix for poison damage applying, double the damage and heal it
        return;
    }
#endif

    BattleInitItemEffect(unit, -1);

    AddUnitHp(&gBattleActor.unit, -damage);

    if (gBattleActor.unit.curHP < 0)
    {
        gBattleActor.unit.curHP = 0;
    }

    gBattleHitIterator->hpChange = gBattleActor.hpInitial - gBattleActor.unit.curHP;

    if (gBattleActor.unit.curHP == 0)
    {
        gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;
    }

    BattleHitTerminate();

    BeginMapAnimForPoisonDmg();

    RenderMapForFogFadeIfUnitDied(unit);

    return;
}

LYN_REPLACE_CHECK(HasBattleUnitGainedWeaponLevel);
s8 HasBattleUnitGainedWeaponLevel(struct BattleUnit * bu)
{

#if (defined(SID_ShadowgiftPlus) && (COMMON_SKILL_VALID(SID_ShadowgiftPlus)))
    if (BattleSkillTester(bu, SID_ShadowgiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_DARK)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_DARK] == 0)
                return false;
#endif

#if (defined(SID_Shadowgift) && (COMMON_SKILL_VALID(SID_Shadowgift)))
    if (BattleSkillTester(bu, SID_SHadowgift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_DARK)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_DARK] == 0)
                return false;
#endif

#if (defined(SID_LuminaPlus) && (COMMON_SKILL_VALID(SID_LuminaPlus)))
    if (BattleSkillTester(bu, SID_LuminaPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_LIGHT)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_LIGHT] == 0)
                return false;
#endif

#if (defined(SID_StormgiftPlus) && (COMMON_SKILL_VALID(SID_StormgiftPlus)))
    if (BattleSkillTester(bu, SID_StormgiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_ANIMA)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_ANIMA] == 0)
                return false;
#endif

#if (defined(SID_Stormgift) && (COMMON_SKILL_VALID(SID_Stormgift)))
    if (BattleSkillTester(bu, SID_Stormgift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_ANIMA)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_ANIMA] == 0)
                return false;
#endif

#if (defined(SID_Gracegift) && (COMMON_SKILL_VALID(SID_Gracegift)))
    if (BattleSkillTester(bu, SID_Gracegift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_STAFF)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_STAFF] == 0)
                return false;
#endif

#if (defined(SID_GracegiftPlus) && (COMMON_SKILL_VALID(SID_GracegiftPlus)))
    if (BattleSkillTester(bu, SID_GracegiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_STAFF)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_STAFF] == 0)
                return false;
#endif

#if (defined(SID_Gracegift) && (COMMON_SKILL_VALID(SID_Gracegift)))
    if (BattleSkillTester(bu, SID_Gracegift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_STAFF)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_STAFF] == 0)
                return false;
#endif

#if (defined(SID_BladegiftPlus) && (COMMON_SKILL_VALID(SID_BladegiftPlus)))
    if (BattleSkillTester(bu, SID_BladegiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_SWORD)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_SWORD] == 0)
                return false;
#endif

#if (defined(SID_Bladegift) && (COMMON_SKILL_VALID(SID_Bladegift)))
    if (BattleSkillTester(bu, SID_Bladegift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_SWORD)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_SWORD] == 0)
                return false;
#endif

#if (defined(SID_PiercegiftPlus) && (COMMON_SKILL_VALID(SID_PiercegiftPlus)))
    if (BattleSkillTester(bu, SID_PiercegiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_LANCE)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_LANCE] == 0)
                return false;
#endif

#if (defined(SID_Piercegift) && (COMMON_SKILL_VALID(SID_Piercegift)))
    if (BattleSkillTester(bu, SID_Piercegift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_LANCE)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_LANCE] == 0)
                return false;
#endif

#if (defined(SID_HackgiftPlus) && (COMMON_SKILL_VALID(SID_HackgiftPlus)))
    if (BattleSkillTester(bu, SID_HackgiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_AXE)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_AXE] == 0)
                return false;
#endif

#if (defined(SID_Hackgift) && (COMMON_SKILL_VALID(SID_Hackgift)))
    if (BattleSkillTester(bu, SID_Hackgift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_AXE)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_AXE] == 0)
                return false;
#endif

#if (defined(SID_ArcgiftPlus) && (COMMON_SKILL_VALID(SID_ArcgiftPlus)))
    if (BattleSkillTester(bu, SID_ArcgiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_BOW)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_BOW] == 0)
                return false;
#endif

#if (defined(SID_Arcgift) && (COMMON_SKILL_VALID(SID_Arcgift)))
    if (BattleSkillTester(bu, SID_Arcgift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_BOW)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_BOW] == 0)
                return false;
#endif

    int oldWexp = bu->unit.ranks[bu->weaponType];
    int newWexp = GetBattleUnitUpdatedWeaponExp(bu);

    if (newWexp < 0)
        return FALSE;

    return GetWeaponLevelFromExp(oldWexp) != GetWeaponLevelFromExp(newWexp);
}

LYN_REPLACE_CHECK(GenerateFireTileTrapTargets);
void GenerateFireTileTrapTargets(int x, int y, int damage)
{
    FORCE_DECLARE bool dontAddTarget = false;

#if (defined(SID_Absolve) && (COMMON_SKILL_VALID(SID_Absolve)))
    if (SkillTester(GetUnit(gBmMapUnit[y][x]), SID_Absolve))
        dontAddTarget = true;
#endif

    if (!dontAddTarget)
        AddTarget(x, y, gBmMapUnit[y][x], damage);
}

LYN_REPLACE_CHECK(GenerateArrowTrapTargets);
void GenerateArrowTrapTargets(int x, int y, int damage)
{
    FORCE_DECLARE bool dontAddTarget = false;
    int iy;

    for (iy = 0; iy < gBmMapSize.y; ++iy)
    {
        if (gBmMapUnit[iy][x])
        {
#if (defined(SID_Absolve) && (COMMON_SKILL_VALID(SID_Absolve)))
            if (SkillTester(GetUnit(gBmMapUnit[iy][x]), SID_Absolve))
                dontAddTarget = true;
#endif
            if (!dontAddTarget)
                AddTarget(x, iy, gBmMapUnit[iy][x], damage);
        }
    }
}

LYN_REPLACE_CHECK(GenerateGasTrapTargets);
void GenerateGasTrapTargets(int x, int y, int damage, int facing)
{
    FORCE_DECLARE bool dontAddTarget = false;

    int i;

    int xInc = 0;
    int yInc = 0;

    switch (facing)
    {

        case FACING_UP:
            xInc = 0;
            yInc = -1;

            break;

        case FACING_DOWN:
            xInc = 0;
            yInc = +1;

            break;

        case FACING_LEFT:
            xInc = -1;
            yInc = 0;

            break;

        case FACING_RIGHT:
            xInc = +1;
            yInc = 0;

            break;

    } // switch (facing)

    for (i = 2; i >= 0; --i)
    {
        x += xInc;
        y += yInc;

        if (gBmMapUnit[y][x])
        {
#if (defined(SID_Absolve) && (COMMON_SKILL_VALID(SID_Absolve)))
            if (SkillTester(GetUnit(gBmMapUnit[y][x]), SID_Absolve))
                dontAddTarget = true;
#endif
            if (!dontAddTarget)
                AddTarget(x, y, gBmMapUnit[y][x], damage);
        }
    }
}

bool isWeaponTriangleAdvantage(int attackerWeapon, int defenderWeapon)
{
    if (attackerWeapon == ITYPE_SWORD && defenderWeapon == ITYPE_AXE)
        return true;
    if (attackerWeapon == ITYPE_LANCE && defenderWeapon == ITYPE_SWORD)
        return true;
    if (attackerWeapon == ITYPE_AXE && defenderWeapon == ITYPE_LANCE)
        return true;
    if (attackerWeapon == ITYPE_ANIMA && defenderWeapon == ITYPE_LIGHT)
        return true;
    if (attackerWeapon == ITYPE_LIGHT && defenderWeapon == ITYPE_DARK)
        return true;
    if (attackerWeapon == ITYPE_DARK && defenderWeapon == ITYPE_ANIMA)
        return true;

    return false;
}

bool weaponHasSpecialEffect(int weaponAttributes)
{
    if (weaponAttributes & IA_NEGATE_DEFENSE)
        return true;
    if (weaponAttributes & IA_NEGATE_CRIT)
        return true;
    if (weaponAttributes & IA_NEGATE_FLYING)
        return true;
    if (weaponAttributes & IA_REVERTTRIANGLE)
        return true;
    if (weaponAttributes & IA_UNCOUNTERABLE)
        return true;
    if (weaponAttributes & IA_BRAVE)
        return true;
    if (weaponAttributes & IA_UNBREAKABLE)
        return true;
    
    return false;
}

int findMax(u8 *array, int size) {
    u8 max = 0;
    FORCE_DECLARE u8 array_position = 0;

    for (int i = 0; i < size - 1; i++) {
        if (array[i] > max) {
            max = array[i];
            array_position = i;
        }
    }
    return array_position;
}
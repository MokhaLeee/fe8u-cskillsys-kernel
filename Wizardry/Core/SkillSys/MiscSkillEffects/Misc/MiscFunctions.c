#include "common-chax.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "battle-system.h"
#include "unit-expa.h"

extern void ForEachAdjacentUnit(int x, int y, void (*)(struct Unit *));

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
void TryAddUnitToTradeTargetList(struct Unit *unit)
{

    /**
     *  With capture, a unit should be able to trade with rescued enemies
     */
    if (
        !IsSameAllegiance(gSubjectUnit->index, unit->index) &&
#if (defined(SID_Capture) && (COMMON_SKILL_VALID(SID_Capture)))
        !SkillTester(gSubjectUnit, SID_Capture)
#endif
    )
    {
        return;
    }

    if (gSubjectUnit->pClassData->number == CLASS_PHANTOM || unit->pClassData->number == CLASS_PHANTOM)
    {
        return;
    }

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
        struct Unit *rescue = GetUnit(unit->rescue);

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
void MakeTradeTargetList(struct Unit *unit)
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
void KillUnitOnCombatDeath(struct Unit *unitA, struct Unit *unitB)
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
void TryAddUnitToRescueTargetList(struct Unit *unit)
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
void UnitDrop(struct Unit *actor, int xTarget, int yTarget)
{
    struct Unit *target = GetUnit(actor->rescue);

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
s8 Vanilla_CanUnitCrossTerrain(struct Unit *unit, int terrain)
{
    const s8 *lookup = (s8 *)GetUnitMovementCost(unit);
    return (lookup[terrain] > 0) ? TRUE : FALSE;
}

bool Generic_CanUnitBeOnPos(struct Unit *unit, s8 x, s8 y, int x2, int y2)
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
            struct Unit *unit = GetUnit(uid);

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
            struct Unit *unit = GetUnit(uid);

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
            struct Unit *unit = GetUnit(uid);

            if (unit->_u3A == UES_BIT_TURNCOAT)
                UnitChangeFaction(unit, FACTION_RED);
        }
#endif

        if (gPlaySt.chapterTurnNumber < 999)
            gPlaySt.chapterTurnNumber++;

        ProcessTurnSupportExp();
    }
}

struct EvCheck0A {
    u32 unk0;
    u32 script;
    u8 x;
    u8 y;
    u16 tileCommand;
};

//! FE8U = 0x08083A58
LYN_REPLACE_CHECK(EvCheck0A_SHOP);
int EvCheck0A_SHOP(struct EventInfo* info) {
    struct EvCheck0A* listScript = (void *)info->listScript;

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

    if ((x == info->xPos) && (y == info->yPos)) {
        if ((tileCommand != TILE_COMMAND_SECRET || (GetUnitItemSlot(gActiveUnit, ITEM_MEMBERCARD) != -1))) {
            info->script = listScript->script;
            info->flag = listScript->unk0 >> 16;
            info->commandId = tileCommand;
            return 1;
        }
    }

    return 0;
}
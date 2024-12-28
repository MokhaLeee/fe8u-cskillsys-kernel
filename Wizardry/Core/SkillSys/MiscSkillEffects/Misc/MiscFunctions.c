#include "common-chax.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "battle-system.h"
#include "unit-expa.h"
#include "eventinfo.h"
#include "bmmenu.h"
#include "eventscript.h"
#include "EAstdlib.h"
#include "worldmap.h"
#include "constants/texts.h"
#include "jester_headers/event-call.h"
#include "jester_headers/macros.h"
#include "jester_headers/soundtrack-ids.h"
#include "jester_headers/maps.h"
#include "jester_headers/flags.h"
#include "jester_headers/miscellaenous.h"
#include "debuff.h"
#include "jester_headers/custom-structs.h"
#include "jester_headers/custom-functions.h"
#include "action-expa.h"

#ifdef CONFIG_BEXP
    extern u16 sBEXP[CONFIG_BEXP];
#endif

// Misc Functions

extern void ForEachAdjacentUnit(int x, int y, void (*)(struct Unit *));

extern void GenerateFireTileTrapTargets(int x, int y, int damage);
extern void GenerateArrowTrapTargets(int x, int y, int damage);
extern void GenerateGasTrapTargets(int x, int y, int damage, int facing);

extern const u16 * Events_WM_Beginning[];
extern const u16 * Events_WM_ChapterIntro[];

typedef struct {
    /* 00 */ struct Font font;
    /* 18 */ struct Text th[18];
} PrepItemSuppyText;

struct ProcCmd CONST_DATA ProcScr_PrepItemScreen[] = {
    PROC_SLEEP(0),

    PROC_CALL(PrepItemScreen_Init),
    PROC_CALL(PrepItemScreen_SetupGfx),
    PROC_CALL(sub_8098CC0),

    PROC_CALL_ARG(NewFadeIn, 16),
    PROC_WHILE(FadeInExists),

    PROC_SLEEP(0),

    PROC_GOTO(1),

PROC_LABEL(0),
    PROC_CALL(sub_8098CC0),

    // fallthrough

PROC_LABEL(1),
    PROC_REPEAT(sub_8099120),
    PROC_CALL(DisableAllGfx),
    PROC_SLEEP(0),

    PROC_CALL(PrepItemScreen_StartStatScreen),
    PROC_SLEEP(0),

    PROC_CALL(PrepItemScreen_ResumeFromStatScreen),
    PROC_CALL(sub_8098CC0),
    PROC_SLEEP(0),

    PROC_CALL(EnableAllGfx),

    PROC_GOTO(1),

PROC_LABEL(2),
    PROC_CALL(sub_80995D4),
    PROC_CALL(PrepItemScreen_DrawFunds),
    PROC_SLEEP(0),

    PROC_CALL(sub_8099654),

    // fallthrough

PROC_LABEL(3),
    PROC_REPEAT(sub_80996E8),

    // fallthrough

PROC_LABEL(4),
    PROC_CALL(PrepItemScreen_HideFunds),
    PROC_CALL(sub_8099AF8),
    PROC_SLEEP(0),

    PROC_CALL(sub_8099C60),

    // fallthrough

PROC_LABEL(5),
    PROC_REPEAT(PrepItemScreen_Loop_MainKeyHandler),
    PROC_CALL(DisableAllGfx),
    PROC_SLEEP(0),

    PROC_CALL(PrepItemScreen_StartStatScreen),
    PROC_SLEEP(0),

    PROC_CALL(PrepItemScreen_ResumeFromStatScreen),
    PROC_CALL(sub_8099AF8),
    PROC_CALL(sub_8099C60),
    PROC_SLEEP(0),

    PROC_CALL(EnableAllGfx),

    PROC_GOTO(5),

PROC_LABEL(6),
    PROC_CALL_ARG(NewFadeOut, 16),
    PROC_WHILE(FadeOutExists),

    PROC_CALL(StartPrepItemTradeScreen),
    PROC_SLEEP(0),

    PROC_CALL(PrepItemScreen_SetupGfx),
    PROC_SLEEP(0),

    PROC_CALL(sub_8099AF8),
    PROC_CALL(sub_8099C60),

    PROC_CALL_ARG(NewFadeIn, 16),
    PROC_WHILE(FadeInExists),

    PROC_GOTO(5),

PROC_LABEL(8),
    PROC_CALL_ARG(NewFadeOut, 16),
    PROC_WHILE(FadeOutExists),

    PROC_CALL(PrepItemScreen_OnEnd),
    PROC_CALL(StartPrepItemListScreen),
    PROC_SLEEP(0),

    PROC_CALL(PrepItemScreen_SetupGfx),
    PROC_SLEEP(0),

    PROC_CALL(sub_8034194),
    PROC_CALL(sub_80996B0),
    PROC_CALL(sub_80995D4),
    PROC_CALL(PrepItemScreen_DrawFunds),
    PROC_CALL(sub_8099654),

    PROC_CALL_ARG(NewFadeIn, 16),
    PROC_WHILE(FadeInExists),

    PROC_GOTO(3),

PROC_LABEL(9),
    PROC_CALL_ARG(NewFadeOut, 16),
    PROC_WHILE(FadeOutExists),

    PROC_CALL(PrepItemScreen_OnEnd),
    PROC_CALL(StartPrepItemUse),
    PROC_SLEEP(0),

    PROC_CALL(PrepItemScreen_SetupGfx),
    PROC_SLEEP(0),

    PROC_CALL(sub_8034194),
    PROC_CALL(sub_80996B0),
    PROC_CALL(sub_80995D4),
    PROC_CALL(PrepItemScreen_DrawFunds),
    PROC_CALL(sub_8099654),

    PROC_CALL_ARG(NewFadeIn, 16),
    PROC_WHILE(FadeInExists),

    PROC_GOTO(3),

PROC_LABEL(10),
    PROC_CALL_ARG(NewFadeOut, 16),
    PROC_WHILE(FadeOutExists),

    PROC_CALL(PrepItemScreen_OnEnd),
    PROC_CALL(StartPrepItemSupply),
    PROC_SLEEP(0),
    PROC_CALL(PrepItemScreen_SetupGfx),
    PROC_SLEEP(0),
    PROC_CALL(sub_8034194),
    PROC_CALL(sub_80996B0),
    PROC_CALL(sub_80995D4),
    PROC_CALL(PrepItemScreen_DrawFunds),
    PROC_CALL(sub_8099654),

    PROC_CALL_ARG(NewFadeIn, 16),
    PROC_WHILE(FadeInExists),

    PROC_GOTO(3),

PROC_LABEL(11),
    PROC_CALL_ARG(NewFadeOut, 16),
    PROC_WHILE(FadeOutExists),

    PROC_CALL(PrepItemScreen_OnEnd),
    PROC_CALL(StartPrepSell),
    PROC_SLEEP(0),
    PROC_CALL(PrepItemScreen_SetupGfx),
    PROC_SLEEP(0),
    PROC_CALL(sub_8034194),
    PROC_CALL(sub_80996B0),
    PROC_CALL(sub_80995D4),
    PROC_CALL(PrepItemScreen_DrawFunds),
    PROC_CALL(sub_8099654),

    PROC_CALL_ARG(NewFadeIn, 16),
    PROC_WHILE(FadeInExists),

    PROC_GOTO(3),

PROC_LABEL(12),
    PROC_CALL_ARG(NewFadeOut, 16),
    PROC_WHILE(FadeOutExists),

    PROC_CALL(PrepItemScreen_OnEnd),
    PROC_CALL(StartPrepArmory),
    PROC_SLEEP(0),
    PROC_CALL(PrepItemScreen_SetupGfx),
    PROC_SLEEP(0),
    PROC_CALL(sub_8034194),
    PROC_CALL(sub_80996B0),
    PROC_CALL(sub_80995D4),
    PROC_CALL(PrepItemScreen_DrawFunds),
    PROC_CALL(sub_8099654),

    PROC_CALL_ARG(NewFadeIn, 16),
    PROC_WHILE(FadeInExists),

    PROC_GOTO(3),

PROC_LABEL(13),
    PROC_CALL_ARG(NewFadeOut, 16),
    PROC_WHILE(FadeOutExists),

    PROC_CALL(PrepItemScreen_OnEnd),

    PROC_CALL(sub_8099E68),

    PROC_END,

/* BEXP proc */
PROC_LABEL(14),
    PROC_CALL_ARG(NewFadeOut, 16),
    PROC_WHILE(FadeOutExists),

    PROC_CALL(PrepItemScreen_OnEnd),
    PROC_CALL(StartPrepSell),
    PROC_SLEEP(0),
    PROC_CALL(PrepItemScreen_SetupGfx),
    PROC_SLEEP(0),
    PROC_CALL(sub_8034194),
    PROC_CALL(sub_80996B0),
    PROC_CALL(sub_80995D4),
    PROC_CALL(PrepItemScreen_DrawFunds),
    PROC_CALL(sub_8099654),

    PROC_CALL_ARG(NewFadeIn, 16),
    PROC_WHILE(FadeInExists),

    PROC_GOTO(3),
};


// struct PrepItemSuppyText PrepItemSuppyTexts = {};

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

/* Let rescued units move after the rescuer dies */
#ifdef CONFIG_DEATH_DANCE
    if (UNIT_FACTION(target) == gPlaySt.faction && actor->curHP != 0)
        target->state |= US_UNSELECTABLE; // TODO: US_GRAYED    
#else
    if (UNIT_FACTION(target) == gPlaySt.faction)
        target->state |= US_UNSELECTABLE; // TODO: US_GRAYED
#endif

    actor->rescue = 0;
    target->rescue = 0;

    target->xPos = xTarget;
    target->yPos = yTarget;

    /* If we've captured an enemy, dropping them will kill them immediately */
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
    if (BattleFastSkillTester(bu, SID_ShadowgiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_DARK)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_DARK] == 0)
                return false;
#endif

#if (defined(SID_Shadowgift) && (COMMON_SKILL_VALID(SID_Shadowgift)))
    if (BattleFastSkillTester(bu, SID_SHadowgift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_DARK)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_DARK] == 0)
                return false;
#endif

#if (defined(SID_LuminaPlus) && (COMMON_SKILL_VALID(SID_LuminaPlus)))
    if (BattleFastSkillTester(bu, SID_LuminaPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_LIGHT)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_LIGHT] == 0)
                return false;
#endif

#if (defined(SID_StormgiftPlus) && (COMMON_SKILL_VALID(SID_StormgiftPlus)))
    if (BattleFastSkillTester(bu, SID_StormgiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_ANIMA)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_ANIMA] == 0)
                return false;
#endif

#if (defined(SID_Stormgift) && (COMMON_SKILL_VALID(SID_Stormgift)))
    if (BattleFastSkillTester(bu, SID_Stormgift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_ANIMA)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_ANIMA] == 0)
                return false;
#endif

#if (defined(SID_Gracegift) && (COMMON_SKILL_VALID(SID_Gracegift)))
    if (BattleFastSkillTester(bu, SID_Gracegift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_STAFF)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_STAFF] == 0)
                return false;
#endif

#if (defined(SID_GracegiftPlus) && (COMMON_SKILL_VALID(SID_GracegiftPlus)))
    if (BattleFastSkillTester(bu, SID_GracegiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_STAFF)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_STAFF] == 0)
                return false;
#endif

#if (defined(SID_Gracegift) && (COMMON_SKILL_VALID(SID_Gracegift)))
    if (BattleFastSkillTester(bu, SID_Gracegift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_STAFF)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_STAFF] == 0)
                return false;
#endif

#if (defined(SID_BladegiftPlus) && (COMMON_SKILL_VALID(SID_BladegiftPlus)))
    if (BattleFastSkillTester(bu, SID_BladegiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_SWORD)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_SWORD] == 0)
                return false;
#endif

#if (defined(SID_Bladegift) && (COMMON_SKILL_VALID(SID_Bladegift)))
    if (BattleFastSkillTester(bu, SID_Bladegift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_SWORD)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_SWORD] == 0)
                return false;
#endif

#if (defined(SID_PiercegiftPlus) && (COMMON_SKILL_VALID(SID_PiercegiftPlus)))
    if (BattleFastSkillTester(bu, SID_PiercegiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_LANCE)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_LANCE] == 0)
                return false;
#endif

#if (defined(SID_Piercegift) && (COMMON_SKILL_VALID(SID_Piercegift)))
    if (BattleFastSkillTester(bu, SID_Piercegift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_LANCE)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_LANCE] == 0)
                return false;
#endif

#if (defined(SID_HackgiftPlus) && (COMMON_SKILL_VALID(SID_HackgiftPlus)))
    if (BattleFastSkillTester(bu, SID_HackgiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_AXE)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_AXE] == 0)
                return false;
#endif

#if (defined(SID_Hackgift) && (COMMON_SKILL_VALID(SID_Hackgift)))
    if (BattleFastSkillTester(bu, SID_Hackgift))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_AXE)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_AXE] == 0)
                return false;
#endif

#if (defined(SID_ArcgiftPlus) && (COMMON_SKILL_VALID(SID_ArcgiftPlus)))
    if (BattleFastSkillTester(bu, SID_ArcgiftPlus))
        if (GetItemType(GetUnit(bu->unit.index)->items[0]) == ITYPE_BOW)
            if (GetUnit(bu->unit.index)->ranks[ITYPE_BOW] == 0)
                return false;
#endif

#if (defined(SID_Arcgift) && (COMMON_SKILL_VALID(SID_Arcgift)))
    if (BattleFastSkillTester(bu, SID_Arcgift))
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

u8 EscapeCommandUsability(const struct MenuItemDef* def, int number) {

    if (gActiveUnit->pClassData->number == CLASS_PHANTOM) {
        return MENU_NOTSHOWN;
    }

    if (gActiveUnit->state & US_HAS_MOVED) {
        return MENU_NOTSHOWN;
    }

    return GetAvailableTileEventCommand(gActiveUnit->xPos, gActiveUnit->yPos) == TILE_COMMAND_VISIT
    ? MENU_ENABLED : MENU_NOTSHOWN;
}

u8 EscapeCommandEffect(struct MenuProc* menu, struct MenuItemProc* menuItem) {

    StartAvailableTileEvent(gActiveUnit->xPos, gActiveUnit->yPos);
    
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

LYN_REPLACE_CHECK(Event19_Checks);
u8 Event19_Checks(struct EventEngineProc * proc)
{
    u8 subcode = EVT_SUB_CMD(proc->pEventCurrent);
    switch (subcode) {
    case EVSUBCMD_CHECK_MODE: // Check Mode
        gEventSlots[0xC] = gPlaySt.chapterModeIndex;
        break;

    case EVSUBCMD_CHECK_CHAPTER_NUMBER: // Check (Next?) Chapter Index
        gEventSlots[0xC] = proc->chapterIndex;
        break;

    case EVSUBCMD_CHECK_HARD: // Check Difficult Mode
        if (!(gPlaySt.chapterStateBits & PLAY_FLAG_HARD))
            gEventSlots[0xC] = FALSE;
        else
            gEventSlots[0xC] = TRUE;

        break;

    case EVSUBCMD_CHECK_TURNS: // Check Turn Number
        gEventSlots[0xC] = gPlaySt.chapterTurnNumber;
        break;

    case EVSUBCMD_CHECK_ENEMIES: // Check Red Unit Count
        gEventSlots[0xC] = CountRedUnits();
        break;

    case EVSUBCMD_CHECK_OTHERS: // Check Green Unit Count
        gEventSlots[0xC] = CountGreenUnits();
        break;

    case EVSUBCMD_CHECK_PLAYERS: // Check Blue Unit Count
        gEventSlots[0xC] = CountAvailableBlueUnits();
        break;

    case EVSUBCMD_CHECK_SKIRMISH: // Check Chapter Type?
        gEventSlots[0xC] = GetBattleMapKind();
        break;

    case EVSUBCMD_CHECK_TUTORIAL: // Check Some option or difficult mode
        if (gPlaySt.config.controller || (gPlaySt.chapterStateBits & PLAY_FLAG_HARD))
            gEventSlots[0xC] = FALSE;
        else
            gEventSlots[0xC] = TRUE;

        break;

    case EVSUBCMD_CHECK_MONEY: // Check gold
        gEventSlots[0xC] = GetPartyGoldAmount();
        break;

    case EVSUBCMD_19CHECK_EVENTID: // Check Event Trigger ID
        gEventSlots[0xC] = GetEventTriggerId(proc->pEventStart);
        break;

    case EVSUBCMD_CHECK_POSTGAME: // Check Game Complete
        if (gPlaySt.chapterStateBits & PLAY_FLAG_COMPLETE)
            gEventSlots[0xC] = TRUE;
        else
            gEventSlots[0xC] = FALSE;

        break;

    default:
        break;
    } // switch (subcode)

    return EVC_ADVANCE_CONTINUE;
}

#ifdef CONFIG_FE8_REWRITE
LYN_REPLACE_CHECK(WorldMap_CallBeginningEvent);
//! FE8U = 0x080BA334
void WorldMap_CallBeginningEvent(struct WorldMapMainProc * proc)
{
    int chIndex;
    int node_next;

    Sound_FadeOutBGM(4);

    if ((gGMData.state.bits.monster_merged) || (gPlaySt.chapterStateBits & PLAY_FLAG_POSTGAME))
    {
        sub_80BA008(proc->timer);
    }
    else
    {
        int loc = gGMData.units[0].location;

        gGMData.current_node = loc;
        node_next = WMLoc_GetNextLocId(loc);

        if (node_next > -1)
        {
            chIndex = WMLoc_GetChapterId(node_next);

            gPlaySt.chapterIndex = chIndex;
            

            if (Events_WM_Beginning[GetROMChapterStruct(chIndex)->gmapEventId] == NULL)
                return;

            ResetGmStoryNode();
            proc->gm_icon->merge_next_node = false;

            /**
             * Jester - I've resorted to hooking into the WM call function to directly load the
             * WM events I want based on the supplied eventSCR. It's an unfortunate bit of hardcoding
             * I'm looking to remove, but it frees me from having to rely on the list in ASM in vanilla.
             */
            int eventID = GetROMChapterStruct(chIndex)->gmapEventId;

            NoCashGBAPrintf("event id is: %d", eventID);
            
            switch (eventID) {
            case 55:
                CallEvent((const u16 *)EventScrWM_Ch1_ENDING, 0);
                break;
            case 2:
                // CallEvent((const u16 *)EventScrWM_Ch2_SET_NODE, 0);
                break;
            case 3:
                CallEvent((const u16 *)EventScrWM_Ch2_SET_NODE, 0);
                break;
            case 4:
                CallEvent((const u16 *)EventScrWM_Ch3_SET_NODE, 0);
                break;
            case 5:
                CallEvent((const u16 *)EventScrWM_Ch4_SET_NODE, 0);
                break;
            // case 6:
            //     break;
            case 7:
                CallEvent((const u16 *)EventScrWM_Ch5_SET_NODE, 0);
                break;
            // case 8:
            //     CallEvent((const u16 *)EventScrWM_Ch7_SET_NODE, 0);
            //     break;
            // case 9:
            //     CallEvent((const u16 *)EventScrWM_Ch8_SET_NODE, 0);
            //     break;
            // case 10:
            //     CallEvent((const u16 *)EventScrWM_Ch9_SET_NODE, 0);
            //     break;
            // case 11:
            //     CallEvent((const u16 *)EventScrWM_Ch10_SET_NODE, 0);
            //     break;
            default: 
                CallEvent(Events_WM_Beginning[eventID], 0);
                break;
            }
        }
    }

    StartWMFaceCtrl(proc);
    StartGmapMuEntry(NULL);
}

LYN_REPLACE_CHECK(CallChapterWMIntroEvents);
//! FE8U = 0x080BA3D4
void CallChapterWMIntroEvents(ProcPtr proc)
{
    if (Events_WM_ChapterIntro[GetROMChapterStruct(gPlaySt.chapterIndex)->gmapEventId] != NULL)
    {

         /**
        * Jester - I've resorted to hooking into the WM call function to directly load the
        * WM events I want based on the supplied eventSCR. It's an unfortunate bit of hardcoding
        * I'm looking to remove, but it frees me from having to rely on the list in ASM in vanilla.
        */
        int eventID = GetROMChapterStruct(gPlaySt.chapterIndex)->gmapEventId;
            
        switch (eventID) {
        case 55:
            // CallEvent((const u16 *)EventScrWM_Ch1_ENDING, 0);
            break;
        case 2:
            // CallEvent((const u16 *)EventScrWM_Ch2_TRAVEL_TO_NODE, 0);
            break;
        case 3:
            CallEvent((const u16 *)EventScrWM_Ch2_TRAVEL_TO_NODE, 0);
            break;
        case 4:
            CallEvent((const u16 *)EventScrWM_Ch3_TRAVEL_TO_NODE, 0);
            break;
        case 5:
            CallEvent((const u16 *)EventScrWM_Ch4_TRAVEL_TO_NODE, 0);
            break;
        // case 6:
        //     break;
        case 7:
            CallEvent((const u16 *)EventScrWM_Ch5_TRAVEL_TO_NODE, 0);
            break;
        // case 8:
        //     CallEvent((const u16 *)EventScrWM_Ch7_SET_NODE, 0);
        //     break;
        // case 9:
        //     CallEvent((const u16 *)EventScrWM_Ch8_SET_NODE, 0);
        //     break;
        // case 10:
        //     CallEvent((const u16 *)EventScrWM_Ch9_SET_NODE, 0);
        //     break;
        // case 11:
        //     CallEvent((const u16 *)EventScrWM_Ch10_SET_NODE, 0);
        //     break;
        default: 
            CallEvent(Events_WM_ChapterIntro[eventID], 0);
            break;
        }
        StartWMFaceCtrl(proc);
        StartGmapMuEntry(NULL);
    }
}
#endif

LYN_REPLACE_CHECK(Event97_WmInitNextStoryNode);
//! FE8U = 0x0800C2DC
u8 Event97_WmInitNextStoryNode(struct EventEngineProc * proc)
{
    // struct WorldMapMainProc * worldMapProc;

    int nodeId = WMLoc_GetNextLocId(gGMData.current_node);

    if (nodeId < 0)
    {
        return EVC_ADVANCE_CONTINUE;
    }

    if (EVENT_IS_SKIPPING(proc))
    {
        ResetGmStoryNode();
        gGMData.nodes[nodeId].state |= 1;
        gGMData.nodes[nodeId].state |= 2;

        GM_ICON->nodeId = nodeId;
        GM_ICON->merge_next_node = true;
    }
    else
    {
        if (!(gGMData.nodes[nodeId].state & 1))
        {
            StartGmBaseEntry(nodeId, 0, NULL);
            ResetGmStoryNode();
            gGMData.nodes[nodeId].state |= 2;
        }
    }

    return EVC_ADVANCE_CONTINUE;
};

LYN_REPLACE_CHECK(Event3E_PrepScreenCall);
//! FE8U = 0x08010968
u8 Event3E_PrepScreenCall(struct EventEngineProc * proc)
{
    HideAllUnits();
    ClearFlag(0x84);
    Proc_StartBlocking(gProcScr_SALLYCURSOR, proc);

    return EVC_ADVANCE_YIELD;
}

void GiveScroll(void)
{
    u16 skillId = gEventSlots[EVT_SLOT_3];
    u16 charId = gEventSlots[EVT_SLOT_4];

    struct Unit * unit;
    unit = GetUnitFromCharId(charId);

    for (int i = 0; i < 5; i++) {
        if(unit->items[i] == ((SID_Supply << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL))
        {
            unit->items[i] = (skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL;
            break;
        }
    }

    unsigned short *items;
    items = GetConvoyItemArray();

    for (int i = 0; i < CONFIG_INSTALL_CONVOYEXPA_AMT; i++) {
        if(items[i] == ((SID_Supply << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL))
        {
            items[i] = (skillId << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL;
            break;
        }
    }
}

LYN_REPLACE_CHECK(ItemGot_GotLeItem);
void ItemGot_GotLeItem(struct GotItemPopupProc * proc)
{
    /* Stop gap measure to enable scrolls to have the right IDs
    ** They will return a short (maybe a word?) with the first half being the skill ID
    ** and the last part being the skill scroll ID, which is currently 0xBD.
    ** Check if the item we're giving is above the byte limit to determine that
    ** we're giving a scroll.
    
    ** The limitation of this approach is that it locks us out of having other dynamically
    ** assigned effects and icons for other items in the future. It might be best to create
    ** another version of this function in that case
    */
    if (proc->item > 255)
        HandleNewItemGetFromDrop(proc->unit, proc->item, proc);
    else
        HandleNewItemGetFromDrop(proc->unit, MakeNewItem(proc->item), proc);
}

void IsTraineeLevelCappedOrPromoted(void)
{
    u16 charId = gEventSlots[EVT_SLOT_3];
    u16 level = gEventSlots[EVT_SLOT_4];
    u16 class = gEventSlots[EVT_SLOT_5];

    struct Unit * unit;
    unit = GetUnitFromCharId(charId);

    if (unit->level >= level || unit->pClassData->number != class)
        gEventSlots[EVT_SLOT_C] = true;
    else
        gEventSlots[EVT_SLOT_C] = false;
}

//! FE8U = 0x08084800
LYN_REPLACE_CHECK(IsCharacterForceDeployed_);
bool IsCharacterForceDeployed_(u16 pid)
{
    const struct ForceDeploymentEnt gForceDeploymentList[] = {
        {CHARACTER_EIRIKA,  CHAPTER_MODE_COMMON,  -1  },
        {CHARACTER_EIRIKA,  CHAPTER_MODE_EIRIKA,  -1  },
        {CHARACTER_EPHRAIM, CHAPTER_MODE_EPHRAIM, -1  },
        {CHARACTER_ARTUR,   -1,                    4  },
        {CHARACTER_NATASHA, -1,                    6  },
        {CHARACTER_JOSHUA,  -1,                    6  },
        {CHARACTER_TANA,    -1,                    10 },
        {CHARACTER_SALEH,   -1,                    12 },
        {CHARACTER_EPHRAIM, CHAPTER_MODE_EIRIKA,   21 },
        {CHARACTER_EIRIKA,  CHAPTER_MODE_EPHRAIM,  34 },
        {-1, 0, 0},
    };

    const struct ForceDeploymentEnt * it;

    for (it = gForceDeploymentList; it->pid != (u16)-1; it++)
    {
        if (it->route != 0xFF && it->route != gPlaySt.chapterModeIndex)
            continue;

        if (it->chapter != 0xFF && it->chapter != gPlaySt.chapterIndex)
            continue;

        if (pid != it->pid)
            continue;

        return true;
    }
    return false;
}

LYN_REPLACE_CHECK(GetUnitEquippedWeaponSlot);
int GetUnitEquippedWeaponSlot(struct Unit* unit) {

    if (GetUnitStatusIndex(unit) == NEW_UNIT_STATUS_BREAK)
        return -1;

    int i;

    for (i = 0; i < UNIT_ITEM_COUNT; ++i)
        if (CanUnitUseWeaponNow(unit, unit->items[i]) == TRUE)
            return i;

    return -1;
}

LYN_REPLACE_CHECK(GetUnitEquippedWeapon);
u16 GetUnitEquippedWeapon(struct Unit* unit) {
    
    if (GetUnitStatusIndex(unit) == NEW_UNIT_STATUS_BREAK)
        return 0;

    int i;

    for (i = 0; i < UNIT_ITEM_COUNT; ++i)
        if (CanUnitUseWeapon(unit, unit->items[i]) == TRUE)
            return unit->items[i];

    return 0;
}

LYN_REPLACE_CHECK(AddArrowTrap);
void AddArrowTrap(int x, int turnCountdown, int turnInterval)
{
    if(gEventSlots[EVT_SLOT_9] == 99)
        AddDamagingTrap(gEventSlots[EVT_SLOT_3], 0, TRAP_LIGHTARROW, 0, gEventSlots[EVT_SLOT_4], gEventSlots[EVT_SLOT_5], 10);
    else
        AddDamagingTrap(x, 0, TRAP_LIGHTARROW, 0, turnCountdown, turnInterval, 10);
}

//! FE8U = 0x08010D28
LYN_REPLACE_CHECK(Event44_BreakingSacredStone);
u8 Event44_BreakingSacredStone(struct EventEngineProc * proc)
{
    struct Unit * unit;

    if (EVENT_IS_SKIPPING(proc))
    {
        return EVC_ADVANCE_CONTINUE;
    }

    if (gEventSlots[EVT_SLOT_9] == 99) 
        unit = GetUnit(gBattleTarget.unit.index);
    else
        unit = GetUnitStructFromEventParameter(EVT_CMD_ARGV(proc->pEventCurrent)[0]);

    if (!unit)
    {
        return EVC_ERROR;
    }

    StartStoneShatterAnim(unit, proc);

    return EVC_ADVANCE_YIELD;
}

LYN_REPLACE_CHECK(InitSubtitleHelpText);
void InitSubtitleHelpText(struct SubtitleHelpProc * proc)
{
    const char * iter;
    int line;
    /* Originally an int in the decomp, but won't compile unless it's u32 */
    u32 width;

    iter = proc->string;

#ifdef CONFIG_EXTENDED_HELPBOXES
    InitSpriteTextFont(&proc->font, OBJ_VRAM0 + 0x5800, 0x14);
#else
    InitSpriteTextFont(&proc->font, OBJ_VRAM0 + 0x4800, 0x14);
#endif
    SetTextFontGlyphs(1);

    ApplyPalette(gUnknown_0859EF20, 0x14);

    for (line = 0; line < 2; line++) {
        InitSpriteText(proc->text + line);

        SpriteText_DrawBackgroundExt(proc->text + line, 0);
        Text_SetColor(proc->text + line, 0);
    }

    line = 0;

    if (iter != 0) {
        while (*iter > 1) {

            iter = Text_DrawCharacter(proc->text + line, iter);

            if (Text_GetCursor(proc->text + line) > 0xE0) {

                iter -= 2;
                line++;

                GetCharTextLen(iter, &width);

                Text_SetCursor(proc->text + line, (Text_GetCursor(proc->text) - width) - 0xC0);
            }
        }

        proc->textCount = ((GetStringTextLen(proc->string) + 16) >> 5) + 1;
        proc->textNum = proc->textCount - 1;
    }

    SetTextFont(0);

    return;
}

LYN_REPLACE_CHECK(PrepItemScreen_SetupGfx);
//! FE8U = 0x08098620
void PrepItemScreen_SetupGfx(struct PrepItemScreenProc * proc)
{
    int i;

    struct FaceVramEntry faceConfig[4] = {
        { 0x5800, 6, },
        { 0x6800, 7, },
        { 0x0000, 0, },
        { 0x0000, 0, },
    };

    SetupBackgrounds(gBgConfig_ItemUseScreen);

    gLCDControlBuffer.dispcnt.mode = 0;

    SetupFaceGfxData(faceConfig);

    SetDispEnable(0, 0, 0, 0, 0);

    SetupDebugFontForOBJ(0x06017800, 0);

    gLCDControlBuffer.bg0cnt.priority = 0;
    gLCDControlBuffer.bg1cnt.priority = 2;
    gLCDControlBuffer.bg2cnt.priority = 1;
    gLCDControlBuffer.bg3cnt.priority = 3;

    ResetText();
    ResetIconGraphics_();
    LoadIconPalettes(4);
    LoadUiFrameGraphics();
    ApplyPalette(gUiFramePaletteD, 2);
    LoadObjUIGfx();

    MakePrepUnitList();
    proc->hoverUnitIdx = UnitGetIndexInPrepList(PrepGetLatestCharId());

    ResetSysHandCursor(proc);
    StartParallelWorker(sub_809A274, proc);
    StartUiCursorHand(proc);

    SetPrimaryHBlankHandler(NULL);

    BG_Fill(BG_GetMapBuffer(0), 0);
    BG_Fill(BG_GetMapBuffer(1), 0);
    BG_Fill(BG_GetMapBuffer(2), 0);

    gPaletteBuffer[PAL_BACKDROP_OFFSET] = 0;
    EnablePaletteSync();

    for (i = 0; i < 15; i++) {
        InitText(gPrepItemTexts + i, 5);
    }

    for (i = 0; i < 5; i++) {
        InitText(&gPrepItemTexts[15 + i], 7);
        InitText(&gPrepItemTexts[20 + i], 7);
    }

    InitTextDb(&gPrepItemTexts[25], 8);
    InitTextDb(&gPrepItemTexts[26], 8);
    InitText(&gPrepItemTexts[27], 8);
    InitText(&gPrepItemTexts[30], 7);
    InitText(&gPrepItemTexts[31], 5);

#ifdef CONFIG_EXTENDED_HELPBOXES
    LoadHelpBoxGfx((void *)0x06012000, -1);    
#else
    LoadHelpBoxGfx((void *)0x06014000, -1);
#endif


    SetBlendBackdropA(0);
    SetBlendBackdropB(0);

    SetBlendTargetA(0, 0, 0, 0, 0);
    SetBlendTargetB(0, 0, 0, 0, 0);

    gLCDControlBuffer.wincnt.wout_enableBlend = 1;

    SetWinEnable(1, 0, 0);
    SetWin0Box(0, 4, 240, 68);
    SetWin0Layers(1, 1, 1, 1, 1);
    SetWOutLayers(1, 1, 0, 1, 1);

    BG_SetPosition(0, 4, -4);
    BG_SetPosition(1, 0, 4);
    BG_SetPosition(2, -40, (proc->unk_34 - 4) & 0xff);
    BG_EnableSyncByMask(7);

    ApplyUnitSpritePalettes();
    CpuFastFill(0, PAL_OBJ(0x0B), 0x20);

    ForceSyncUnitSpriteSheet();

    Decompress(Img_PrepTextShadow, (void *)0x06013E00);
    UiCursorHand_80ACA4C(0, 0, 0, 0xd0, 0x3c);
    DisplaySysHandCursorTextShadow(0x600, 1);

    RestartMuralBackground();

    if (proc->selectedUnitIdx != 0xff) {
        SetUiCursorHandConfig(
            0,
            ((proc->selectedUnitIdx % 3) * 64) + 24,
            ((proc->selectedUnitIdx / 3) * 16) + 4 - proc->unk_34,
            2
        );
        UpdatePrepItemScreenFace(0, GetUnitFromPrepList(proc->selectedUnitIdx), 60, 76, 0x0503);
    }

    StartMenuScrollBarExt(proc, 224, 11, 0x200, 4);

    UpdateMenuScrollBarConfig(6, proc->unk_34, ((PrepGetUnitAmount() - 1) / 3) + 1, 4);
    sub_8097668();
    PrepUpdateSMS();
    SetBlendConfig(1, 8, 8, 0);
    SetBlendTargetA(0, 1, 0, 0, 0);
    SetBlendTargetB(0, 1, 0, 0, 0);

    SetPrimaryHBlankHandler(PrepItemScreen_OnHBlank);

    StartDrawPrepFundsSprite(168, 133, 11, proc);

    StartDrawPrepUnitInfoBgSprites(proc);

    return;
}

LYN_REPLACE_CHECK(PrepItemTrade_Init);
//! FE8U = 0x0809B86C
void PrepItemTrade_Init(struct PrepMenuTradeProc * proc)
{
    const char* str;
    int i;

    struct FaceVramEntry faceConfig[4] = {
        { 0x5800, 6 },
        { 0x6800, 7 },
        { 0x0000, 0 },
        { 0x0000, 0 }
    };

    // TODO: Seems to be necessary to match - there is a "ldrh" here...
    SetupBackgrounds((void*)(u32)*gBgConfig_ItemUseScreen);

    SetupFaceGfxData(faceConfig);

    gLCDControlBuffer.bg0cnt.priority = 1;
    gLCDControlBuffer.bg1cnt.priority = 2;
    gLCDControlBuffer.bg2cnt.priority = 0;
    gLCDControlBuffer.bg3cnt.priority = 3;

    BG_Fill(BG_GetMapBuffer(0), 0);
    BG_Fill(BG_GetMapBuffer(1), 0);
    BG_Fill(BG_GetMapBuffer(2), 0);

    ResetText();
    ResetIconGraphics_();
    LoadUiFrameGraphics();
    LoadObjUIGfx();

    BG_SetPosition(0, 0, 0);
    BG_SetPosition(1, 0, 0);
    BG_SetPosition(2, 0, 0);

#ifdef CONFIG_EXTENDED_HELPBOXES
    LoadHelpBoxGfx((void*)0x06012000, -1);
#else
    LoadHelpBoxGfx((void*)0x06014000, -1);
#endif

    LoadIconPalettes(4);

    RestartMuralBackground();

    for (i = 0; i < 5; i++) {
        InitTextDb(gPrepItemTexts + 15 + i, 7);
        InitTextDb(gPrepItemTexts + 20 + i, 7);
    }

    proc->selectedItemSlot = 0xff;

    if (proc->unk_40 != -1) {
        proc->cursorItemSlot = proc->unk_40 + 8;
    } else {
        if (GetUnitItemCount(proc->units[0]) == 0) {
            proc->cursorItemSlot = 8;
        } else {
            proc->cursorItemSlot = 0;
        }
    }

    proc->helpBoxItemSlot = 0xff;

    StartFace2(0, GetUnitPortraitId(proc->units[0]), 64, -4, (FACE_DISP_KIND(FACE_96x80_FLIPPED) | FACE_DISP_HLAYER(FACE_HLAYER_3)));
    StartFace2(1, GetUnitPortraitId(proc->units[1]), 174, -4, (FACE_DISP_KIND(FACE_96x80) | FACE_DISP_HLAYER(FACE_HLAYER_3)));

    DrawUiFrame2(1, 8, 14, 12, 0);
    DrawUiFrame2(15, 8, 14, 12, 0);

    BG_EnableSyncByMask(7);

    str = GetStringFromIndex(proc->units[0]->pCharacterData->nameTextId);
    PutDrawText(0, gBG0TilemapBuffer, 0, ((48 - GetStringTextLen(str)) / 2), 6, str);

    str = GetStringFromIndex(proc->units[1]->pCharacterData->nameTextId);
    PutDrawText(0, gBG0TilemapBuffer + 0x18, 0, ((48 - GetStringTextLen(str)) / 2), 6, str);

    DrawPrepScreenItems(gBG0TilemapBuffer + 0x122, gPrepItemTexts + 15, proc->units[0], 0);
    DrawPrepScreenItems(gBG0TilemapBuffer + 0x130, gPrepItemTexts + 20, proc->units[1], 0);

    StartUiCursorHand(proc);

    ResetSysHandCursor(proc);
    DisplaySysHandCursorTextShadow(0x600, 1);
    ShowSysHandCursor(
        (proc->cursorItemSlot >> 3) * 0x70 + 0x10,
        (proc->cursorItemSlot & 7) * 0x10 + 0x48,
        0xb,
        0x800
    );

    StartHelpPromptSprite(200, 0x93, 2, proc);

    StartSysBrownBox(0xd, 0xe00, 0xf, 0xc00, 0x400, proc);

    EnableSysBrownBox(0, -0x28, -1, 1);
    EnableSysBrownBox(1, 0xb8, -1, 0);

    SetBlendConfig(1, 0xe, 4, 0);

    SetBlendTargetA(0, 0, 0, 0, 0);
    SetBlendTargetB(0, 0, 0, 1, 0);

    return;
}

LYN_REPLACE_CHECK(sub_809D914);
//! FE8U = 0x0809D914
void sub_809D914(struct PrepItemSupplyProc * proc)
{

    gLCDControlBuffer.dispcnt.mode = 0;
    SetupBackgrounds(NULL);

    BG_Fill(BG_GetMapBuffer(0), 0);
    BG_Fill(BG_GetMapBuffer(1), 0);
    BG_Fill(BG_GetMapBuffer(2), 0);

    gLCDControlBuffer.bg0cnt.priority = 1;
    gLCDControlBuffer.bg1cnt.priority = 2;
    gLCDControlBuffer.bg2cnt.priority = 0;
    gLCDControlBuffer.bg3cnt.priority = 3;

    ResetFaces();
    ResetText();
    ResetIconGraphics_();
    LoadUiFrameGraphics();
    LoadObjUIGfx();

    BG_SetPosition(0, 0, 0);
    BG_SetPosition(1, 0, 0);
    BG_SetPosition(2, 0, proc->yOffsetPerPage[proc->currentPage] - 0x28);

#ifdef CONFIG_EXTENDED_HELPBOXES
    LoadHelpBoxGfx((void*)0x06012000, -1);
#else
    LoadHelpBoxGfx((void*)0x06013000, -1);
#endif

    LoadIconPalettes(4);
    RestartMuralBackground();
    BG_EnableSyncByMask(8);

    gLCDControlBuffer.dispcnt.bg0_on = 0;
    gLCDControlBuffer.dispcnt.bg1_on = 0;
    gLCDControlBuffer.dispcnt.bg2_on = 0;
    gLCDControlBuffer.dispcnt.bg3_on = 0;
    gLCDControlBuffer.dispcnt.obj_on = 0;

    return;
}

LYN_REPLACE_CHECK(PrepItemList_InitGfx);
//! FE8U = 0x0809ED8C
void PrepItemList_InitGfx(struct PrepItemListProc * proc)
{
    int i;
    const char* str;

    PrepItemSuppyText PrepItemSuppyTexts;

    gLCDControlBuffer.dispcnt.mode = 0;

    SetupBackgrounds(NULL);

    BG_Fill(BG_GetMapBuffer(0), 0);
    BG_Fill(BG_GetMapBuffer(1), 0);
    BG_Fill(BG_GetMapBuffer(2), 0);

    gLCDControlBuffer.bg0cnt.priority = 1;
    gLCDControlBuffer.bg1cnt.priority = 2;
    gLCDControlBuffer.bg2cnt.priority = 0;
    gLCDControlBuffer.bg3cnt.priority = 3;

    ResetFaces();
    ResetText();
    ResetIconGraphics_();
    LoadUiFrameGraphics();
    LoadObjUIGfx();

    BG_SetPosition(0, 0, 0);
    BG_SetPosition(1, 0, 0);
    BG_SetPosition(2, 0, proc->yOffsetPerPage[proc->currentPage] - 40);

#ifdef CONFIG_EXTENDED_HELPBOXES
    LoadHelpBoxGfx((void*)0x06013000, -1);
#else
    LoadHelpBoxGfx((void*)0x06012000, -1);
#endif

    LoadIconPalettes(4);

    RestartMuralBackground();

    PutImg_PrepItemUseUnk(0x5000, 5);

    Decompress(gUnknown_08A1B9EC, gGenericBuffer);
    CallARM_FillTileRect(gBG1TilemapBuffer, gGenericBuffer, 0x1000);

    BG_EnableSyncByMask(7);

    StartUiCursorHand(proc);

    ResetSysHandCursor(proc);
    DisplaySysHandCursorTextShadow(0x600, 1);

    gLCDControlBuffer.dispcnt.win0_on = 1;
    gLCDControlBuffer.dispcnt.win1_on = 0;
    gLCDControlBuffer.dispcnt.objWin_on = 0;

    gLCDControlBuffer.win0_left = 128;
    gLCDControlBuffer.win0_top = 40;
    gLCDControlBuffer.win0_right = 224;
    gLCDControlBuffer.win0_bottom = 152;

    gLCDControlBuffer.wincnt.win0_enableBg0 = 1;
    gLCDControlBuffer.wincnt.win0_enableBg1 = 1;
    gLCDControlBuffer.wincnt.win0_enableBg2 = 1;
    gLCDControlBuffer.wincnt.win0_enableBg3 = 1;
    gLCDControlBuffer.wincnt.win0_enableObj = 1;

    gLCDControlBuffer.wincnt.wout_enableBg0 = 1;
    gLCDControlBuffer.wincnt.wout_enableBg1 = 1;
    gLCDControlBuffer.wincnt.wout_enableBg2 = 0;
    gLCDControlBuffer.wincnt.wout_enableBg3 = 1;
    gLCDControlBuffer.wincnt.wout_enableObj = 1;

    StartGreenText(proc);

    StartHelpPromptSprite(195, 147, 9, proc);

    InitText(PrepItemSuppyTexts.th + 0, 6);
    InitText(PrepItemSuppyTexts.th + 1, 5);

    InitText(PrepItemSuppyTexts.th + 15, 4);

    for (i = 0; i < UNIT_ITEM_COUNT; i++) {
        InitText(PrepItemSuppyTexts.th + 2 + i, 7);
    }

    for (i = 0; i < 8; i++) {
        InitTextDb(PrepItemSuppyTexts.th + 7 + i, 7);
    }

    StoreConvoyWeaponIconGraphics(0x4000, 6);

    sub_809D8D4(gBG0TilemapBuffer + 0x6F, 0x4000, 6);

    Decompress(gUnknown_08A19CCC, (void*)0x06015000);
    ApplyPalette(Pal_SpinningArrow, 0x14);

    StartMenuScrollBarExt(proc, 225, 47, 0x5800, 9);
    sub_8097668();
    SomethingPrepListRelated(proc->unit, proc->currentPage, 3);
    sub_809F5F4(proc);

    sub_809D300(
        PrepItemSuppyTexts.th + 7,
        gBG2TilemapBuffer + 0xF,
        (proc->yOffsetPerPage[proc->currentPage]) >> 4,
        proc->unit
    );

    BG_EnableSyncByMask(4);

    DrawPrepScreenItems(gBG0TilemapBuffer + 0x6F + 0xb3, PrepItemSuppyTexts.th + 2, proc->unit, 0);
    sub_809EBF0();

    StartUiSpinningArrows(proc);
    LoadUiSpinningArrowGfx(0, 0x280, 2);
    SetUiSpinningArrowPositions(0x78, 0x18, 0xe9, 0x18);
    SetUiSpinningArrowConfig(3);

    StartParallelWorker(List_PutHighlightedCategorySprites, proc);

    StartFace2(0, GetUnitPortraitId(proc->unit), 64, -4, FACE_DISP_KIND(FACE_96x80_FLIPPED) | FACE_DISP_HLAYER(FACE_HLAYER_3));

    str = GetStringFromIndex(proc->unit->pCharacterData->nameTextId);

    StartSysBrownBox(0xd, 0xe00, 0xf, 0xc00, 0x400, proc);

    EnableSysBrownBox(0, -40, -1, 1);
    EnableSysBrownBox(1, 0x98, 6, 2);

    SetBlendConfig(1, 0xe, 4, 0);
    SetBlendTargetA(0, 0, 0, 0, 0);
    SetBlendTargetB(0, 0, 0, 1, 0);

    PutDrawText(PrepItemSuppyTexts.th, gBG0TilemapBuffer, 0, GetStringTextCenteredPos(48, str), 0, str);

    PrepItemList_DrawCurrentOwnerText(proc);

    return;
}

LYN_REPLACE_CHECK(PutSubtitleHelpText);
void PutSubtitleHelpText(struct SubtitleHelpProc * proc, int y)
{
#ifdef CONFIG_EXTENDED_HELPBOXES
    static u16 lut[] = {
        0x00, 0x04, 0x08, 0x0C, 0x10, 0x14, 0x18, 0x1C,
        0x40, 0x44, 0x48, 0x4C, 0x50, 0x54, 0x58, 0x5C,
    };

#else
    static u16 lut[] = {
        0x00,
        0x04, 0x08, 0x0C, 0x10, 0x14, 0x18,
        0x44, 0x48, 0x4C, 0x50, 0x54, 0x58,
    };
#endif

    int i;

    for (i = 0; i < 9; i++) {
        int x = (i * 32) - 32 + proc->textOffset;
        int index = (proc->textNum + i) % proc->textCount;

#ifdef CONFIG_EXTENDED_HELPBOXES
    /* Delete this when this area is fixed, placeholder so the variables in the if statement don't go unused */
    if (index + x + lut[index] == 0)
        return;
    else
        return;
        /* Turning it off for now as the suggested new area 42C0 is already used/corrupted */
        // PutSprite(2, x, y, gObject_32x16, 0x42C0 + lut[index]);
#else
        PutSprite(2, x, y, gObject_32x16, 0x4240 + lut[index]);
#endif
    }

    return;
}

LYN_REPLACE_CHECK(TradeMenu_InitUnitNameDisplay);
void TradeMenu_InitUnitNameDisplay(struct TradeMenuProc * proc)
{
    char* str;
    int xStart;

    // TODO: constants
#ifdef CONFIG_EXTENDED_HELPBOXES
    /* Maybe change 0x5800 to 0x7080? It doesn't seem clear */
    /* Keeping it at vanilla 0x4800 fpr now because the brown name tile screws up no matter what */
    StartSysBrownBox(6, 0x4800, 0x08, 0x800, 0x400, (struct Proc *) (proc));
#else
    StartSysBrownBox(6, 0x4800, 0x08, 0x800, 0x400, (struct Proc *) (proc));
#endif

    EnableSysBrownBox(0, -40, -1, 1);
    EnableSysBrownBox(1, 184, -1, 0);

    // TODO: special effect constants
    SetBlendConfig(1, 12, 6, 0);

    // TODO: name functions
    SetBlendTargetA(FALSE, FALSE, FALSE, FALSE, FALSE);
    SetBlendTargetB(TRUE,  TRUE,  TRUE,  TRUE,  TRUE);

    // TODO: text color constants

    str = GetStringFromIndex(UNIT_NAME_ID(proc->units[0]));
    xStart = ((8 * UNIT_PANEL_WIDTH) - GetStringTextLen(str)) / 2;

    PutDrawText(NULL, gBG0TilemapBuffer + TILEMAP_INDEX(0, 0), 0, xStart, UNIT_PANEL_WIDTH, str);

    str = GetStringFromIndex(UNIT_NAME_ID(proc->units[1]));
    xStart = ((8 * UNIT_PANEL_WIDTH) - GetStringTextLen(str)) / 2;

    PutDrawText(NULL, gBG0TilemapBuffer + TILEMAP_INDEX(24, 0), 0, xStart, UNIT_PANEL_WIDTH, str);

    BG_EnableSyncByMask(BG0_SYNC_BIT);
}

//! FE8U = 0x0809D300 (DisplayConvoyItemStrings?)
LYN_REPLACE_CHECK(sub_809D300);
void sub_809D300(struct Text * textBase, u16 * tm, int yLines, struct Unit * unit)
{
    int i;

    TileMap_FillRect(tm, 12, 31, 0);

    if (gUnknown_02012F56 == 0) {
        ClearText(textBase);
        Text_InsertDrawString(textBase, 0, 1, GetStringFromIndex(0x5a8)); // TODO: msgid "Nothing[.]"
        PutText(textBase, tm + 3);
        return;
    }

    for (i = yLines; (i < yLines + 7) && (i < gUnknown_02012F56); i++) {
        struct Text* th = textBase + (i & 7);
        int item = gPrepScreenItemList[i].item;
        int unusable = !IsItemDisplayUsable(unit, item);

        ClearText(th);

        Text_InsertDrawString(
            th,
            0,
            unusable,
            GetItemName(item)
        );

        DrawIcon(tm + TILEMAP_INDEX(1, i*2 & 0x1f), GetItemIconId(item), 0x4000);

        PutText(th, tm + TILEMAP_INDEX(3, i*2 & 0x1f));

#ifndef CONFIG_INFINITE_DURABILITY
        PutNumberOrBlank(tm + TILEMAP_INDEX(12, i*2 & 0x1f), !unusable ? 2 : 1, GetItemUses(item));
#endif
    }

    return;
}


//! FE8U = 0x0809D47C
LYN_REPLACE_CHECK(sub_809D47C);
void sub_809D47C(struct Text * textBase, u16 * tm, int yLines, struct Unit * unit)
{
    if (gUnknown_02012F56 > yLines) {
        int y = (yLines * 2) & 0x1f;
        struct Text* th = textBase + (yLines & 7);
        int item = gPrepScreenItemList[yLines].item;
        int unusable = !IsItemDisplayUsable(unit, item);

        int offset = TILEMAP_INDEX(0, y);
        TileMap_FillRect(tm + offset, 12, 1, 0);

        ClearText(th);
        Text_InsertDrawString(th, 0, unusable, GetItemName(item));
        DrawIcon(tm + offset + 1, GetItemIconId(item), 0x4000);
        PutText(th, tm + offset + 3);

#ifndef CONFIG_INFINITE_DURABILITY
        PutNumberOrBlank(tm + offset + 12, !unusable ? 2 : 1,  GetItemUses(item));
#endif
    }
}

//! FE8U = 0x08099F7C
LYN_REPLACE_CHECK(sub_8099F7C);
void sub_8099F7C(struct Text* th, u16 * tm, struct Unit* unit, u16 flags) {
    int itemCount;
    int i;

    TileMap_FillRect(tm, 12, 20, 0);

    if ((flags & 2) != 0) {
        ResetIconGraphics();
    }

    if (!unit) {
        return;
    }

    itemCount = GetUnitItemCount(unit);

    for (i = 0; i < itemCount; th++, i++) {
        u16 item = unit->items[i];

        int isUnusable = ((flags & 4) != 0)
            ? !CanUnitUseItemPrepScreen(unit, item)
            : !IsItemDisplayUsable(unit, item);

        if ((flags & 1) == 0) {
            ClearText(th);
            Text_SetColor(th, isUnusable);
            Text_SetCursor(th, 0);
            Text_DrawString(th, GetItemName(item));
        }

        DrawIcon(tm + i * 0x40, GetItemIconId(item), 0x4000);

        PutText(th, tm + 2 + i * 0x40);

#ifndef CONFIG_INFINITE_DURABILITY
        PutNumberOrBlank(tm + 11 + i * 0x40, !isUnusable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemUses(item));
#endif
    }

    return;
}

//! FE8U = 0x0809B74C
LYN_REPLACE_CHECK(DrawPrepScreenItems);
void DrawPrepScreenItems(u16 * tm, struct Text* th, struct Unit* unit, u8 checkPrepUsability) {
    s8 isUsable;
    int i;
    int itemCount;

    TileMap_FillRect(tm, 11, 9, 0);

    itemCount = GetUnitItemCount(unit);

    for (i = 0; i < itemCount; i++) {
        int item = unit->items[i];

        if (checkPrepUsability != 0) {
            isUsable = CanUnitUseItemPrepScreen(unit, item);
        } else {
            isUsable = IsItemDisplayUsable(unit, item);
        }

        ClearText(th);
        PutDrawText(
            th,
            tm + i * 0x40 + 2,
            !isUsable ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE,
            0,
            0,
            GetItemName(item)
        );

#ifndef CONFIG_INFINITE_DURABILITY
        PutNumberOrBlank(tm + i * 0x40 + 0xB, isUsable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemUses(item));
#endif

        DrawIcon(tm + i * 0x40, GetItemIconId(item), 0x4000);

        th++;
    }

    return;
}

//! FE8U = 0x08099328 - PrepItemScreen_DisplayMenuOptions
LYN_REPLACE_CHECK(sub_8099328);
void sub_8099328(struct PrepItemScreenProc* proc, u16 * tm, struct Unit* unit) {

    /* It doesn't seem to save on shutdown (just suspend), so this is a test to give me something to work with for now */
#ifdef CONFIG_BEXP
    sBEXP[0] = 50;
#endif

    /* Funds textbox */
    TileMap_FillRect(tm, 10, 6, 0);

    ClearText(&gPrepItemTexts[25]);
    Text_InsertDrawString(&gPrepItemTexts[25], 0, PrepGetUnitAmount() < 2 ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x594)); // TODO: msgid "Trade"

#ifdef CONFIG_FORGE
    Text_InsertDrawString(&gPrepItemTexts[25], 32, PrepGetUnitAmount() < 2 ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE, "Forge"); // TODO: msgid "List"
#else
    Text_InsertDrawString(&gPrepItemTexts[25], 32, PrepGetUnitAmount() < 2 ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x595)); // TODO: msgid "List"
#endif

    PutText(&gPrepItemTexts[25], tm + TILEMAP_INDEX(0, 1));

    ClearText(&gPrepItemTexts[26]);
    Text_InsertDrawString(&gPrepItemTexts[26], 0, !CanUnitPrepScreenUse(unit) ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x596)); // TODO: msgid "Use"

#ifdef CONFIG_BEXP
    Text_InsertDrawString(&gPrepItemTexts[26], 32, (sBEXP[0] <= 0 ) ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_GOLD, "BEXP"); // TODO: msgid "BEXP"
#else
    Text_InsertDrawString(&gPrepItemTexts[26], 32, !proc->hasConvoyAccess ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x59A)); // TODO: msgid "Give all"
#endif

    PutText(&gPrepItemTexts[26], tm + TILEMAP_INDEX(0, 3));

    ClearText(&gPrepItemTexts[27]);
    Text_InsertDrawString(&gPrepItemTexts[27], 0, !proc->hasConvoyAccess ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x598)); // TODO: msgid "Supply"

    if (gGMData.state.bits.state_0) {
        struct Text* th = &gPrepItemTexts[27];
        int color = TEXT_COLOR_SYSTEM_WHITE;
        if ((!proc->hasConvoyAccess) || (GetUnitItemCount(unit) < 1) || CheckInLinkArena()) {
            color = TEXT_COLOR_SYSTEM_GRAY;
        }
        Text_InsertDrawString(th, 32, color, GetStringFromIndex(0x597)); // TODO: msgid "Sell"
    } else {
        if (CheckInLinkArena()) {
            Text_InsertDrawString(&gPrepItemTexts[27], 32, TEXT_COLOR_SYSTEM_GRAY, GetStringFromIndex(0x599)); // TODO: msgid "Armory"
        } else {
            Text_InsertDrawString(&gPrepItemTexts[27], 32, TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x599)); // TODO: msgid "Armory"
        }
    }

    PutText(&gPrepItemTexts[27], tm + TILEMAP_INDEX(0, 5));
    return;
}

//! FE8U = 0x080996E8 - PrepItemScreen_MenuOptionProcs
LYN_REPLACE_CHECK(sub_80996E8);
void sub_80996E8(struct PrepItemScreenProc* proc) {
    int previous = proc->popupPromptIdx;

    if (proc->unk_2d == 0xff) {
        if (gKeyStatusPtr->newKeys & R_BUTTON) {
            proc->unk_2d = proc->popupPromptIdx;

            if ((gGMData.state.bits.state_0)) {
                StartHelpBox(
                    (proc->popupPromptIdx & 1) * 32 + 136,
                    (proc->popupPromptIdx >> 1) * 16 + 84,
                    gHelpTextIds_GMapItemScreen[proc->popupPromptIdx]
                );
                return;
            } else {
                StartHelpBox(
                    (proc->popupPromptIdx & 1) * 32 + 136,
                    (proc->popupPromptIdx >> 1) * 16 + 84,
                    gHelpTextIds_PrepItemScreen[proc->popupPromptIdx]
                );
                return;
            }
        }

        if (gKeyStatusPtr->newKeys & A_BUTTON) {
            switch (proc->popupPromptIdx) {
                case 0:
                    if (PrepGetUnitAmount() < 2) {
                        PlaySoundEffect(0x6c);
                    } else {
                        Proc_Goto(proc, 4);
                        PlaySoundEffect(0x6a);
                    }

                    break;

                case 1:
                    if (PrepGetUnitAmount() < 2) {
                        PlaySoundEffect(0x6c);
                    } else {
                        Proc_Goto(proc, 8);
                        PlaySoundEffect(0x6a);
                    }

                    break;

                case 2:
                    if (!CanUnitPrepScreenUse(GetUnitFromPrepList(proc->selectedUnitIdx))) {
                        PlaySoundEffect(0x6c);
                    } else {
                        Proc_Goto(proc, 9);
                        PlaySoundEffect(0x6a);
                    }

                    break;

                case 3:
                    if (CheckInLinkArena() && (!(GetUnitFromPrepList(proc->selectedUnitIdx)->state & US_NOT_DEPLOYED))) {
                        StartPrepErrorHelpbox(-1, -1, 0x088B, proc);
                        return;
                    }

#ifdef CONFIG_BEXP
                    if (sBEXP[0] > 0) {
                            Proc_Goto(proc, 14);
                            PlaySoundEffect(0x6a);
                            return;
                        }
#else

                    if (proc->hasConvoyAccess) {
                        if (PrepItemScreen_GiveAll(GetUnitFromPrepList(proc->selectedUnitIdx)) != 0) {
                            sub_8099328(proc, TILEMAP_LOCATED(gBG0TilemapBuffer, 19, 9), GetUnitFromPrepList(proc->selectedUnitIdx));
                            sub_8099F7C(&gPrepItemTexts[15], TILEMAP_LOCATED(gBG0TilemapBuffer, 2, 9), GetUnitFromPrepList(proc->selectedUnitIdx), 0);

                            BG_EnableSyncByMask(1);

                            PlaySoundEffect(0x6a);
                            return;
                        }
                    }
#endif

                    PlaySoundEffect(0x6c);
                    break;

                case 4:
                    if (!proc->hasConvoyAccess) {
                        PlaySoundEffect(0x6c);
                    } else {
                        Proc_Goto(proc, 10);
                        PlaySoundEffect(0x6a);
                    }

                    break;

                case 5:
                    if ((gGMData.state.bits.state_0) != 0) {
                        if (proc->hasConvoyAccess) {
                            int itemCount = GetUnitItemCount(GetUnitFromPrepList(proc->selectedUnitIdx));
                            if ((itemCount > 0) && (!CheckInLinkArena())) {
                                Proc_Goto(proc, 11);
                                PlaySoundEffect(0x6a);
                                return;
                            }
                        }
                    } else {
                        if (!CheckInLinkArena()) {
                            Proc_Goto(proc, 12);
                            PlaySoundEffect(0x6a);
                            return;
                        }
                    }

                    PlaySoundEffect(0x6c);
                    break;

                default:
                    PlaySoundEffect(0x6c);
                    break;
            }

            return;
        }

        if (gKeyStatusPtr->newKeys & B_BUTTON) {
            proc->hoverUnitIdx = proc->selectedUnitIdx;
            proc->selectedUnitIdx = 0xff;
            sub_80ACA84(0);
            PlaySoundEffect(0x6b);
            Proc_Goto(proc, 0);
            return;
        }
    } else if (gKeyStatusPtr->newKeys & (B_BUTTON | R_BUTTON)) {
        CloseHelpBox();
        proc->unk_2d = 0xff;
    }

    if (gKeyStatusPtr->repeatedKeys & DPAD_LEFT) {
        if ((proc->popupPromptIdx & 1) != 0) {
            proc->popupPromptIdx--;
        } else if (gKeyStatusPtr->newKeys & DPAD_LEFT) {
            proc->popupPromptIdx++;
        }
    }

    if (gKeyStatusPtr->repeatedKeys & DPAD_RIGHT) {
        if ((proc->popupPromptIdx & 1) == 0) {
            proc->popupPromptIdx++;
        } else if (gKeyStatusPtr->newKeys & DPAD_RIGHT) {
            proc->popupPromptIdx--;
        }
    }

    if (gKeyStatusPtr->repeatedKeys & DPAD_UP) {
        if (proc->popupPromptIdx >= 2) {
            proc->popupPromptIdx -= 2;
        } else if (gKeyStatusPtr->newKeys & DPAD_UP) {
            proc->popupPromptIdx += 4;
        }
    }

    if (gKeyStatusPtr->repeatedKeys & DPAD_DOWN) {
        if (proc->popupPromptIdx < 4) {
            proc->popupPromptIdx += 2;
        } else if (gKeyStatusPtr->newKeys & DPAD_DOWN) {
            proc->popupPromptIdx -= 4;
        }
    }

    if (previous == proc->popupPromptIdx) {
        return;
    }

    PlaySoundEffect(0x65);
    ShowSysHandCursor(
        (proc->popupPromptIdx & 1) * 32 + 144,
        (proc->popupPromptIdx >> 1) * 16 + 84,
        3,
        0x400
    );

    if (proc->unk_2d == 0xff) {
        return;
    }

    if (gGMData.state.bits.state_0) {
        StartHelpBox(
            (proc->popupPromptIdx & 1) * 38 + 144,
            (proc->popupPromptIdx >> 1) * 16 + 84,
            gHelpTextIds_GMapItemScreen[proc->popupPromptIdx]
        );
    } else {
        StartHelpBox(
            (proc->popupPromptIdx & 1) * 38 + 144,
            (proc->popupPromptIdx >> 1) * 16 + 84,
            gHelpTextIds_PrepItemScreen[proc->popupPromptIdx]
        );
    }

    return;
}

//! FE8U = 0x080995D4
LYN_REPLACE_CHECK(sub_80995D4);
void sub_80995D4(struct PrepItemScreenProc* proc) {
    s8 unk;

    TileMap_FillRect(TILEMAP_LOCATED(gBG0TilemapBuffer, 15, 9), 12, 20, 0);

    PutImg_PrepItemUseUnk(0x6000, 5);
    PutImg_PrepPopupWindow(0x800, 10);
    sub_80994C4(proc);

    unk = sub_809A21C(
        ((proc->selectedUnitIdx % 3) * 64) + 20,
        ((proc->selectedUnitIdx / 3) * 16) + 4 - proc->unk_34
    );

    if (unk != 0) {
        BlockUiCursorHand();
    } else {
        UnblockUiCursorHand();
    }

    sub_8098BE8();

    BG_EnableSyncByMask(1);

    return;
}

//! FE8U = 0x08099654
LYN_REPLACE_CHECK(sub_8099654);
void sub_8099654(struct PrepItemScreenProc* proc) {
    /* Display prep menu light brown box options */
    /* Shift the x direction of the text for the options for BEXP using the second parameter of TILEMAP_LOCATED */
#ifdef CONFIG_BEXP
    sub_8099328(proc, TILEMAP_LOCATED(gBG0TilemapBuffer, 19, 9), GetUnitFromPrepList(proc->selectedUnitIdx));
#else
    sub_8099328(proc, TILEMAP_LOCATED(gBG0TilemapBuffer, 19, 9), GetUnitFromPrepList(proc->selectedUnitIdx));
#endif
    /* Display lightbox brown box after selecting unit */
    StartParallelWorker(sub_8098BC8, proc);
    StartHelpPromptSprite(120, 140, 9, proc);

    ShowSysHandCursor(
        (proc->popupPromptIdx & 1) * 32 + 144,
        (proc->popupPromptIdx >> 1) * 16 + 84,
        3,
        0x400
    );

    BG_EnableSyncByMask(1);

    return;
}

// void WmBEXP_Setup(struct WmSellProc* proc) {
//     int i;

//     gLCDControlBuffer.dispcnt.mode = 0;

//     SetupBackgrounds(0);

//     BG_Fill(BG_GetMapBuffer(0), 0);
//     BG_Fill(BG_GetMapBuffer(1), 0);
//     BG_Fill(BG_GetMapBuffer(2), 0);

//     gLCDControlBuffer.bg0cnt.priority = 0;
//     gLCDControlBuffer.bg1cnt.priority = 2;
//     gLCDControlBuffer.bg2cnt.priority = 0;
//     gLCDControlBuffer.bg3cnt.priority = 3;

//     ResetFaces();

//     ResetText();
//     ResetIconGraphics_();
//     LoadUiFrameGraphics();
//     LoadObjUIGfx();

//     BG_SetPosition(0, 0, 0);
//     BG_SetPosition(1, 0, 0);
//     BG_SetPosition(2, 0, 0);

//     LoadHelpBoxGfx((void*)0x06014000, -1);
//     LoadIconPalettes(4);

//     RestartMuralBackground();

//     PutImg_PrepItemUseUnk(0x5000, 5);
//     PutImg_PrepPopupWindow(0x800, 8);

//     Decompress(gUnknown_08A1BBD0, gGenericBuffer);
//     CallARM_FillTileRect(gBG1TilemapBuffer, gGenericBuffer, 0x1000);

//     BG_EnableSyncByMask(7);

//     StartFace2(
//         0,
//         GetUnitPortraitId(proc->unit),
//         68,
//         72,
//         FACE_DISP_KIND(FACE_96x80_FLIPPED) | FACE_DISP_HLAYER(FACE_HLAYER_2) | FACE_DISP_BLEND
//     );
//     StartUiCursorHand(proc);
//     ResetSysHandCursor(proc);
//     DisplaySysHandCursorTextShadow(0x600, 1);

//     gLCDControlBuffer.dispcnt.win0_on = 1;
//     gLCDControlBuffer.dispcnt.win1_on = 0;
//     gLCDControlBuffer.dispcnt.objWin_on = 0;

//     gLCDControlBuffer.win0_left = 128;
//     gLCDControlBuffer.win0_top = 40;
//     gLCDControlBuffer.win0_right = 224;
//     gLCDControlBuffer.win0_bottom = 152;

//     gLCDControlBuffer.wincnt.win0_enableBg0 = 1;
//     gLCDControlBuffer.wincnt.win0_enableBg1 = 1;
//     gLCDControlBuffer.wincnt.win0_enableBg2 = 1;
//     gLCDControlBuffer.wincnt.win0_enableBg3 = 1;
//     gLCDControlBuffer.wincnt.win0_enableObj = 1;

//     gLCDControlBuffer.wincnt.wout_enableBg0 = 1;
//     gLCDControlBuffer.wincnt.wout_enableBg1 = 1;
//     gLCDControlBuffer.wincnt.wout_enableBg2 = 0;
//     gLCDControlBuffer.wincnt.wout_enableBg3 = 1;
//     gLCDControlBuffer.wincnt.wout_enableObj = 1;

//     gLCDControlBuffer.wincnt.win0_enableBlend = 1;
//     gLCDControlBuffer.wincnt.wout_enableBlend = 1;

//     SetBlendConfig(0, 8, 8, 8);

//     StartGreenText((struct Proc*)proc);

//     StartHelpPromptSprite(120, 140, 2, (struct Proc*)proc);
//     StartDrawPrepFundsSprite(165, 128, 10, proc);
//     ShowPrepFundsSpriteAt(165, 128);

//     InitText(&_PrepItemSuppyTexts->textA, 4);
//     InitText(&_PrepItemSuppyTexts->textB, 2);

//     sub_809FE68();

//     for (i = 0; i < 5; i++) {
//         InitText(&_PrepItemSuppyTexts->textArray[i], 7);
//     }

//     SetPrimaryHBlankHandler(0);
//     SetPrimaryHBlankHandler(sub_809FE1C);

//     BG_EnableSyncByMask(4);

//     DrawPrepScreenItems(gBG0TilemapBuffer + 0x122, &_PrepItemSuppyTexts->textArray[0], proc->unit, 0);
//     WmSell_PutSupplyFaceAndText();

//     StartParallelWorker(WmSell_DrawValueSpriteText, proc);

//     WmSell_DrawItemGoldValue(proc->unit->items[proc->unk_30]);
//     WmSell_DrawPartyFunds();

//     return;
// }

LYN_REPLACE_CHECK(UnitRescue);
void UnitRescue(struct Unit* actor, struct Unit* target) {

#if defined(SID_DangerRanger) && (COMMON_SKILL_VALID(SID_DangerRanger))
        if (SkillTester(actor, SID_DangerRanger))
            gActionDataExpa.refrain_action = true;
#endif

    actor->state  |= US_RESCUING;
    target->state |= US_RESCUED | US_HIDDEN;

    actor->rescue = target->index;
    target->rescue = actor->index;

    target->xPos = actor->xPos;
    target->yPos = actor->yPos;
}
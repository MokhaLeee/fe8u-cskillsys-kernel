#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "jester_headers/macros.h"
#include "jester_headers/soundtrack-ids.h"
#include "jester_headers/maps.h"
#include "jester_headers/flags.h"

/**
 * Ally unit and REDA definitions
 */
static const struct REDA REDAs_Seth[] = { 
    { .x = 9, .y = 5, .b = -1, .delayFrames = 0, },
    { .x = 6, .y = 5, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_Franz[] = {
    { .x = 9, .y = 4, .b = -1, .delayFrames = 16, },
    { .x = 6, .y = 4, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_Eirika[] = {
    { .x = 7, .y = 5, .a = 2, .b = -1, .delayFrames = 16, },
};

static const struct REDA REDAs_Seth_Prologue[] = {
    { .x = 13, .y = 8, .b = -1, .delayFrames = 0, },
    { .x = 10, .y = 8, .b = -1, .delayFrames = 0, },
    { .x = 10, .y = 7, .b = -1, .delayFrames = 0, },
    { .x = 9,  .y = 7, .b = -1, .delayFrames = 0, },
    { .x = 9,  .y = 5, .b = -1, .delayFrames = 0, },
    { .x = 8,  .y = 5, .b = -1, .delayFrames = 0, }
};

static const struct REDA REDAs_Eirika_Prologue[] = {
    { .x = 9, .y = 5, .a = 2, .b = -1, .delayFrames = 0, }
};

static const struct REDA REDAs_Soldier[] = {
    { .x = 9,  .y = 11, .b = -1, .delayFrames = 0, },
    { .x = 13, .y = 11, .b = -1, .delayFrames = 0, },
    { .x = 13, .y = 6,  .b = -1, .delayFrames = 0, }
};

static const struct UnitDefinition Renais_Castle_Allies[] = {
    UNIT_ENTRY(CHARACTER_FADO, 0x73, FACTION_ID_BLUE, true, 1, 13, 3, 0, 0, 0, 0, 0, 0),
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE, true, 1, 14, 4, 0, 0, 0, 0, 0, 0),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE, true, 1, 15, 4, 0, 0, 0, 0, 0, 0),
    UNIT_ENTRY(CHARACTER_FRANZ, CLASS_CAVALIER, FACTION_ID_BLUE, true, 1, 11, 4, 0, 0, 0, 0, 0, 0),
    UNIT_ENTRY(CHARACTER_MOULDER, CLASS_GENERAL, FACTION_ID_BLUE, true, 1, 11, 7, 0, 0, 0, 0, 0, 0),
    UNIT_ENTRY(CHARACTER_VANESSA, CLASS_GENERAL, FACTION_ID_BLUE, true, 1, 15, 7, 0, 0, 0, 0, 0, 0),
    UNIT_ENTRY(CHARACTER_GILLIAM, CLASS_ARMOR_KNIGHT, FACTION_ID_BLUE, true, 1, 7, 14, 0, 0, 0, 0, 0, 0),
    UNIT_ENTRY(CHARACTER_TANA, CLASS_ARMOR_KNIGHT, FACTION_ID_BLUE, true, 1, 10, 14, 0, 0, 0, 0, 0, 0),
    {}
};

static const struct UnitDefinition Panicked_Soldier_Allies[] = {
    UNIT_ENTRY(CHARACTER_EPHRAIM, CLASS_SOLDIER, FACTION_ID_BLUE, true, 1, 9, 15, 3, REDAs_Soldier, 0, 0, 0, 0),
    {}
};

static const struct UnitDefinition Renais_Runaways_Allies[] = {
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE, true, 1, 9, 3, 2, REDAs_Seth, 0, 0, 0, 0, ITEM_LANCE_STEEL),
    UNIT_ENTRY(CHARACTER_FRANZ, CLASS_CAVALIER, FACTION_ID_BLUE, true, 1, 9, 3, 2, REDAs_Franz, 0, 0, 0, 0),
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE, true, 1, 6, 5, 1, REDAs_Eirika, 0, 0, 0, 0),
    {}
};


static const struct UnitDefinition Eirika_Seth_Allies[] = {
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE, true, 1, 13, 9, 6, REDAs_Seth_Prologue, 0, 0, 0, 0, ITEM_LANCE_STEEL),
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE, true, 1, 8, 5, 1, REDAs_Eirika_Prologue, 0, 0, 0, 0),
    {}
};

/**
 * Enemy unit and REDA definitions
 */

static const struct REDA REDAs_Druid1_Enemy[] = {
    { .x = 7, .y = 4, .b = -1, }
};
static const struct REDA REDAs_Druid2_Enemy[] = { 
    { .x = 6, .y = 6, .b = -1, } 
};
static const struct REDA REDAs_Druid3_Enemy[] = { 
    { .x = 19, .y = 3, .b = -1, } 
};
static const struct REDA REDAs_Druid4_Enemy[] = { 
    { .x = 20, .y = 6, .b = -1, } 
};
static const struct REDA REDAs_GreatKnight1_Enemy[] = {
    { .x = 10, .y = 4,  .b = -1, }
};
static const struct REDA REDAs_GreatKnight2_Enemy[] = {
    { .x = 16, .y = 4, .b = -1,  }
};
static const struct REDA REDAs_Paladin1_Enemy[] = {
    { .x = 10, .y = 6, .b = -1, }
};
static const struct REDA REDAs_Paladin2_Enemy[] = {
    { .x = 16, .y = 6, .b = -1, }
};
static const struct REDA REDAs_MageKnight1_Enemy[] = {
    { .x = 10, .y = 8, .b = -1, }
};
static const struct REDA REDAs_MageKnight2_Enemy[] = {
    { .x = 16, .y = 8, .b = -1, }
};
static const struct REDA REDAs_Vigarde_Enemy[] = {
    { .x = 13, .y = 6, .b = -1, }
};
static const struct REDA REDAs_Lyon_Enemy[] = {
    { .x = 12, .y = 7, .b = -1, }
};
static const struct REDA REDAs_Valter_Enemy[] = {
    { .x = 10, .y = 5, .b = -1, },
    { .x = 9,  .y = 5, .b = 1, .delayFrames = 16, }
};
static const struct REDA REDAs_Wyvern1_Enemy[] = {
    { .x = 11, .y = 4, .b = -1, .delayFrames = 16, }
};
static const struct REDA REDAs_Wyvern2_Enemy[] = {
    { .x = 11, .y = 6, .b = -1, .delayFrames = 16, }
};
static const struct REDA REDAs_ONeill_Enemy[] = {
    { .x = 10, .y = 8, .b = -1, .delayFrames = 0, }
};
static const struct REDA REDAs_Fighter1_Enemy[] = {
    { .x = 8, .y = 6, .b = -1, .delayFrames = 16, }
};
static const struct REDA REDAs_Fighter2_Enemy[] = {
    { .x = 9, .y = 6, .b = -1, .delayFrames = 16, }
};

static const struct UnitDefinition Renais_Castle_Enemy1[] = {
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_DRUID, FACTION_ID_RED, true, 4, 5, 1, 1, REDAs_Druid1_Enemy, 0, 4, 9, 0),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_DRUID, FACTION_ID_RED, true, 4, 5, 2, 1, REDAs_Druid2_Enemy, 0, 4, 9, 0),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_DRUID, FACTION_ID_RED, true, 4, 19, 10, 1, REDAs_Druid3_Enemy, 0, 4, 9, 0),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_DRUID, FACTION_ID_RED, true, 4, 20, 10, 1, REDAs_Druid4_Enemy, 0, 0, 0, 0),
    {}
};

static const struct UnitDefinition Renais_Castle_Enemy2[] = {
    UNIT_ENTRY(CHARACTER_SOLDIER_83, CLASS_GREAT_KNIGHT, FACTION_ID_RED, true, 4, 5, 1, 1, REDAs_GreatKnight1_Enemy, 0, 4, 9, 0),
    UNIT_ENTRY(CHARACTER_SOLDIER_83, CLASS_GREAT_KNIGHT, FACTION_ID_RED, true, 4, 5, 2, 1, REDAs_GreatKnight2_Enemy, 0, 4, 9, 0),
    UNIT_ENTRY(CHARACTER_SOLDIER_84, CLASS_PALADIN, FACTION_ID_RED, true, 4, 19, 10, 1, REDAs_Paladin1_Enemy, 0, 4, 9, 0),
    UNIT_ENTRY(CHARACTER_SOLDIER_84, CLASS_PALADIN, FACTION_ID_RED, true, 4, 20, 10, 1, REDAs_Paladin2_Enemy, 0, 4, 9, 0),
    UNIT_ENTRY(CHARACTER_SOLDIER_85, CLASS_MAGE_KNIGHT_F, FACTION_ID_RED, true, 4, 10, 10, 1, REDAs_MageKnight1_Enemy, 0, 4, 9, 0),
    UNIT_ENTRY(CHARACTER_SOLDIER_85, CLASS_MAGE_KNIGHT_F, FACTION_ID_RED, true, 4, 16, 10, 1, REDAs_MageKnight2_Enemy, 0, 4, 9, 0),
    {}
};

static const struct UnitDefinition Renais_Castle_Enemy3[] = {
    UNIT_ENTRY(CHARACTER_VIGARDE, CLASS_GENERAL, FACTION_ID_RED, true, 4, 13, 10, 1, REDAs_Vigarde_Enemy, 0, 4, 9, 0),
    UNIT_ENTRY(CHARACTER_LYON, CLASS_NECROMANCER, FACTION_ID_GREEN, true, 4, 12, 11, 1, REDAs_Lyon_Enemy, 0, 4, 9, 0),
    {}
};

static const struct UnitDefinition Renais_Outside_Castle_Enemy[] = {
    UNIT_ENTRY(CHARACTER_VALTER_PROLOGUE, CLASS_WYVERN_KNIGHT, FACTION_ID_RED, false, 13, 14, 5, 2, REDAs_Valter_Enemy, 0, 4, 9, 0, ITEM_LANCE_SILVER),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_WYVERN_RIDER, FACTION_ID_RED, true, 4, 14, 4, 1, REDAs_Wyvern1_Enemy, 0, 4, 9, 0),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_WYVERN_RIDER, FACTION_ID_RED, true, 4, 14, 6, 1, REDAs_Wyvern2_Enemy, 0, 4, 9, 0),
    {}
};

static const struct UnitDefinition Grado_Reinforcements_Enemy[] = {
    UNIT_ENTRY(CHARACTER_ONEILL, CLASS_FIGHTER, FACTION_ID_RED, false, 4, 14, 8, 1, REDAs_ONeill_Enemy, 6, 3, 0, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(0x82, CLASS_FIGHTER, FACTION_ID_RED, false, 1, 14, 7, 1, REDAs_Fighter1_Enemy, 0, 10, 0, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, false, 2, 14, 7, 1, REDAs_Fighter2_Enemy, 0, 12, 2, 0, ITEM_AXE_IRON),
    {}
};

/**
 * Main events
 */
static const EventScr EventScr_Beginning[] = {
    LOAD_WAIT(Renais_Castle_Allies) // Load units and move them if possible
    LOAD_MAP_XY(17, 12, CHAPTER_16) // Load coordinates and the map for this chapter
    FADE_IN_SCREEN(16) // Fade in from black in 1 second
    MUSC(BGM_RAID) // Set BGM to RAID
    BROWNBOXTEXT(BB_Renais_Castle, 2, 2) // Set location text pop up
    LOAD_WAIT(Panicked_Soldier_Allies) // Load units and move them if possible
    CAMERA(17, 2) // Set camera to these coordinates
    HIGHLIGHT_CHARACTER(CHARACTER_EPHRAIM, 60) // Set flashing cursor on unit for a number of frames
    Text(Chapter_00_Scene_01_Convo_01) // Play this dialogue
    MOVE_WAIT(0, CHARACTER_EPHRAIM, 13, 11) // Move unit to chosen coordinates at given speed then wait
    ERASE(CHARACTER_EPHRAIM) // Erase given unit
    MOVE_1STEP_WAIT(0, CHARACTER_EIRIKA, MOVE_LEFT) // Move unit 1 step in given direction and wait
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60) // Set flashing cursor on unit for a number of frames
    Text(Chapter_00_Scene_01_Convo_02)
    MOVEONTO_WAIT(0, CHARACTER_SETH, CHARACTER_EIRIKA) // Move unit to another unit and wait    
    ERASE(CHARACTER_EIRIKA) // Erase given unit
    HIGHLIGHT_CHARACTER(CHARACTER_SETH, 60) // Set flashing cursor on unit for a number of frames
    Text(Chapter_00_Scene_01_Convo_03)
    MOVE(0, CHARACTER_SETH, 13, 11) // Move unit to chosen coordinates at given speed then wait
    MOVE_TWICE_WAIT(0, CHARACTER_FRANZ, 12, 4, 12, 11) // Move unit twice and wait
    ERASE(CHARACTER_SETH) // Erase given unit
    ERASE(CHARACTER_FRANZ) // Erase given unit
    MOVE(0, CHARACTER_MOULDER, 11, 4)
    MOVE(0, CHARACTER_VANESSA, 15, 4)
    ENUN // Wait for units to finish moving before continuing
    MOVE_1STEP(0, CHARACTER_MOULDER, MOVE_RIGHT)
    MOVE_1STEP(0, CHARACTER_VANESSA, MOVE_LEFT)
    ENUN // Wait for units to finish moving before continuing
    LOAD_WAIT(Renais_Castle_Enemy1) // Load units and move them if possible
    LOAD_WAIT(Renais_Castle_Enemy2) // Load units and move them if possible
    LOAD_WAIT(Renais_Castle_Enemy3) // Load units and move them if possible
    HIGHLIGHT_CHARACTER(CHARACTER_FADO, 60) // Set flashing cursor on unit for a number of frames
    Text(Chapter_00_Scene_01_Convo_04)
    FADE_OUT_SCREEN(2) // Fade out to black in 8 seconds
    REMA // Remove the current map
    SKIP_POINT
    CLEAR_ALL_UNITS

    // New scene
    LOAD_MAP_XY(0, 0, OUTSIDE_RENAIS)
    FADE_IN_SCREEN(16)
    LOAD_WAIT(Renais_Runaways_Allies)
    HIGHLIGHT_CHARACTER(CHARACTER_SETH, 60)
    Text_BG(0x1D, Chapter_00_Scene_02_Convo_01)
    MOVE_THRICE_WAIT(0, CHARACTER_FRANZ, 4, 4, 4, 2, 0, 2)
    ERASE(CHARACTER_FRANZ)
    HIGHLIGHT_CHARACTER(CHARACTER_SETH, 60)
    Text_BG(0x1D, Chapter_00_Scene_02_Convo_02)
    LOAD_WAIT(Renais_Outside_Castle_Enemy)
    MOVE_1STEP_WAIT(0, CHARACTER_SETH, MOVE_RIGHT)
    MOVE_1STEP_WAIT(0, CHARACTER_EIRIKA, MOVE_LEFT)
    HIGHLIGHT_CHARACTER(CHARACTER_VALTER_PROLOGUE, 60)
    Text_BG(0x1D, Chapter_00_Scene_02_Convo_03)
    MOVE_1STEP_WAIT(0, CHARACTER_VALTER_PROLOGUE, MOVE_LEFT)
    // Battle scene
    StartBattle
    MissedAttack(0, 0)
    NormalDamage(1, 0)
    EndAttack
    FIGHT(CHARACTER_SETH, CHARACTER_VALTER_PROLOGUE, 0, false)
    //
    HIGHLIGHT_CHARACTER(CHARACTER_SETH, 60)
    Text(Chapter_00_Scene_02_Convo_04)
    MOVE_1STEP_WAIT(0, CHARACTER_SETH, MOVE_LEFT)
    ERASE(CHARACTER_EIRIKA)
    MOVE_THRICE_WAIT(0, CHARACTER_SETH, 4, 4, 4, 2, 0, 2)
    ERASE(CHARACTER_SETH)
    HIGHLIGHT_CHARACTER(CHARACTER_VALTER_PROLOGUE, 60)
    Text(Chapter_00_Scene_02_Convo_05)
    FADE_OUT_SCREEN(16)
    REMA
    SKIP_POINT
    CLEAR_ALL_UNITS

    // New scene
    LOAD_MAP_XY(0, 0, PROLOGUE)
    FADE_IN_SCREEN(16)
    LOAD_WAIT_PERSIST(Eirika_Seth_Allies)
    SET_UNIT_HP(CHARACTER_SETH, 10)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    REDUCE_VOLUME
    Text_BG(0x1D, Chapter_00_Scene_03_Convo_01)
    MOVE_WAIT(0, CHARACTER_SETH, 4, 4)
    HIGHLIGHT_CHARACTER(CHARACTER_SETH, 60)
    Text(Chapter_00_Scene_03_Convo_02)
    MOVE_WAIT(0, CHARACTER_EIRIKA, 4, 5)
    HIGHLIGHT_CHARACTER(CHARACTER_SETH, 60)
    Text(Chapter_00_Scene_03_Convo_03)
    GIVE_ITEM_TO(ITEM_SWORD_RAPIER, CHARACTER_EIRIKA)
    LOAD_WAIT(Grado_Reinforcements_Enemy)
    HIGHLIGHT_CHARACTER(CHARACTER_ONEILL, 60)
    Text(Chapter_00_Scene_03_Convo_04)
    ENDA
};

static const EventScr EventScr_Ending[] = {
    MUSC(BGM_VICTORY)
    Text_BG(0x1D, Chapter_00_Scene_03_Convo_06)
    FADE_OUT_SCREEN(16)
    NEXT_CHAPTER
    REMA
    SET_FLAG(GUIDE_SUSPEND)
    SET_FLAG(GUIDE_SAVE)
    SET_FLAG(GUIDE_MOVEMENT_RANGE)
    SET_FLAG(GUIDE_VIEWING_UNITS)
    SET_FLAG(GUIDE_GAME_FLOW)
    SET_FLAG(GUIDE_DEFEAT_A_BOSS)
    SET_FLAG(GUIDE_ATTACK_RANGE)
    SET_FLAG(GUIDE_STRATEGIC_BATTLE_INFO)
    SET_FLAG(GUIDE_DETAILED_BATTLE_INFO)
    SET_FLAG(GUIDE_LEVELING_UP)
    SET_FLAG(GUIDE_WEAPON_DURABILITY)
    ENDA
};

/**
 * Misc events
 */
static const EventListScr EventScr_Talk_EirikaSeth[] = {
    //ConvoEvent(0xB32)
};

/**
 * Event list
 */
static const EventListScr EventListScr_Turn[] = {
    END_MAIN
};

static const EventListScr EventListScr_OneEnemyLeft[] = {
    CHECK_ENEMIES
    SVAL(EVT_SLOT_7, 1)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    Text(Chapter_00_Scene_03_Convo_05)
    GOTO(0x1)

// Preserves the flag if the condition isn't met
LABEL(0x0)
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF_SLOT2

LABEL(0x1)
    NoFade
    ENDA
};

static const EventListScr EventListScr_Character[] = {
    CharacterEventBothWays(EVFLAG_TMP(7), EventScr_Talk_EirikaSeth, CHARACTER_EIRIKA, CHARACTER_SETH)
    END_MAIN
};

static const EventListScr EventListScr_Location[] = {
    END_MAIN
};

static const EventListScr EventListScr_Misc[] = {
    DefeatAll(EventScr_Ending)
    AFEV(EVFLAG_TMP(7), EventListScr_OneEnemyLeft, 0)
    CauseGameOverIfLordDies
    END_MAIN
};

static const EventListScr EventListScr_SelectUnit[] = {
    END_MAIN
};

static const EventListScr EventListScr_SelectDestination[] = {
    END_MAIN
};

static const EventListScr EventListScr_UnitMove[] = {
    END_MAIN
};

static void const * const EventListScr_Tutorial[] = {
    NULL
};

static const u8 TrapData_ThisEvent[] = {
    TRAP_NONE
};

static const u8 TrapData_ThisEventHard[] = {
    TRAP_NONE
};

const struct ChapterEventGroup Chapter00Event = {
    .turnBasedEvents               = EventListScr_Turn,
    .characterBasedEvents          = EventListScr_Character,
    .locationBasedEvents           = EventListScr_Location,
    .miscBasedEvents               = EventListScr_Misc,
    .specialEventsWhenUnitSelected = EventListScr_SelectUnit,
    .specialEventsWhenDestSelected = EventListScr_SelectDestination,
    .specialEventsAfterUnitMoved   = EventListScr_UnitMove,
    .tutorialEvents                = EventListScr_Tutorial,

    .traps            = TrapData_ThisEvent,
    .extraTrapsInHard = TrapData_ThisEventHard,

    .playerUnitsChoice1InEncounter = NULL,
    .playerUnitsChoice2InEncounter = NULL,
    .playerUnitsChoice3InEncounter = NULL,

    .enemyUnitsChoice1InEncounter = NULL,
    .enemyUnitsChoice2InEncounter = NULL,
    .enemyUnitsChoice3InEncounter = NULL,

    .beginningSceneEvents = EventScr_Beginning,
    .endingSceneEvents    = EventScr_Ending,
};

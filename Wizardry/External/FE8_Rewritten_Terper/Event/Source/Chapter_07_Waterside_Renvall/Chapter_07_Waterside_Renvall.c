#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "jester_headers/macros.h"
#include "jester_headers/soundtrack-ids.h"
#include "jester_headers/maps.h"
#include "jester_headers/flags.h"
#include "jester_headers/miscellaenous.h"
#include "EAstdlib.h"

static const struct REDA REDAs_NULL[] = {
    { } 
};

/**
 * Ally unit and REDA definitions
 */

static const struct REDA REDAs_CHARACTER_1[] = {
    { .x = 2, .y = 17, .b = -1, .delayFrames = 0, },
    {},
};
static const struct REDA REDAs_CHARACTER_2[] = {
    { .x = 5, .y = 16, .b = -1, .delayFrames = 0, },
    {},
};
static const struct REDA REDAs_CHARACTER_3[] = {
    { .x = 4, .y = 17, .b = -1, .delayFrames = 0, },
    {},
};
static const struct REDA REDAs_CHARACTER_4[] = {
    { .x = 3, .y = 16, .b = -1, .delayFrames = 0, },
    {},
};
static const struct REDA REDAs_CHARACTER_5[] = {
    { .x = 3, .y = 18, .b = -1, .delayFrames = 0, },
    {},
};
static const struct REDA REDAs_CHARACTER_6[] = {
    { .x = 2, .y = 16, .b = -1, .delayFrames = 0, },
    {},
};
static const struct REDA REDAs_CHARACTER_7[] = {
    { .x = 2, .y = 19, .b = -1, .delayFrames = 0, },
    {},
};
static const struct REDA REDAs_CHARACTER_8[] = {
    { .x = 1, .y = 18, .b = -1, .delayFrames = 0, },
    {},
};

static const struct UnitDefinition CH7_PLAYER_UNITS[] = {
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 0, 17, 1, REDAs_CHARACTER_1, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_NATASHA, CLASS_CLERIC, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 0, 16, 1, REDAs_CHARACTER_2, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_COLM, CLASS_THIEF, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 0, 17, 1, REDAs_CHARACTER_3, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_LOCKPICK),
    UNIT_ENTRY(CHARACTER_TANA, CLASS_PEGASUS_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 0, 0, 16, 1, REDAs_CHARACTER_4, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_GILLIAM, CLASS_ARMOR_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 0, 18, 1, REDAs_CHARACTER_5, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_FRANZ, CLASS_CAVALIER, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 1, 16, 1, REDAs_CHARACTER_6, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_NEIMI, CLASS_ARCHER_F, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 0, 19, 1, REDAs_CHARACTER_7, 0, 0, 0, 0, ITEM_BOW_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 0, 18, 1, REDAs_CHARACTER_8, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_LANCE_SILVER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_JOSHUA, CLASS_MYRMIDON, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 4, 0, 17, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_KILLER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 0, 19, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_LANCE_SILVER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_JOSHUA, CLASS_MYRMIDON, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 4, 4, 15, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_KILLER, ITEM_VULNERARY),
    {},
};

static const struct REDA REDAs_EIRIKA_ARRIVES[] = {
    { .x = 3, .y = 4, .b = -1, .delayFrames = 0, },
    { .x = 8, .y = 4, .b = -1, .delayFrames = 0, },
    {},
};
static const struct REDA REDAs_SETH_ARRIVES[] = {
    { .x = 3, .y = 4, .b = -1, .delayFrames = 20, },
    { .x = 7, .y = 4, .b = -1, .delayFrames = 0, },
    {},
};

static const struct UnitDefinition CH7_EIRIKA_SETH[] = {
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 5, 3, 9, 2, REDAs_EIRIKA_ARRIVES, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 3, 9, 2, REDAs_SETH_ARRIVES, 0, 0, 0, 0, ITEM_SWORD_STEEL, ITEM_LANCE_SILVER),
    {},
};

static const struct REDA REDAs_ORSON[] = {
    { .x = 10, .y = 4, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH7_ORSON[] = {
    UNIT_ENTRY(CHARACTER_ORSON, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 5, 10, 0, 1, REDAs_ORSON, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    {}
};

// /**
//  * Enemy unit and REDA definitions
//  */

static const struct REDA REDAs_ARCHER_1[] = {
    { .x = 8, .y = 5, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_ARCHER_2[] = {
    { .x = 10, .y = 5, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH7_ENEMY_UNITS[] = {
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 17, 6, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_AXE_STEEL),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 8, 11, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_LANCE_IRON), 
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 9, 11, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 10, 17, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 16, 19, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_AXE_STEEL),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 2, 10, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 13, 13, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_MAGE, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 15, 14, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_ANIMA_FIRE),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 19, 8, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 18, 19, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_AXE_STEEL),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 10, 13, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_SWORD_IRON), 
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_MAGE, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 15, 10, 0, REDAs_NULL, 0, 6, 9, 0, ITEM_ANIMA_THUNDER, ITEM_BOOSTER_POW),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 5, 17, 8, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_BOW_STEEL),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 19, 21, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_BOW_STEEL),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 12, 18, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_LANCE_STEEL),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_MAGE, FACTION_ID_RED, ITEM_DROP, AUTOLEVEL, 7, 1, 8, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_ANIMA_THUNDER, ITEM_PUREWATER),
    UNIT_ENTRY(CHARACTER_MURRAY, CLASS_CAVALIER, FACTION_ID_RED, ITEM_DROP, NO_AUTOLEVEL, 12, 9, 4, 0, REDAs_NULL, 3, 3, 9, 20, ITEM_SWORD_STEEL, ITEM_LANCE_JAVELIN, ITEM_MASTERSEAL),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 9, 4, 1, REDAs_ARCHER_1, 0, 3, 9, 0, ITEM_BOW_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 9, 4, 1, REDAs_ARCHER_2, 0, 3, 9, 0, ITEM_BOW_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 17, 6, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_AXE_STEEL), 
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 2, 13, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_BLADE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARMOR_KNIGHT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 7, 17, 0, REDAs_NULL, 3, 3, 9, 0, ITEM_LANCE_JAVELIN),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 17, 12, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_BOW_LONGBOW),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_WYVERN_RIDER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 5, 2, 0, REDAs_NULL, 0, 6, 9, 0, ITEM_LANCE_STEEL),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_WYVERN_RIDER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 19, 6, 0, REDAs_NULL, 0, 6, 9, 0, ITEM_LANCE_AXEREAVER),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_WYVERN_RIDER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 16, 21, 0, REDAs_NULL, 0, 6, 9, 0, ITEM_LANCE_JAVELIN),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARMOR_KNIGHT, FACTION_ID_RED, ITEM_DROP, AUTOLEVEL, 10, 9, 6, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_LANCE_STEEL, ITEM_ELIXIR),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_CAVALIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 9, 9, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_SWORD_ARMORSLAYER),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_CAVALIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 8, 9, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_LANCE_HORSESLAYER), 
    {}
};


/**
 * Green unit and REDA definitions
 */

/**
 * Main events
 */
static const EventScr EventScr_Beginning[] = {
    LOAD_WAIT_PERSIST(CH7_ENEMY_UNITS)
    FADE_IN_SCREEN(16)
    LOAD_WAIT_DEPLOYMENT_ORDER(CH7_PLAYER_UNITS)
    STAL(15)
    MOVE_CAMERA_TO_POSITION_CENTER(9, 4)
    HIGHLIGHT_COORDINATES(9, 4, 60)
    MOVE_CAMERA_TO_POSITION(0, 21)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    SET_BACKGROUND(0x21)
    TEXT_NO_REMA(Chapter_07_Pre_Scene_01_Convo_01)
    MUSC(BGM_RAY_OF_HOPE)
    TEXT(Chapter_07_Pre_Scene_Franz)
    TEXT(Chapter_07_Pre_Scene_Gilliam_Moulder_Tana)
    TEXT(Chapter_07_Pre_Scene_Ross_Garcia)
    TEXT(Chapter_07_Pre_Scene_Neimi_Colm)
    TEXT(Chapter_07_Pre_Scene_Lute_Artur)
    TEXT(Chapter_07_Pre_Scene_Joshua)
    TEXT(Chapter_07_Pre_Scene_Natasha)
    TEXT(Chapter_07_Pre_Scene_02_Convo_01)
    PREP
    FADE_OUT_SCREEN(16)
    ENDA
};

static const EventScr EventScr_Ending[] = {
    FADE_OUT_SCREEN(16)
    LOMA(0x44)
    CLEAR_ALL_UNITS
    FADE_IN_SCREEN(16)
    MUSC(SFX_RIVER)
    LOAD_WAIT(CH7_EIRIKA_SETH)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    TEXT(Chapter_07_Post_Scene_01_Convo_01)
    MOVE_1STEP(0, CHARACTER_SETH, MOVE_RIGHT)
    MOVE_1STEP(0, CHARACTER_EIRIKA, MOVE_LEFT)
    LOAD_WAIT(CH7_ORSON)
    HIGHLIGHT_CHARACTER(CHARACTER_ORSON, 60)
    REDUCE_VOLUME
    TEXT_BG(0x10, Chapter_07_Post_Scene_02_Convo_01)
    MOVE_1STEP(0, CHARACTER_ORSON, MOVE_LEFT)
    ENUN
    MOVE(0, CHARACTER_ORSON, 9, 0)
    SVAL(EVT_SLOT_D, 0x0)
    SVAL(EVT_SLOT_1, 0x109)
    SENQUEUE1
    SVAL(EVT_SLOT_1, 0x0)
    SENQUEUE1
    SVAL(EVT_SLOT_1, 0x9)
    SENQUEUE1
    SVAL(EVT_SLOT_1, 0x0)
    SENQUEUE1
    MOVE_DEFINED(CHARACTER_EIRIKA)
    SVAL(EVT_SLOT_D, 0x0)
    SVAL(EVT_SLOT_1, 0x10a)
    SENQUEUE1
    SVAL(EVT_SLOT_1, 0x0)
    SENQUEUE1
    SVAL(EVT_SLOT_1, 0xa)
    SENQUEUE1
    SVAL(EVT_SLOT_1, 0x0)
    SENQUEUE1
    MOVE_DEFINED(CHARACTER_SETH)
    STAL(8)
    FADE_OUT_SCREEN(16)
    ENUN
    NEXT_CHAPTER_WITH_MAP(CHAPTER_08)
    ENDA
};

/**
 * Misc events
 */

static const EventListScr EventListScr_HOUSE_1_TALK[] = {
    HOUSE_EVENT_NO_END(0x0, Chapter_07_In_Scene_House_1)
    CALL(EventScr_RemoveBGIfNeeded) // This is vital, the game crashes without it for this event
    // GIVE_ITEM_TO(ITEM_SWORD_ARMORSLAYER, CHARACTER_EVT_ACTIVE)
    NOFADE
    ENDA
};
static const EventListScr EventListScr_HOUSE_2_TALK[] = {
    HOUSE_EVENT_NO_END(0x0, Chapter_07_In_Scene_House_2)
    CALL(EventScr_RemoveBGIfNeeded) // This is vital, the game crashes without it for this event
    // GIVE_ITEM_TO(ITEM_BOOSTER_DEF, CHARACTER_EVT_ACTIVE)
    NOFADE
    ENDA
};

/**
 * Event list
 */

static const EventListScr EventListScr_Turn[] = {
    END_MAIN
};

static const EventListScr EventListScr_Character[] = {
    END_MAIN
};

static const EventListScr EventListScr_Location[] = {
    VILLAGE(EVFLAG_TMP(10), EventListScr_HOUSE_1_TALK, 1, 15)
    VILLAGE(EVFLAG_TMP(11), EventListScr_HOUSE_2_TALK, 17, 21)
    SEIZE(9, 4)
    END_MAIN
};

static const EventListScr EventListScr_Misc[] = {
    DEFEAT_ALL(EventScr_Ending)
    CAUSE_GAME_OVER_IF_LORD_DIES
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

const struct ChapterEventGroup Chapter07Event = {
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

    .playerUnitsInNormal = CH7_PLAYER_UNITS,
    .playerUnitsInHard   = CH7_PLAYER_UNITS,

    .playerUnitsChoice1InEncounter = NULL,
    .playerUnitsChoice2InEncounter = NULL,
    .playerUnitsChoice3InEncounter = NULL,

    .enemyUnitsChoice1InEncounter = NULL,
    .enemyUnitsChoice2InEncounter = NULL,
    .enemyUnitsChoice3InEncounter = NULL,

    .beginningSceneEvents = EventScr_Beginning,
    .endingSceneEvents    = EventScr_Ending,
};
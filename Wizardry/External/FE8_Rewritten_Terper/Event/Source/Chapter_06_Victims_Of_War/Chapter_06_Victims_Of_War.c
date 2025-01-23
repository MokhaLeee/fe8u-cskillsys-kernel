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
static const struct REDA REDAs_EIRIKA_1[] = {
    { .x = 4, .y = 17, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_SETH_1[] = {
    { .x = 6, .y = 17, .b = -1, .delayFrames = 0, },
};


static const struct UnitDefinition CH6_PLAYER_UNITS[] = {
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 6, 17, 1, REDAs_EIRIKA_1, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_NATASHA, CLASS_CLERIC, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 5, 18, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_COLM, CLASS_THIEF, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 5, 16, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_LOCKPICK),
    UNIT_ENTRY(CHARACTER_TANA, CLASS_PEGASUS_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 0, 4, 19, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_GILLIAM, CLASS_ARMOR_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 3, 16, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_FRANZ, CLASS_CAVALIER, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 6, 19, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_NEIMI, CLASS_ARCHER_F, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 7, 18, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_BOW_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 4, 17, 1, REDAs_SETH_1, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_LANCE_SILVER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_JOSHUA, CLASS_MYRMIDON, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 4, 3, 18, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_KILLER, ITEM_VULNERARY),
    {},
};

static const struct REDA REDAs_EIRIKA_ARRIVES[] = {
    { .x = 3, .y = 0, .b = -1, .delayFrames = 0, },
    { .x = 3, .y = 2, .b = -1, .delayFrames = 0, },
    { .x = 4, .y = 2, .b = -1, .delayFrames = 0, },
    { .x = 4, .y = 8, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_SETH_ARRIVES[] = {
    { .x = 3, .y = 0, .b = 0xFFFF, .a = CHARACTER_EIRIKA, .delayFrames = 0, },
    { .x = 3, .y = 2, .b = -1, .delayFrames = 0, },
    { .x = 4, .y = 2, .b = -1, .delayFrames = 0, },
    { .x = 4, .y = 7, .b = -1, .delayFrames = 0, },
    { .x = 5, .y = 7, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH6_EIRIKA_SETH[] = {
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 5, 2, 0, 4, REDAs_EIRIKA_ARRIVES, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 2, 0, 5, REDAs_SETH_ARRIVES, 0, 0, 0, 0, ITEM_SWORD_STEEL, ITEM_LANCE_SILVER),
    {},
};

// /**
//  * Enemy unit and REDA definitions
//  */

static const struct UnitDefinition CH6_ENEMY_UNITS[] = {
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 14, 9, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 10, 0, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_JAVELIN, ITEM_LANCE_IRON), 
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 16, 9, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_AXE_POISON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_MAGE, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 18, 5, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_ANIMA_THUNDER),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 14, 11, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 0, 0, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARMOR_KNIGHT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 16, 12, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARMOR_KNIGHT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 16, 13, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_PRIEST, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 18, 9, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_STAFF_MEND),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SHAMAN, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 18, 4, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_DARK_FLUX),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 15, 15, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_AXE_STEEL), 
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_MERCENARY, FACTION_ID_RED, ITEM_DROP, AUTOLEVEL, 6, 15, 10, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_BLADE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_CAVALIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 8, 1, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_BLADE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_CAVALIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 7, 1, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_CAVALIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 11, 0, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_TROUBADOUR, FACTION_ID_RED, ITEM_DROP, AUTOLEVEL, 6, 10, 0, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_STAFF_MEND, ITEM_ELIXIR),
    UNIT_ENTRY(CHARACTER_MONSTER_B0, CLASS_BAEL, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 27, 4, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_MONSTER_POISON_CLAW),
    UNIT_ENTRY(CHARACTER_NOVALA, CLASS_SHAMAN, FACTION_ID_RED, NO_ITEM_DROP, NO_AUTOLEVEL, 10, 20, 5, 0, REDAs_NULL, 3, 3, 0, 0, ITEM_DARK_FLUX),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 12, 4, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_BOW_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 14, 6, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_LANCE_IRON), 
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 20, 7, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARMOR_KNIGHT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 21, 13, 0, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_HORSESLAYER, ITEM_LANCE_JAVELIN),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 19, 13, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, ITEM_DROP, AUTOLEVEL, 5, 13, 6, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_AXE_IRON, ITEM_AXE_HALBERD),
    {}
};

static const struct REDA REDAs_CAVALIER_1[] = {
    { .x = 3, .y = 11, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_CAVALIER_2[] = {
    { .x = 2, .y = 9, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_CAVALIER_3[] = {
    { .x = 2, .y = 13, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH6_TURN_4_REINFORCEMENTS[] = {
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_CAVALIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 0, 11, 1, REDAs_CAVALIER_1, 0, 0, 1, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_CAVALIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 0, 9, 1, REDAs_CAVALIER_2, 0, 0, 1, 0, ITEM_SWORD_IRON, ITEM_LANCE_JAVELIN), 
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_CAVALIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 0, 13, 1, REDAs_CAVALIER_3, 0, 0, 1, 0, ITEM_LANCE_JAVELIN, ITEM_LANCE_IRON), 
    {}
};

/**
 * Green unit and REDA definitions
 */

static const struct UnitDefinition CH6_RENAIS_CITIZENS[] = {
    UNIT_ENTRY(CHARACTER_CITIZEN_YOUNG_GIRL, 0x6E, FACTION_ID_GREEN, NO_ITEM_DROP, AUTOLEVEL, 1, 26, 12, 0, REDAs_NULL, 0, 3, 0, 0),
    UNIT_ENTRY(CHARACTER_CITIZEN_YOUNG_MAN, 0x71, FACTION_ID_GREEN, NO_ITEM_DROP, AUTOLEVEL, 1, 25, 13, 0, REDAs_NULL, 0, 3, 0, 0), 
    UNIT_ENTRY(CHARACTER_CITIZEN_TEEN_GIRL, 0x72, FACTION_ID_GREEN, NO_ITEM_DROP, AUTOLEVEL, 1, 25, 12, 0, REDAs_NULL, 0, 3, 0, 0),
    {}
};

/**
 * Main events
 */
static const EventScr EventScr_Beginning[] = {
    MUSC(BGM_SOLVE_THE_RIDDLE)
    SET_BACKGROUND(0x1A)
    TEXT(Chapter_06_Pre_Scene_01_Convo_01)
    MUSC(MUTE)
    FADE_OUT_SCREEN(16)
    CLEAN
    MAKE_CAMERA_FOLLOW_MOVING_UNITS
    FADE_IN_SCREEN(16)
    LOAD_WAIT(CH6_EIRIKA_SETH)
    STOP_CAMERA_FOLLOW_MOVING_UNITS
    MOVE_CAMERA_TO_POSITION(7, 7)
    LOAD_WAIT_PERSIST(CH6_RENAIS_CITIZENS)
    LOAD_WAIT_PERSIST(CH6_ENEMY_UNITS)
    WARP_CHARACTER_TO_COORDINATES(CHARACTER_NOVALA, 5, 8)
    WARP_CHARACTER_TO_COORDINATES(0xF9, 6, 8)
    MOVE_1STEP(0, CHARACTER_EIRIKA, MOVE_LEFT)
    MOVE_1STEP(0, CHARACTER_SETH, MOVE_LEFT)
    HIGHLIGHT_CHARACTER(CHARACTER_NOVALA, 60)
    SET_BACKGROUND(0x1A)
    TEXT_CHANGE_BGM(Chapter_06_Pre_Scene_02_Convo_01, BGM_RAID)
    SET_BACKGROUND(0x1A)
    TEXT(Chapter_06_Pre_Scene_02_Convo_02)
    FADE_OUT_SCREEN(16)
    CLEAN
    FADE_IN_SCREEN(16)
    MOVE(0, 0xFB, 20, 5)
    WARP_CHARACTER_OUT(CHARACTER_NOVALA)
    WARP_CHARACTER_OUT(0xF9)
    MOVE_CAMERA_TO_POSITION(19, 5)
    WARP_CHARACTER_TO_COORDINATES(CHARACTER_NOVALA, 19, 6)
    WARP_CHARACTER_TO_COORDINATES(0xF9, 20, 6)
    HIGHLIGHT_CHARACTER(CHARACTER_NOVALA, 60)
    TEXT_BG(0x1E, Chapter_06_Pre_Scene_03_Convo_01)
    STAL(60)
    WARP_CHARACTER_OUT(0xF9)
    WARP_CHARACTER_OUT(0xFB)
    MOVE_CAMERA_TO_POSITION(21, 11)
    WARP_CHARACTER_TO_COORDINATES(0xF9, 26, 12)
    WARP_CHARACTER_TO_COORDINATES(0xFB, 25, 12)
    HIGHLIGHT_CHARACTER(0xF9, 60)
    TEXT_BG(0x1E, Chapter_06_Pre_Scene_03_Convo_02)
    FADE_OUT_SCREEN(16)
    PREP
    ENDA
};

static const EventScr EventScr_Ending[] = {
    MUSC(MUTE)
    CHECK_ALIVE(0xfa)
    BEQ(0x0, EVT_SLOT_C, EVT_SLOT_0)
    CHECK_ALIVE(0xfb)
    BEQ(0x0, EVT_SLOT_C, EVT_SLOT_0)
    CHECK_ALIVE(0xf9)
    BEQ(0x0, EVT_SLOT_C, EVT_SLOT_0)
    SET_BACKGROUND(0x1E)
    MUSC(BGM_VICTORY)
    TEXTSHOW(Chapter_06_Post_Scene_01_Convo_01)
    TEXTEND
    REMA
    MUSC(MUTE)
    CALL(EventScr_RemoveBGIfNeeded)
    SVAL(EVT_SLOT_3, ITEM_BOOSTER_POW)
    GIVEITEMTO(CHARACTER_EIRIKA)
    FADE_OUT_SCREEN(3)

LABEL(0x0)
    REMA
    SET_BACKGROUND(0x20)
    MUSC(BGM_SORROW)
    FADE_IN_SCREEN(3)
    TEXT_NO_REMA(Chapter_06_Post_Scene_02_Convo_01)
    FADE_OUT_SCREEN(3)
    REMA
    FADE_IN_SCREEN(4)
    TEXT_NO_REMA(Chapter_06_Post_Scene_03_Convo_01)
    FADE_OUT_SCREEN(4)
    REMA
    FADE_IN_SCREEN(4)
    TEXT_NO_REMA(Chapter_06_Post_Scene_04_Convo_01)
    FADE_OUT_SCREEN(4)
    REMA
    FADE_IN_SCREEN(4)
    TEXT_NO_REMA(Chapter_06_Post_Scene_05_Convo_01)
    MUSC(MUTE)
    FADE_OUT_SCREEN(4)
    REMA
    FADE_IN_SCREEN(4)
    TEXT_NO_REMA(Chapter_06_Post_Scene_06_Convo_01)
    FADE_OUT_SCREEN(2)
    REMA
    STAL(60)
    TEXTCONT
    TEXTEND
    REMA
    NEXT_CHAPTER_WITH_MAP(CHAPTER_07) // Chapter 7 - Waterside Renvall
    ENDA
};

/**
 * Misc events
 */

static const EventListScr EventScr_CH6_TURN_4_REINFORCEMENTS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 8)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    MOVE_CAMERA_TO_POSITION(13, 0)
    LOAD_WAIT_PERSIST(CH6_TURN_4_REINFORCEMENTS)
    GOTO(0x1)

LABEL(0x0)
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF_SLOT2
    GOTO(0x1)

LABEL(0x1)
    NOFADE
    ENDA
};

static const EventListScr EventListScr_HOUSE_1_TALK[] = {
    HOUSE_EVENT_NO_END(0x0, Chapter_06_In_Scene_House_1)
    CALL(EventScr_RemoveBGIfNeeded) // This is vital, the game crashes without it for this event
    GIVE_ITEM_TO(ITEM_ANTITOXIN, CHARACTER_EVT_ACTIVE)
    NOFADE
    ENDA
};

const EventListScr EventScr_EndAndResetTriggEvent_JESTER[] = {
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF(-1)

    NOFADE
    ENDB
};


const EventListScr EventScr_UnTriggerIfNotUnit_JESTER[] = {
    CHECK_ACTIVE
    BEQ(0x0, EVT_SLOT_C, EVT_SLOT_2)

    CALL(EventScr_EndAndResetTriggEvent)

LABEL(0x0)
    ENDA
};

static const EventListScr EventScr_CH6_CHECK_AREA[] = {
    CHECK_ALIVE(0xf9)
    BEQ(0x63, EVT_SLOT_C, EVT_SLOT_0)
    CHECK_INAREA(0xf9, 24, 9, 4, 6)
    BEQ(0x4, EVT_SLOT_C, EVT_SLOT_0)
    SVAL(EVT_SLOT_2, 0xb0)
    CALL(EventScr_UnTriggerIfNotUnit_JESTER)
    MUSC(0x18)
    CAMERA_CAHR(0xf9)
    CUMO_CHAR(0xf9)
    STAL(60)
    CURE
    RANDOMNUMBER(2)
    SVAL(EVT_SLOT_7, 0x1)
    BEQ(0x1, EVT_SLOT_C, EVT_SLOT_7)
    SVAL(EVT_SLOT_7, 0x2)
    BEQ(0x2, EVT_SLOT_C, EVT_SLOT_7)
LABEL(0x0)
    SVAL(EVT_SLOT_2, 0x9ec)
    GOTO(0x3)
LABEL(0x1)
    SVAL(EVT_SLOT_2, 0x9ed)
    GOTO(0x3)
LABEL(0x2)
    SVAL(EVT_SLOT_2, 0x9ee)
LABEL(0x3)
    TEXTSTART
    TEXTSHOW(0xffff)
    TEXTEND
    REMA
LABEL(0x4)
    CALL(EventScr_EndAndResetTriggEvent_JESTER)
LABEL(0x63)
    EVBIT_T(7)
    ENDA
};

/**
 * Event list
 */

static const EventListScr EventListScr_Turn[] = {
    AFEV(EVFLAG_TMP(7), EventScr_CH6_TURN_4_REINFORCEMENTS, 0)
    END_MAIN
};

static const EventListScr EventListScr_Character[] = {
    END_MAIN
};

static const EventListScr EventListScr_Location[] = {
    VILLAGE(EVFLAG_TMP(10), EventListScr_HOUSE_1_TALK, 11, 18)
    END_MAIN
};

static const EventListScr EventListScr_Misc[] = {
    DEFEAT_ALL(EventScr_Ending)
    CAUSE_GAME_OVER_IF_LORD_DIES
    AFEV(EVFLAG_TMP(7), EventScr_CH6_CHECK_AREA, 0)
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

const struct ChapterEventGroup Chapter06Event = {
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

    .playerUnitsInNormal = CH6_PLAYER_UNITS,
    .playerUnitsInHard   = CH6_PLAYER_UNITS,

    .playerUnitsChoice1InEncounter = NULL,
    .playerUnitsChoice2InEncounter = NULL,
    .playerUnitsChoice3InEncounter = NULL,

    .enemyUnitsChoice1InEncounter = NULL,
    .enemyUnitsChoice2InEncounter = NULL,
    .enemyUnitsChoice3InEncounter = NULL,

    .beginningSceneEvents = EventScr_Beginning,
    .endingSceneEvents    = EventScr_Ending,
};
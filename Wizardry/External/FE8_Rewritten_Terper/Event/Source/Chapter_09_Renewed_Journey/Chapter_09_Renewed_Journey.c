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
static const struct REDA REDAs_EIRIKA_2[] = {
    { .x = 4, .y = 17, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_SETH_2[] = {
    { .x = 6, .y = 17, .b = -1, .delayFrames = 0, },
};


static const struct UnitDefinition CH5_PLAYER_UNITS[] = {
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 6, 17, 1, REDAs_EIRIKA_2, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_NATASHA, CLASS_CLERIC, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 5, 18, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_COLM, CLASS_THIEF, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 5, 16, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_LOCKPICK),
    UNIT_ENTRY(CHARACTER_TANA, CLASS_PEGASUS_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 0, 4, 19, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_GILLIAM, CLASS_ARMOR_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 3, 16, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_FRANZ, CLASS_CAVALIER, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 6, 19, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_NEIMI, CLASS_ARCHER_F, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 7, 18, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_BOW_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 4, 17, 1, REDAs_SETH_2, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_LANCE_SILVER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_JOSHUA, CLASS_MYRMIDON, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 4, 3, 18, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_KILLER, ITEM_VULNERARY),
    {},
};

static const struct REDA REDAs_NATASHA_2[] = {
    { .x = 9, .y = 12, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH5_NATASHA_RUNNING[] = {
    UNIT_ENTRY(CHARACTER_NATASHA, CLASS_CLERIC, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 10, 0, 1, REDAs_NATASHA_2, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    {},
};

static const struct REDA REDAs_EIRIKA_ARRIVES[] = {
    { .x = 6, .y = 16, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_SETH_ARRIVES[] = {
    { .x = 5, .y = 15, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH5_EIRIKA_SETH[] = {
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 5, 0, 16, 1, REDAs_EIRIKA_ARRIVES, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 0, 15, 1, REDAs_SETH_ARRIVES, 0, 0, 0, 0, ITEM_SWORD_STEEL, ITEM_LANCE_SILVER),
    {},
};

// /**
//  * Enemy unit and REDA definitions
//  */

static const struct REDA REDAs_JOSHUA_1[] = {
    { .x = 9, .y = 6, .b = -1, .delayFrames = 0, },
    { .x = 9, .y = 7, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_NATASHA_1[] = {
    { .x = 9, .y = 10, .b = -1, .delayFrames = 0, },
    { .x = 9, .y = 8, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH5_JOSHUA[] = {
    UNIT_ENTRY(CHARACTER_JOSHUA, CLASS_MYRMIDON, FACTION_ID_RED,  NO_ITEM_DROP, AUTOLEVEL, 1, 9, 6, 2, REDAs_JOSHUA_1, 0, 0, 0, 0, ITEM_SWORD_KILLER, ITEM_VULNERARY), 
    {}
};

static const struct UnitDefinition CH5_NATASHA[] = {
    UNIT_ENTRY(CHARACTER_NATASHA, CLASS_CLERIC, FACTION_ID_BLUE, NO_ITEM_DROP, NO_AUTOLEVEL, 1, 9, 15, 2, REDAs_NATASHA_1, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    {}
};

static const struct UnitDefinition CH5_SAAR_GLEN_CORMAG[] = {
    UNIT_ENTRY(CHARACTER_SAAR, CLASS_ARMOR_KNIGHT, FACTION_ID_RED,  NO_ITEM_DROP, AUTOLEVEL, 1, 9, 7, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_LANCE_IRON), 
    UNIT_ENTRY(CHARACTER_GLEN, CLASS_WYVERN_LORD, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 9, 9, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_CORMAG, CLASS_WYVERN_RIDER, FACTION_ID_RED,  NO_ITEM_DROP, AUTOLEVEL, 1, 8, 10, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_LANCE_IRON),
    {}
};

static const struct UnitDefinition CH5_JOSHUA_ARENA[] = {
    UNIT_ENTRY(CHARACTER_JOSHUA, CLASS_MYRMIDON, FACTION_ID_RED,  NO_ITEM_DROP, NO_AUTOLEVEL, 5, 12, 6, 0, REDAs_NULL, 0, 7, 3, 9, ITEM_SWORD_KILLER),
    {},
};

static const struct REDA REDAs_SAAR[] = {
    { .x = 12, .y = 1, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_SOLDIER_1[] = {
    { .x = 11, .y = 2, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_SOLDIER_2[] = {
    { .x = 13, .y = 3, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_SOLDIER_3[] = {
    { .x = 9, .y = 9, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_SOLDIER_4[] = {
    { .x = 9, .y = 4, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_FIGHTER_1[] = {
    { .x = 10, .y = 8, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_FIGHTER_2[] = {
    { .x = 8, .y = 8, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH5_GRADO_UNITS_PURSUING[] = {
    UNIT_ENTRY(CHARACTER_SAAR, CLASS_ARMOR_KNIGHT, FACTION_ID_RED, NO_ITEM_DROP, NO_AUTOLEVEL, 1, 13, 0, 1, REDAs_SAAR, 3, 3, 9, 14, ITEM_LANCE_JAVELIN), 
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 12, 0, 1, REDAs_SOLDIER_1, 0, 3, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 13, 0, 1, REDAs_SOLDIER_2, 0, 3, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 12, 0, 1, REDAs_SOLDIER_3, 0, 0, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 10, 0, 1, REDAs_SOLDIER_4, 0, 0, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 10, 0, 1, REDAs_FIGHTER_1, 0, 3, 9, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 10, 0, 1, REDAs_FIGHTER_2, 0, 3, 9, 0, ITEM_AXE_IRON),
    {}
};

static const struct REDA REDAs_MERCENARY_1[] = {
    { .x = 3, .y = 3, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_FIGHTER_3[] = {
    { .x = 7, .y = 2, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_FIGHTER_4[] = {
    { .x = 0, .y = 0, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_FIGHTER_5[] = {
    { .x = 4, .y = 10, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_ARCHER_1[] = {
    { .x = 10, .y = 0, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_ARCHER_2[] = {
    { .x = 1, .y = 7, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_ARCHER_3[] = {
    { .x = 3, .y = 7, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_SOLDIER_5[] = {
    { .x = 0, .y = 10, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_SOLDIER_6[] = {
    { .x = 5, .y = 7, .b = -1, .delayFrames = 0, },
};


static const struct UnitDefinition CH5_GRADO_ENEMY_UNITS[] = {
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 0, 0, 1, REDAs_MERCENARY_1, 0, 3, 9, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 5, 10, 0, 1, REDAs_FIGHTER_3, 0, 3, 9, 0, ITEM_AXE_IRON), 
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 5, 0, 0, 1, REDAs_FIGHTER_4, 0, 3, 9, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 4, 10, 0, 1, REDAs_FIGHTER_5, 0, 0, 9, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 5, 11, 0, 1, REDAs_ARCHER_1, 0, 0, 9, 0, ITEM_BOW_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 5, 0, 0, 1, REDAs_ARCHER_2, 0, 3, 9, 0, ITEM_BOW_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 4, 10, 0, 1, REDAs_ARCHER_3, 0, 0, 9, 0, ITEM_BOW_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 5, 0, 0, 1, REDAs_SOLDIER_5, 0, 0, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 5, 10, 0, 1, REDAs_SOLDIER_6, 0, 0, 9, 0, ITEM_LANCE_IRON),
    {}
};

static const struct UnitDefinition CH5_TURN_2_REINFORCEMENTS[] = {
    UNIT_ENTRY(CHARACTER_ENEMY_65, CLASS_BRIGAND, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 4, 14, 16, 0, REDAs_NULL, 0, 4, 9, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_BANDIT_CH5, CLASS_BRIGAND, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 4, 14, 15, 0, REDAs_NULL, 0, 4, 9, 0, ITEM_AXE_IRON), 
    {}
};

static const struct UnitDefinition CH5_TURN_6_REINFORCEMENTS[] = {
    UNIT_ENTRY(CHARACTER_ENEMY_65, CLASS_BRIGAND, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 4, 14, 8, 0, REDAs_NULL, 0, 4, 9, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_ENEMY_65, CLASS_BRIGAND, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 4, 14, 6, 0, REDAs_NULL, 0, 4, 9, 0, ITEM_AXE_IRON), 
    {}
};

static const struct UnitDefinition CH5_TURN_8_REINFORCEMENTS[] = {
    UNIT_ENTRY(CHARACTER_ENEMY_65, CLASS_BRIGAND, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 4, 13, 0, 0, REDAs_NULL, 0, 4, 9, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_ENEMY_65, CLASS_BRIGAND, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 4, 14, 1, 0, REDAs_NULL, 0, 4, 9, 0, ITEM_AXE_IRON), 
    {}
};


/**
 * Green unit and REDA definitions
 */

/**
 * Main events
 */
static const EventScr EventScr_Beginning[] = {
    // CHECK_EVENTID(136) // Flag 0x88
    // BEQ(0x0, EVT_SLOT_C, EVT_SLOT_0)
    // CALL(EventScr_NEW_JOURNEY)
// LABEL(0x0)

    MUSC(BGM_SHOPS)
    LOAD_WAIT(CH5_JOSHUA)
    TEXT_BG(0x5, Chapter_05_Scene_01_Convo_01)
    LOAD_WAIT(CH5_NATASHA)
    HIGHLIGHT_CHARACTER(CHARACTER_JOSHUA, 60)
    SET_BACKGROUND(0x5)
    TEXTSTART
    TEXTSHOW(Chapter_05_Scene_01_Convo_02)
    TEXTEND
    SET_BACKGROUND(BLACK_BACKGROUND)
    REMA
    FADE_OUT_SCREEN(16)
    CLEAR_ALL_UNITS
    CLEAN
    MUSC(BGM_SOLVE_THE_RIDDLE)
    LOAD_WAIT_PERSIST(CH5_SAAR_GLEN_CORMAG)
    FADE_IN_SCREEN(16)
    HIGHLIGHT_CHARACTER(CHARACTER_GLEN, 60)
    TEXT_BG(0x5, Chapter_05_Scene_02_Convo_01)
    MOVE_CLOSEST_ENUN(0, CHARACTER_SAAR, 9, 4)
    MOVE_1STEP_WAIT(16, CHARACTER_CORMAG, MOVE_UP)
    HIGHLIGHT_CHARACTER(CHARACTER_GLEN, 60)
    TEXT_BG(0x5, Chapter_05_Scene_02_Convo_02)
    MOVE_CLOSEST_ENUN(0, CHARACTER_GLEN, 9, 7)
    MOVE_CLOSEST_ENUN(0, CHARACTER_GLEN, 11, 7)
    MOVE_CLOSEST_ENUN(0, CHARACTER_GLEN, 11, 4)
    MOVE_CLOSEST_ENUN(0, CHARACTER_CORMAG, 8, 7)
    MOVE_CLOSEST_ENUN(0, CHARACTER_CORMAG, 11, 7)
    MOVE_CLOSEST_ENUN(0, CHARACTER_CORMAG, 11, 4)
    STAL(30)
    FADE_OUT_SCREEN(16)
    MUSC(0x7FFF)
    CLEAR_ALL_UNITS
    SET_BACKGROUND(0x24)
    MUSC(BGM_ADVANCE)
    TEXT(Chapter_05_Scene_03_Convo_01)
    REDUCE_VOLUME
    STAL(30)
    RESTORE_VOLUME
    TEXT_BG(0x24, Chapter_05_Scene_03_Convo_02)
    FADE_OUT_SCREEN(16)
    MUSC(0x7FFF)
    LOAD_WAIT_PERSIST(CH5_NATASHA_RUNNING)
    LOAD_WAIT_PERSIST(CH5_GRADO_UNITS_PURSUING)
    MUSC(BGM_RAID)
    TEXT(Chapter_05_Scene_04_Convo_01)
    LOAD_WAIT(CH5_JOSHUA_ARENA)
    MOVE_CLOSEST_ENUN(0, CHARACTER_JOSHUA, 9, 7)
    HIGHLIGHT_CHARACTER(CHARACTER_JOSHUA, 60)
    TEXT(Chapter_05_Scene_04_Convo_02)
    MOVE_CLOSEST_ENUN(0, CHARACTER_JOSHUA, 8, 7)
    KILL_AT_COORDINATES_ENUN(8, 8)
    MOVE_CLOSEST_ENUN(0, CHARACTER_JOSHUA, 10, 7)
    KILL_AT_COORDINATES_ENUN(10, 8)
    MOVE_CLOSEST_ENUN(0, CHARACTER_JOSHUA, 9, 8)
    TEXT(Chapter_05_Scene_04_Convo_03)
    KILL_AT_COORDINATES_ENUN(9, 9)
    STAL(32)
    TEXT_BG(0x5, Chapter_05_Scene_04_Convo_04)
    CHANGE_TO_BLUE(CHARACTER_JOSHUA)
    LOAD_WAIT_PERSIST(CH5_GRADO_ENEMY_UNITS)
    MOVE_CAMERA_TO_POSITION(7, 14)
    LOAD_WAIT(CH5_EIRIKA_SETH)
    HIGHLIGHT_CHARACTER(CHARACTER_SETH, 60)
    TEXT(Chapter_05_Scene_04_Convo_05)
    MOVE_WAIT(0, CHARACTER_NATASHA, 6, 15)
    MOVE_WAIT(0, CHARACTER_JOSHUA, 7, 15)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    TEXT(Chapter_05_Scene_04_Convo_06)
    FADE_OUT_SCREEN(16)
    LOAD_WAIT_PERSIST(CH5_PLAYER_UNITS)
    CALL(EventScr_08591FD8) // Prep screen
    FADE_IN_SCREEN(16)
    MUSC(BGM_DISTANT_ROADS)
    NOFADE
    ENDA
};

static const EventScr EventScr_Ending[] = {
    FADE_OUT_SCREEN(16)

    SET_BACKGROUND(0x5)
    TEXTSTART
    TEXTSHOW(Chapter_05_Scene_05_Convo_01)
    TEXTEND
    SET_BACKGROUND(BLACK_BACKGROUND)
    REMA

    CHECK_EVENTID(10)
    BEQ(0x0, EVT_SLOT_C, EVT_SLOT_0)
    CHECK_EVENTID(11)
    BEQ(0x0, EVT_SLOT_C, EVT_SLOT_0)
    CHECK_EVENTID(12)
    BEQ(0x0, EVT_SLOT_C, EVT_SLOT_0)
    CHECK_EVENTID(13)
    BEQ(0x0, EVT_SLOT_C, EVT_SLOT_0)

    SET_BACKGROUND(0x5)
    TEXTSTART
    TEXTSHOW(Chapter_05_Scene_06_Convo_01)
    TEXTEND
    GIVE_ITEM_TO(ITEM_MASTERSEAL, CHARACTER_EIRIKA)
    REMA

    GOTO(0x0)
LABEL(0x0)
    NEXT_CHAPTER_WITHOUT_MAP(0x5) // Chapter 5x - Unbroken Promise
    ENDA
};

/**
 * Misc events
 */

static const EventListScr EventScr_CH5_TURN_2_REINFORCEMENTS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 2)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    MUSC(BGM_SHADOW_OF_THE_ENEMY)
    MOVE_CAMERA_TO_POSITION(14, 16)
    LOAD_WAIT_PERSIST(CH5_TURN_2_REINFORCEMENTS)
    HIGHLIGHT_COORDINATES(14, 16, 60)
    TEXT(Chapter_05_BANDIT)
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

static const EventListScr EventScr_CH5_TURN_6_REINFORCEMENTS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 6)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    MOVE_CAMERA_TO_POSITION(14, 8)
    LOAD_WAIT_PERSIST(CH5_TURN_6_REINFORCEMENTS)
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

static const EventListScr EventScr_CH5_TURN_8_REINFORCEMENTS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 8)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    MOVE_CAMERA_TO_POSITION(13, 0)
    LOAD_WAIT_PERSIST(CH5_TURN_8_REINFORCEMENTS)
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
    HOUSE_EVENT_NO_END(0x0, Chapter_05_House_01)
    CALL(EventScr_RemoveBGIfNeeded) // This is vital, the game crashes without it for this event
    GIVE_ITEM_TO(ITEM_SWORD_ARMORSLAYER, CHARACTER_EVT_ACTIVE)
    NOFADE
    ENDA
};
static const EventListScr EventListScr_HOUSE_2_TALK[] = {
    HOUSE_EVENT_NO_END(0x0, Chapter_05_House_02)
    CALL(EventScr_RemoveBGIfNeeded) // This is vital, the game crashes without it for this event
    GIVE_ITEM_TO(ITEM_BOOSTER_DEF, CHARACTER_EVT_ACTIVE)
    NOFADE
    ENDA
};
static const EventListScr EventListScr_HOUSE_3_TALK[] = {
    HOUSE_EVENT_NO_END(0x0, Chapter_05_House_03)
    CALL(EventScr_RemoveBGIfNeeded) // This is vital, the game crashes without it for this event
    GIVE_ITEM_TO(ITEM_BOOSTER_SKL, CHARACTER_EVT_ACTIVE)
    NOFADE
    ENDA
};
static const EventListScr EventListScr_HOUSE_4_TALK[] = {
    HOUSE_EVENT_NO_END(0x0, Chapter_05_House_04)
    CALL(EventScr_RemoveBGIfNeeded) // This is vital, the game crashes without it for this event
    GIVE_ITEM_TO(ITEM_TORCH, CHARACTER_EVT_ACTIVE)
    NOFADE
    ENDA
};

static const u16 EventListScr_CH5_ARMORY[] = {
    ITEM_SWORD_SLIM,
    ITEM_SWORD_IRON,
    ITEM_SWORD_STEEL,
    ITEM_LANCE_SLIM,
    ITEM_LANCE_IRON,
    ITEM_LANCE_STEEL,
    ITEM_BOW_IRON,
    ITEM_BOW_STEEL,
};

static const u16 EventListScr_CH5_VENDOR[] = {
    ITEM_ANIMA_FIRE,
    ITEM_LIGHT_LIGHTNING,
    ITEM_STAFF_HEAL,
    ITEM_VULNERARY,
};


/**
 * Event list
 */

static const EventListScr EventListScr_Turn[] = {
    AFEV(EVFLAG_TMP(7), EventScr_CH5_TURN_2_REINFORCEMENTS, 0)
    AFEV(EVFLAG_TMP(8), EventScr_CH5_TURN_6_REINFORCEMENTS, 0)
    AFEV(EVFLAG_TMP(9), EventScr_CH5_TURN_8_REINFORCEMENTS, 0)
    END_MAIN
};

static const EventListScr EventListScr_Character[] = {
    END_MAIN
};

static const EventListScr EventListScr_Location[] = {
    ARMORY(EventListScr_CH5_ARMORY, 2, 1)
    VENDOR(EventListScr_CH5_VENDOR, 6, 10)
    VILLAGE(EVFLAG_TMP(10), EventListScr_HOUSE_1_TALK, 12, 10)
    VILLAGE(EVFLAG_TMP(11), EventListScr_HOUSE_2_TALK, 12, 19)
    VILLAGE(EVFLAG_TMP(12), EventListScr_HOUSE_3_TALK, 5, 6)
    VILLAGE(EVFLAG_TMP(13), EventListScr_HOUSE_4_TALK, 5, 1)
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

const struct ChapterEventGroup Chapter09Event = {
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

    .playerUnitsInNormal = CH5_PLAYER_UNITS,
    .playerUnitsInHard   = CH5_PLAYER_UNITS,

    .playerUnitsChoice1InEncounter = NULL,
    .playerUnitsChoice2InEncounter = NULL,
    .playerUnitsChoice3InEncounter = NULL,

    .enemyUnitsChoice1InEncounter = NULL,
    .enemyUnitsChoice2InEncounter = NULL,
    .enemyUnitsChoice3InEncounter = NULL,

    .beginningSceneEvents = EventScr_Beginning,
    .endingSceneEvents    = EventScr_Ending,
};
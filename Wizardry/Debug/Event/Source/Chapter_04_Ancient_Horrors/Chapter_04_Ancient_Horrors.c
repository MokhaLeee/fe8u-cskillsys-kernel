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
static const struct REDA REDAs_TANA[] = { 
    { .x = 4, .y = 2, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_EIRIKA[] = {
    { .x = 5, .y = 2, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_SETH[] = {
    { .x = 6, .y = 1, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_ROSS[] = { 
    { .x = 2, .y = 2, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_GILLIAM[] = {
    { .x = 3, .y = 3, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_MOULDER[] = {
    { .x = 5, .y = 1, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_NEIMI[] = {
    { .x = 3, .y = 1, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_COLM[] = {
    { .x = 4, .y = 0, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH4_PLAYER_UNITS[] = {
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 5, 2, 1, REDAs_EIRIKA, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 6, 0, 1, REDAs_SETH, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_LANCE_SILVER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_ROSS, CLASS_JOURNEYMAN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 2, 2, 0, 1, REDAs_ROSS, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_GILLIAM, CLASS_ARMOR_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 3, 0, 1, REDAs_GILLIAM, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_MOULDER, CLASS_PRIEST, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 5, 0, 1, REDAs_MOULDER, 0, 0, 0, 0, ITEM_STAFF_HEAL, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_TANA, CLASS_PEGASUS_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 0, 4, 0, 1, REDAs_TANA, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_NEIMI, CLASS_ARCHER_F, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 3, 0, 1, REDAs_NEIMI, 0, 0, 0, 0, ITEM_BOW_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_COLM, CLASS_THIEF, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 4, 0, 1, REDAs_COLM, 0, 0, 0, 0, ITEM_BOW_IRON, ITEM_VULNERARY),
    {},
};

static const struct REDA REDAs_ARTUR[] = {
    { .x = 8, .y = 6, .b = -1, .delayFrames = 0, },
    { .x = 8, .y = 3, .b = -1, .delayFrames = 0, },
    { .x = 7, .y = 3, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH4_ARTUR[] = {
    UNIT_ENTRY(CHARACTER_ARTUR, CLASS_MONK, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 2, 9, 10, 3, REDAs_ARTUR, 0, 0, 0, 0, ITEM_LIGHT_LIGHTNING, ITEM_VULNERARY),
    {},
};

/**
 * Enemy unit and REDA definitions
 */

static const struct UnitDefinition CH4_ENEMY_UNITS[] = {
    UNIT_ENTRY(CHARACTER_MONSTER_B7, CLASS_BONEWALKER_BOW, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 11, 6, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_BOW_IRON), 
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 8, 14, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 7, 9, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 5, 8, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_B7, CLASS_BONEWALKER_BOW, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 13, 7, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_BOW_IRON),
    UNIT_ENTRY(CHARACTER_MONSTER_AC, CLASS_BONEWALKER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 2, 7, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_MONSTER_AC, CLASS_BONEWALKER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 3, 10, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 0, 4, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_B7, CLASS_BONEWALKER_BOW, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 12, 8, 0, REDAs_NULL, 0, 12, 0xC, 0, ITEM_BOW_IRON), 
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 5, 13, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 11, 4, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_AC, CLASS_BONEWALKER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 1, 14, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 10, 12, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 11, 14, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_ENTOMBED_CH4, CLASS_ENTOUMBED, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 13, 13, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_MONSTER_FETIDCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_B7, CLASS_BONEWALKER_BOW, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 13, 11, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_BOW_IRON),   
    {}
};

// static const struct UnitDefinition CH3_GRADO_1[] = {
//     UNIT_ENTRY(CHARACTER_VIGARDE, CLASS_GENERAL, FACTION_ID_RED,  NO_ITEM_DROP, NO_AUTOLEVEL, 6, 14, 2, 1, REDAs_VIGARDE_ENEMY, 3, 3, 9, 20, ITEM_AXE_STEEL, ITEM_AXE_HANDAXE), 
//     UNIT_ENTRY(CHARACTER_LYON, CLASS_NECROMANCER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 15, 3, 1, REDAs_LYON_ENEMY, 0, 3, 9, 0, ITEM_AXE_IRON),
//     UNIT_ENTRY(CHARACTER_DUESSEL, CLASS_GREAT_KNIGHT, FACTION_ID_RED,  NO_ITEM_DROP, AUTOLEVEL, 2, 14, 5, 1, REDAs_DUESSEL_ENEMY, 0, 3, 9, 0, ITEM_AXE_HANDAXE),
//     UNIT_ENTRY(CHARACTER_GLEN, CLASS_WYVERN_LORD, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 13, 5, 1, REDAs_GLEN_ENEMY, 0, 12, 9, 0, ITEM_AXE_IRON, ITEM_VULNERARY),
//     UNIT_ENTRY(CHARACTER_SELENA, CLASS_MAGE_KNIGHT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 4, 15, 5, 1, REDAs_SELENA_ENEMY, 0, 3, 9, 0, ITEM_BOW_IRON),
//     {}
// };

// static const struct UnitDefinition CH3_GRADO_2[] = {
//     UNIT_ENTRY(CHARACTER_RIEV, CLASS_BISHOP, FACTION_ID_RED,  NO_ITEM_DROP, NO_AUTOLEVEL, 6, 14, 10, 1, REDAs_RIEV_ENEMY, 3, 3, 9, 20, ITEM_AXE_STEEL, ITEM_AXE_HANDAXE), 
//     UNIT_ENTRY(CHARACTER_CAELLACH, CLASS_HERO, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 15, 10, 1, REDAs_CAELLACH_ENEMY, 0, 3, 9, 0, ITEM_AXE_IRON),
//     UNIT_ENTRY(CHARACTER_VALTER, CLASS_WYVERN_KNIGHT, FACTION_ID_RED,  NO_ITEM_DROP, AUTOLEVEL, 2, 13, 10, 1, REDAs_VALTER_ENEMY, 0, 3, 9, 0, ITEM_AXE_HANDAXE),
//     {}
// };

/**
 * Green unit and REDA definitions
 */

// static const struct REDA REDAs_COLM[] = {
//     { .x = 0, .y = 5, .b = -1 },
// };
// static const struct REDA REDAs_ROSS[] = {
//     { .x = 3, .y = 15, .b = -1 }, 
// };

// static const struct UnitDefinition CH3_COLM[] = {
//     UNIT_ENTRY(CHARACTER_COLM, CLASS_THIEF, FACTION_ID_GREEN, NO_ITEM_DROP, NO_AUTOLEVEL, 2, 0, 5, 1, REDAs_COLM,  6, 5, 8, 1, ITEM_SWORD_IRON, ITEM_LOCKPICK, ITEM_VULNERARY),
//     {}
// };

// static const struct UnitDefinition CH3_ROSS[] = {
//     UNIT_ENTRY(CHARACTER_ROSS, CLASS_JOURNEYMAN, FACTION_ID_GREEN, NO_ITEM_DROP, NO_AUTOLEVEL, 1, 3, 15, 1, REDAs_ROSS,  0, 0xA, 0, 0, ITEM_AXE_HATCHET, ITEM_VULNERARY),
//     {}
// };


/**
 * Main events
 */
static const EventScr EventScr_Beginning[] = {
    LOAD_WAIT(CH4_PLAYER_UNITS)
    SOUN(SFX_BIRDS_CHIRPING)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    Text_BG(0x23, Chapter_04_Scene_01_Convo_01)
    FADE_OUT_SCREEN(16)
    CLEAN
    LOAD_WAIT_PERSIST(CH4_ENEMY_UNITS)
    FADE_IN_SCREEN(16)
    MUSC(BGM_TENSION)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    Text(Chapter_04_Scene_02_Convo_01)
    FADE_OUT_SCREEN(16)
    MOVE_CAMERA_TO_POSITION(0, 10)
    FADE_IN_SCREEN(16)
    REDUCE_VOLUME
    HIGHLIGHT_COORDINATES(1, 11, 60)

    SetBackground(0x2)
    TEXTSTART
    TEXTSHOW(Chapter_04_Scene_03_Convo_01)
    TEXTEND

    RESTORE_VOLUME
    FADE_OUT_SCREEN(16)
    CLEAN
    MOVE_CAMERA_TO_POSITION(0, 0)
    REMA
    FADE_IN_SCREEN(16)
    LOAD_WAIT_PERSIST(CH4_ARTUR)
    HIGHLIGHT_CHARACTER(CHARACTER_ARTUR, 60)
    Text(Chapter_04_Scene_04_Convo_01)
    MOVE_POSITION_WAIT(24, 11, 6, 9, 3)
    HIGHLIGHT_CHARACTER(CHARACTER_ARTUR, 60)
    Text(Chapter_04_Scene_04_Convo_02)
    // Make enemy the active unit again
    MOVE_POSITION_WAIT(24, 9, 3, 9, 3)
    // Battle scene
    StartBattle
    NormalDamage(1, 0) // Artur attacks
    NormalDamage(0, 0) // Enemy attacks 
    CriticalHit(1, 20) // Artur criticals for 20 damage
    EndAttack
    FIGHT(CHAR_EVT_ACTIVE_UNIT, CHARACTER_ARTUR, 0, false)
    KILL(CHAR_EVT_ACTIVE_UNIT)
    ERASE(CHAR_EVT_ACTIVE_UNIT)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    Text(Chapter_04_Scene_04_Convo_03)
    CALL(EventScr_08591FD8) // Prep screen
    // MOVE_CAMERA_TO_POSITION(0, 0)
    // FADE_IN_SCREEN(16)
    // MUSC(BGM_DISTANT_ROADS)
    // HIGHLIGHT_CHARACTER(CHARACTER_ARTUR, 60)
    // Text(Chapter_04_Scene_05_Convo_01)
    NoFade
    ENDA
};

static const EventScr EventScr_Ending[] = {
//     MUSC(BGM_VICTORY)
// /* If Lute wasn't recruited, then have her join */
//     CHECK_EVENTID(EVFLAG_TMP(0x8))
// 	SVAL(EVT_SLOT_0, 0x1)
// 	BEQ(0x1, EVT_SLOT_C, EVT_SLOT_0)
//     GOTO(0x2)
// LABEL(0x1)
//     TEXT_BG_HIDE_MAP(0x2, Chapter_04_Scene_06_Convo_01)
//     LOAD_WAIT_PERSIST(CH4_LUTE)
//     GOTO(0x2)
// LABEL(0x2)
//     MUSC(BGM_INTO_THE_SHADOWS_OF_VICTORY)
//     TEXT_BG_HIDE_MAP(0x18, Chapter_04_Scene_07_Convo_01)
//     TEXT_BG_HIDE_MAP(0x18, Chapter_04_Scene_08_Convo_01)
//     FADE_OUT_SCREEN(16)
//     MUSC(BGM_NULL)
//     CLEAN
//     MOVE_CAMERA_TO_POSITION(7, 7)
//     CLEAR_ALL_UNITS
//     LOAD_WAIT(CH4_EIRIKA_SETH)
//     FADE_IN_SCREEN(16)
//     LOAD_WAIT(CH4_LARACHEL)
//     MUSC(BGM_LAUGHTER)
//     HIGHLIGHT(60, CHARACTER_LARACHEL)
//     TEXT_BG_HIDE_MAP(0x18, Chapter_04_Scene_09_Convo_01)
//     TEXT_BG_HIDE_MAP(0x19, Chapter_04_Scene_10_Convo_01)
    NEXT_CHAPTER_WITH_MAP(0x5) // Chapter 5 - The Empire's Reach
    ENDA
};

/**
 * Misc events
 */
// static const EventListScr EventScr_COLM_APPEARS[] = {
//     CHECK_TURNS //Store current turn count in slot C
//     SVAL(EVT_SLOT_7, 1)
//     BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
//     MOVE_CAMERA_TO_POSITION(0, 0)
//     STAL(15)
//     LOAD_WAIT_PERSIST(CH3_COLM)
//     MUSC(BGM_BINDING_VOW)
//     HIGHLIGHT_CHARACTER(CHARACTER_COLM, 60)
//     Text(Chapter_03_Colm_Appears)
//     MOVE_CLOSEST_ENUN(0, CHARACTER_COLM, 2, 4)
//     GOTO(0x1)

// LABEL(0x0)
//     CHECK_EVENTID_
//     SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
//     ENUF_SLOT2

// LABEL(0x1)
//     NoFade
//     ENDA
// };

// static const EventListScr EventScr_ROSS_APPEARS[] = {
//     CHECK_TURNS //Store current turn count in slot C
//     SVAL(EVT_SLOT_7, 3)
//     BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
//     STAL(15)
//     MOVE_CAMERA_TO_POSITION(3, 11)
//     LOAD_WAIT_PERSIST(CH3_ROSS)
//     MUSC(BGM_BINDING_VOW)
//     HIGHLIGHT_CHARACTER(CHARACTER_ROSS, 30)
//     Text(Chapter_03_Ross_Appears)
//     GOTO(0x1)

// LABEL(0x0)
//     CHECK_EVENTID_
//     SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
//     ENUF_SLOT2

// LABEL(0x1)
//     NoFade
//     ENDA
// };

// static const EventListScr EventScr_Talk_ROSS_EIRIKA[] = {
//     CHANGE_MUSIC_SAVE_PREVIOUS_MUSIC(BGM_COMRADES)
//     STAL(33)
//     Text(Chapter_03_Ross_Eirika)
//     RESTORE_PREVIOUS_MUSIC
//     CHANGE_TO_BLUE(CHARACTER_ROSS)
//     NoFade
//     ENDA
// };

// static const EventListScr EventScr_Talk_COLM_NEIMI[] = {
//     CHANGE_MUSIC_SAVE_PREVIOUS_MUSIC(BGM_COMRADES)
//     STAL(33)
//     Text(Chapter_03_Colm_Neimi)
//     RESTORE_PREVIOUS_MUSIC
//     CHANGE_TO_BLUE(CHARACTER_COLM)
//     NoFade
//     ENDA
// };

/**
 * Event list
 */

static const EventListScr EventListScr_Turn[] = {
    // AFEV(EVFLAG_TMP(7), EventScr_COLM_APPEARS, 0)
    // AFEV(EVFLAG_TMP(8), EventScr_ROSS_APPEARS, 0)
    END_MAIN
};

static const EventListScr EventListScr_Character[] = {
    // CharacterEventBothWays(EVFLAG_TMP(9), EventScr_Talk_COLM_NEIMI, CHARACTER_COLM, CHARACTER_NEIMI)
    // CharacterEventBothWays(EVFLAG_TMP(10), EventScr_Talk_ROSS_EIRIKA, CHARACTER_ROSS, CHARACTER_EIRIKA)
    END_MAIN
};

static const EventListScr EventListScr_Location[] = {
    // Door(2, 3, EVFLAG_TMP(11))
    // Door(10, 5, EVFLAG_TMP(12))
    // Door(6, 10, EVFLAG_TMP(13))
    // Chest(ITEM_BOW_BEACONBOW, 6, 12)
    // Chest(ITEM_AXE_FIENDCLEAVER, 10, 3)
    // Chest(ITEM_LANCE_BRIGHTLANCE, 8, 3)
    // Chest(ITEM_SWORD_SHADOWKILLR, 6, 3)
    // Seize(14, 1)
    END_MAIN
};

static const EventListScr EventListScr_Misc[] = {
    DefeatAll(EventScr_Ending)
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

const struct ChapterEventGroup Chapter04Event = {
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
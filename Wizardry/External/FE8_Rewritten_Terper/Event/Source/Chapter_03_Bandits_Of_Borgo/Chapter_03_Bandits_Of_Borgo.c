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

/**
 * Ally unit and REDA definitions
 */
static const struct REDA REDAs_TANA[] = { 
    { .x = 2, .y = 13, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_EIRIKA[] = {
    { .x = 2, .y = 11, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_SETH[] = {
    { .x = 1, .y = 12, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_FRANZ[] = { 
    { .x = 0, .y = 11, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_GILLIAM[] = {
    { .x = 1, .y = 8, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_MOULDER[] = {
    { .x = 3, .y = 12, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_NEIMI[] = {
    { .x = 1, .y = 10, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH3_PLAYER_UNITS[] = {
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 0, 12, 1, REDAs_EIRIKA, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 0, 12, 1, REDAs_SETH, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_LANCE_SILVER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_FRANZ, CLASS_CAVALIER, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 0, 11, 1, REDAs_FRANZ, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_GILLIAM, CLASS_ARMOR_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 0, 10, 1, REDAs_GILLIAM, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_MOULDER, CLASS_PRIEST, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 3, 0, 12, 1, REDAs_MOULDER, 0, 0, 0, 0, ITEM_STAFF_HEAL, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_TANA, CLASS_PEGASUS_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 0, 0, 12, 1, REDAs_TANA, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_NEIMI, CLASS_ARCHER_F, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 0, 10, 1, REDAs_NEIMI, 0, 0, 0, 0, ITEM_BOW_IRON, ITEM_VULNERARY),
    {},
};

/**
 * Enemy unit and REDA definitions
 */

static const struct REDA REDAs_BAZBA_ENEMY[] = {
    { .x = 14, .y = 1, .b = -1, } 
};
static const struct REDA REDAs_BRIGAND1_ENEMY[] = {
    { .x = 7, .y = 2, .b = -1, }
};
static const struct REDA REDAs_BRIGAND2_ENEMY[] = { 
    { .x = 9, .y = 8, .b = -1, }
};
static const struct REDA REDAs_BRIGAND3_ENEMY[] = { 
    { .x = 14, .y = 6, .b = -1, } 
};
static const struct REDA REDAs_BRIGAND4_ENEMY[] = {
    { .x = 14, .y = 11,  .b = -1, }
};
static const struct REDA REDAs_BRIGAND5_ENEMY[] = { 
    { .x = 5, .y = 9, .b = -1, } 
};
static const struct REDA REDAs_BRIGAND6_ENEMY[] = { 
    { .x = 7, .y = 11, .b = -1, } 
};
static const struct REDA REDAs_ARCHER1_ENEMY[] = {
    { .x = 12, .y = 9, .b = -1, }
};
static const struct REDA REDAs_THIEF1_ENEMY[] = {
    { .x = 9, .y = 11, .b = -1, }
};
static const struct REDA REDAs_MERCENARY1_ENEMY[] = {
    { .x = 15, .y = 2, .b = -1, }
};


static const struct REDA REDAs_VIGARDE_ENEMY[] = {
    { .x = 14, .y = 2, .b = -1, }
};
static const struct REDA REDAs_LYON_ENEMY[] = {
    { .x = 15, .y = 3, .b = -1, }
};
static const struct REDA REDAs_DUESSEL_ENEMY[] = {
    { .x = 14, .y = 5, .b = -1, }
};
static const struct REDA REDAs_GLEN_ENEMY[] = {
    { .x = 13, .y = 5, .b = -1, }
};
static const struct REDA REDAs_SELENA_ENEMY[] = {
    { .x = 15, .y = 5, .b = -1, }
};


static const struct REDA REDAs_RIEV_ENEMY[] = {
    { .x = 14, .y = 5, .b = -1, }
};
static const struct REDA REDAs_CAELLACH_ENEMY[] = {
    { .x = 15, .y = 5, .b = -1, }
};
static const struct REDA REDAs_VALTER_ENEMY[] = {
    { .x = 13, .y = 5, .b = -1, }
};

static const struct UnitDefinition CH3_BANDITS[] = {
    UNIT_ENTRY(CHARACTER_BAZBA, CLASS_BRIGAND, FACTION_ID_RED,  NO_ITEM_DROP, NO_AUTOLEVEL, 6, 14, 1, 1, REDAs_BAZBA_ENEMY, 3, 3, 9, 20, ITEM_AXE_STEEL, ITEM_AXE_HANDAXE), 
    UNIT_ENTRY(CHARACTER_CUTTHROAT_8E, CLASS_BRIGAND, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 7, 2, 1, REDAs_BRIGAND1_ENEMY, 0, 3, 9, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_CUTTHROAT_8E, CLASS_BRIGAND, FACTION_ID_RED,  NO_ITEM_DROP, AUTOLEVEL, 2, 9, 8, 1, REDAs_BRIGAND2_ENEMY, 0, 3, 9, 0, ITEM_AXE_HANDAXE),
    UNIT_ENTRY(CHARACTER_CUTTHROAT_8E, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 15, 2, 1, REDAs_MERCENARY1_ENEMY, 0, 12, 9, 0, ITEM_AXE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_CUTTHROAT_8E, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 4, 12, 9, 1, REDAs_ARCHER1_ENEMY, 0, 3, 9, 0, ITEM_BOW_IRON),
    UNIT_ENTRY(CHARACTER_CUTTHROAT_8E, CLASS_BRIGAND, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 14, 6, 1, REDAs_BRIGAND3_ENEMY, 0, 3, 9, 0, ITEM_AXE_IRON, ITEM_PUREWATER),
    UNIT_ENTRY(CHARACTER_CUTTHROAT_8E, CLASS_BRIGAND, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 9, 11, 1, REDAs_THIEF1_ENEMY, 0, 3, 9, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_CUTTHROAT_8E, CLASS_BRIGAND, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 4, 14, 11, 1, REDAs_BRIGAND4_ENEMY, 0, 3, 9, 0, ITEM_AXE_HANDAXE),
    UNIT_ENTRY(CHARACTER_CUTTHROAT_8E, CLASS_BRIGAND, FACTION_ID_RED, ITEM_DROP, AUTOLEVEL, 3, 5, 9, 1, REDAs_BRIGAND5_ENEMY, 0, 3, 9, 0, ITEM_AXE_IRON, ITEM_DOORKEY),
    UNIT_ENTRY(CHARACTER_CUTTHROAT_8E, CLASS_BRIGAND, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 7, 11, 1, REDAs_BRIGAND6_ENEMY, 0, 3, 9, 0, ITEM_AXE_IRON, ITEM_DOORKEY),
    {}
};

static const struct UnitDefinition CH3_GRADO_1[] = {
    UNIT_ENTRY(CHARACTER_VIGARDE, CLASS_GENERAL, FACTION_ID_RED,  NO_ITEM_DROP, NO_AUTOLEVEL, 6, 14, 2, 1, REDAs_VIGARDE_ENEMY, 3, 3, 9, 20, ITEM_AXE_STEEL, ITEM_AXE_HANDAXE), 
    UNIT_ENTRY(CHARACTER_LYON, CLASS_NECROMANCER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 15, 3, 1, REDAs_LYON_ENEMY, 0, 3, 9, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_DUESSEL, CLASS_GREAT_KNIGHT, FACTION_ID_RED,  NO_ITEM_DROP, AUTOLEVEL, 2, 14, 5, 1, REDAs_DUESSEL_ENEMY, 0, 3, 9, 0, ITEM_AXE_HANDAXE),
    UNIT_ENTRY(CHARACTER_GLEN, CLASS_WYVERN_LORD, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 13, 5, 1, REDAs_GLEN_ENEMY, 0, 12, 9, 0, ITEM_AXE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SELENA, CLASS_MAGE_KNIGHT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 4, 15, 5, 1, REDAs_SELENA_ENEMY, 0, 3, 9, 0, ITEM_BOW_IRON),
    {}
};

static const struct UnitDefinition CH3_GRADO_2[] = {
    UNIT_ENTRY(CHARACTER_RIEV, CLASS_BISHOP, FACTION_ID_RED,  NO_ITEM_DROP, NO_AUTOLEVEL, 6, 14, 10, 1, REDAs_RIEV_ENEMY, 3, 3, 9, 20, ITEM_AXE_STEEL, ITEM_AXE_HANDAXE), 
    UNIT_ENTRY(CHARACTER_CAELLACH, CLASS_HERO, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 15, 10, 1, REDAs_CAELLACH_ENEMY, 0, 3, 9, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_VALTER, CLASS_WYVERN_KNIGHT, FACTION_ID_RED,  NO_ITEM_DROP, AUTOLEVEL, 2, 13, 10, 1, REDAs_VALTER_ENEMY, 0, 3, 9, 0, ITEM_AXE_HANDAXE),
    {}
};

/**
 * Green unit and REDA definitions
 */

static const struct REDA REDAs_COLM[] = {
    { .x = 0, .y = 5, .b = -1 },
};
static const struct REDA REDAs_ROSS[] = {
    { .x = 3, .y = 15, .b = -1 }, 
};

static const struct UnitDefinition CH3_COLM[] = {
    UNIT_ENTRY(CHARACTER_COLM, CLASS_THIEF, FACTION_ID_GREEN, NO_ITEM_DROP, NO_AUTOLEVEL, 2, 0, 5, 1, REDAs_COLM,  6, 5, 8, 1, ITEM_SWORD_IRON, ITEM_LOCKPICK, ITEM_VULNERARY),
    {}
};

static const struct UnitDefinition CH3_ROSS[] = {
    UNIT_ENTRY(CHARACTER_ROSS, CLASS_JOURNEYMAN, FACTION_ID_GREEN, NO_ITEM_DROP, NO_AUTOLEVEL, 1, 3, 15, 1, REDAs_ROSS,  0, 0xA, 0, 0, ITEM_AXE_HATCHET, ITEM_VULNERARY),
    {}
};


/**
 * Main events
 */
static const EventScr EventScr_Beginning[] = {
    MUSC(BGM_LEGACY)
    TEXT_BG_HIDE_MAP(0x18, Chapter_03_Scene_01_Convo_01)
    MUSC(BGM_TENSION)
    TEXT_BG_HIDE_MAP(0x1D, Chapter_03_Scene_02_Convo_01)
    FADE_OUT_SCREEN(16)
    CLEAN
    LOAD_WAIT_PERSIST(CH3_BANDITS)
    FADE_IN_SCREEN(16)
    LOAD_WAIT_PERSIST(CH3_PLAYER_UNITS)
    HIGHLIGHT_CHARACTER(CHARACTER_NEIMI, 60)
    Text(Chapter_03_Scene_03_Convo_01)
    MOVE_CAMERA_TO_UNIT(CHARACTER_BAZBA)
    MUSC(BGM_SHADOW_OF_THE_ENEMY)
    HIGHLIGHT_CHARACTER(CHARACTER_BAZBA, 60)
    Text(Chapter_03_Scene_03_Convo_02)
    NoFade
    ENDA
};

static const EventScr EventScr_Ending[] = {
    MUSC(BGM_VICTORY)
    TEXT_BG_HIDE_MAP(0x28, Chapter_03_Scene_04_Convo_01)
/* If Colm wasn't recruited, then have him join */
    CHECK_ALLEGIANCE(CHARACTER_COLM)
	SVAL(EVT_SLOT_1, 0x1)
	BEQ(0x1, EVT_SLOT_C, EVT_SLOT_1)
    GOTO(0x2)
LABEL(0x1)
    FADE_OUT_SCREEN(16)
    CHANGE_TO_BLUE(CHARACTER_COLM)
    GOTO(0x2)
LABEL(0x2)
    TEXT_BG_HIDE_MAP(0x29, Chapter_03_Scene_05_Convo_01)
    SOUN(MUTE)
    FADE_OUT_SCREEN(2)
    CLEAR_ALL_UNITS
    LOMA(OUTSIDE_GRADO_CASTLE)
    FADE_IN_SCREEN(16)
    BROWNBOXTEXT(BBT_Grado_Keep, 2, 2)
    HIGHLIGHT_COORDINATES(10, 4, 60)
    TEXT_BG_HIDE_MAP(0xC, Chapter_03_Scene_06_Convo_01)
    FADE_OUT_SCREEN(16)
    CLEAN
    LOMA(INSIDE_GRADO_CASTLE)
    MOVE_CAMERA_TO_POSITION_CENTER(14,2)
    LOAD_WAIT(CH3_GRADO_1)
    FADE_IN_SCREEN(16)
    MUSC(BGM_SOLVE_THE_RIDDLE)
    HIGHLIGHT_CHARACTER(CHARACTER_VIGARDE, 60)
    Text(Chapter_03_Scene_07_Convo_01)
    SOUN(SFX_UNLOCKING_DOOR)
    TILECHANGE_COORDINATES(14, 8)
    MOVE_TWICE_WAIT(36, CHARACTER_DUESSEL, 14, 4, 12, 4)
    MOVE_1STEP_WAIT(36, CHARACTER_GLEN, MOVE_LEFT)
    MOVE_1STEP_WAIT(36, CHARACTER_SELENA, MOVE_RIGHT)
    LOAD_WAIT(CH3_GRADO_2)
    HIGHLIGHT_CHARACTER(CHARACTER_VIGARDE, 60)
    Text(Chapter_03_Scene_07_Convo_02)
    FADE_OUT_SCREEN(16)
    NEXT_CHAPTER_WITH_MAP(0x4) // Chapter 4 - Ancient Horrors
    ENDA
};

/**
 * Misc events
 */
static const EventListScr EventScr_COLM_APPEARS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 1)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    MOVE_CAMERA_TO_POSITION_CENTER(0, 0)
    STAL(15)
    LOAD_WAIT_PERSIST(CH3_COLM)
    MUSC(BGM_BINDING_VOW)
    HIGHLIGHT_CHARACTER(CHARACTER_COLM, 60)
    Text(Chapter_03_Colm_Appears)
    MOVE_CLOSEST_ENUN(0, CHARACTER_COLM, 2, 4)
    GOTO(0x1)

LABEL(0x0)
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF_SLOT2

LABEL(0x1)
    NoFade
    ENDA
};

static const EventListScr EventScr_ROSS_APPEARS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 3)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    STAL(15)
    MOVE_CAMERA_TO_POSITION_CENTER(3, 11)
    LOAD_WAIT_PERSIST(CH3_ROSS)
    MUSC(BGM_BINDING_VOW)
    HIGHLIGHT_CHARACTER(CHARACTER_ROSS, 30)
    Text(Chapter_03_Ross_Appears)
    GOTO(0x1)

LABEL(0x0)
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF_SLOT2

LABEL(0x1)
    NoFade
    ENDA
};

static const EventListScr EventScr_Talk_ROSS_EIRIKA[] = {
    CHANGE_MUSIC_SAVE_PREVIOUS_MUSIC(BGM_COMRADES)
    STAL(33)
    Text(Chapter_03_Ross_Eirika)
    RESTORE_PREVIOUS_MUSIC
    CHANGE_TO_BLUE(CHARACTER_ROSS)
    NoFade
    ENDA
};

static const EventListScr EventScr_Talk_COLM_NEIMI[] = {
    CHANGE_MUSIC_SAVE_PREVIOUS_MUSIC(BGM_COMRADES)
    STAL(33)
    Text(Chapter_03_Colm_Neimi)
    RESTORE_PREVIOUS_MUSIC
    CHANGE_TO_BLUE(CHARACTER_COLM)
    NoFade
    ENDA
};

/**
 * Event list
 */

static const EventListScr EventListScr_Turn[] = {
    AFEV(EVFLAG_TMP(7), EventScr_COLM_APPEARS, 0)
    AFEV(EVFLAG_TMP(8), EventScr_ROSS_APPEARS, 0)
    END_MAIN
};

static const EventListScr EventListScr_Character[] = {
    CharacterEventBothWays(EVFLAG_TMP(9), EventScr_Talk_COLM_NEIMI, CHARACTER_COLM, CHARACTER_NEIMI)
    CharacterEventBothWays(EVFLAG_TMP(10), EventScr_Talk_ROSS_EIRIKA, CHARACTER_ROSS, CHARACTER_EIRIKA)
    END_MAIN
};

static const EventListScr EventListScr_Location[] = {
    Door(2, 3, EVFLAG_TMP(11))
    Door(10, 5, EVFLAG_TMP(12))
    Door(6, 10, EVFLAG_TMP(13))
    Chest(ITEM_BOW_BEACONBOW, 6, 12)
    Chest(ITEM_AXE_FIENDCLEAVER, 10, 3)
    Chest(ITEM_LANCE_BRIGHTLANCE, 8, 3)
    Chest(ITEM_SWORD_SHADOWKILLR, 6, 3)
    Seize(14, 1)
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

const struct ChapterEventGroup Chapter03Event = {
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
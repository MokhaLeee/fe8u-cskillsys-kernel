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
#include "../../../../LimitedShopStock/_src/LimitedShopStock.h"

/**
 * Ally unit and REDA definitions
 */
static const struct REDA REDAs_TANA[] = { 
    { .x = 7, .y = 4, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_EIRIKA[] = {
    { .x = 2, .y = 2, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_SETH[] = {
    { .x = 0, .y = 1, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_FRANZ[] = { 
    { .x = 1, .y = 1, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_GILLIAM[] = {
    { .x = 0, .y = 3, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_MOULDER[] = {
    { .x = 3, .y = 3, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH2_TANA[] = {
    UNIT_ENTRY(CHARACTER_TANA, CLASS_PEGASUS_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 0, 1, 1, REDAs_TANA, 0, 0, 0, 0, ITEM_LANCE_SLIM, ITEM_LANCE_JAVELIN, ITEM_VULNERARY),
    {},
};

static const struct UnitDefinition CH2_PLAYER_UNITS[] = {
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 1, 0, 1, REDAs_EIRIKA, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 0, 0, 1, REDAs_SETH, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_LANCE_SILVER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_FRANZ, CLASS_CAVALIER, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 1, 0, 1, REDAs_FRANZ, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_GILLIAM, CLASS_ARMOR_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 0, 0, 0, 1, REDAs_GILLIAM, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_MOULDER, CLASS_PRIEST, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 3, 1, 0, 1, REDAs_MOULDER, 0, 0, 0, 0, ITEM_STAFF_HEAL, ITEM_VULNERARY),
    {},
};

/**
 * Enemy unit and REDA definitions
 */

static const struct REDA REDAs_BONE_ENEMY[] = { 
    { .x = 13, .y = 7, .b = -1, } 
};
static const struct REDA REDAs_BRIGAND1_ENEMY[] = {
    { .x = 10, .y = 12, .b = -1, }
};
static const struct REDA REDAs_BRIGAND2_ENEMY[] = { 
    { .x = 6, .y = 11, .b = -1, }
};
static const struct REDA REDAs_BRIGAND3_ENEMY[] = { 
    { .x = 7, .y = 14, .b = -1, } 
};
static const struct REDA REDAs_BRIGAND4_ENEMY[] = {
    { .x = 12, .y = 5,  .b = -1, }
};
static const struct REDA REDAs_ARCHER1_ENEMY[] = {
    { .x = 14, .y = 8, .b = -1, }
};
static const struct REDA REDAs_BAZBA_ENEMY[] = {
    { .x = 11, .y = 7, .b = -1, } 
};

static const struct UnitDefinition CH2_BANDITS[] = {
    UNIT_ENTRY(CHARACTER_CUTTHROAT_8E, CLASS_BRIGAND, FACTION_ID_RED,  NO_ITEM_DROP, NO_AUTOLEVEL, 3, 9, 14, 1, REDAs_BRIGAND1_ENEMY, 0, 11, 9, 0, ITEM_AXE_IRON), 
    UNIT_ENTRY(CHARACTER_CUTTHROAT_8E, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 14, 9, 1, REDAs_ARCHER1_ENEMY, 1, 3, 9, 0, ITEM_BOW_IRON),
    UNIT_ENTRY(CHARACTER_BONE, CLASS_BRIGAND, FACTION_ID_RED,  NO_ITEM_DROP, NO_AUTOLEVEL, 2, 14, 8, 1, REDAs_BONE_ENEMY, 4, 4, 9, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_CUTTHROAT_8E, CLASS_BRIGAND, FACTION_ID_RED, ITEM_DROP, AUTOLEVEL, 3, 6, 10, 1, REDAs_BRIGAND2_ENEMY, 0, 12, 9, 0, ITEM_AXE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_CUTTHROAT_8E, CLASS_BRIGAND, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 7, 14, 1, REDAs_BRIGAND3_ENEMY, 0, 0, 9, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_CUTTHROAT_8E, CLASS_BRIGAND, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 14, 7, 1, REDAs_BRIGAND4_ENEMY, 0, 11, 9, 0, ITEM_AXE_IRON),
    {}
};

static const struct UnitDefinition CH2_BAZBA[] = {
    UNIT_ENTRY(CHARACTER_BAZBA, CLASS_BRIGAND, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 14, 8, 1, REDAs_BAZBA_ENEMY,  0, 4, 9, 0, ITEM_AXE_STEEL),
    {}
};

/**
 * Green unit and REDA definitions
 */

static const struct REDA REDAs_ROSS[] = {
    { .x = 10, .y = 5, .b = -1 },
};
static const struct REDA REDAs_GARCIA[] = {
    { .x = 10, .y = 4, .b = -1 }, 
};

static const struct UnitDefinition CH2_ROSS_GARCIA[] = {
    UNIT_ENTRY(CHARACTER_ROSS, CLASS_JOURNEYMAN, FACTION_ID_GREEN, NO_ITEM_DROP, NO_AUTOLEVEL, 1, 11, 2, 1, REDAs_ROSS,  0, 0xA, 0, 0, ITEM_AXE_HATCHET, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_GARCIA, CLASS_FIGHTER, FACTION_ID_GREEN, NO_ITEM_DROP, NO_AUTOLEVEL, 4, 11, 2, 1, REDAs_GARCIA, 0, 3, 0, 0, ITEM_AXE_IRON, ITEM_AXE_HANDAXE, ITEM_VULNERARY),
    {}
};

/**
 * Main events
 */
static const EventScr EventScr_Beginning[] = {
    MUSC(BGM_ADVANCE)
    TEXT_BG(0x18, Chapter_02_Scene_01_Convo_01)
    FADE_OUT_SCREEN(16)
    CLEAN
    FADE_IN_SCREEN(16)
    LOAD_WAIT_PERSIST(CH2_TANA)
    MUSCMID(MUTE)
    LOAD_WAIT_PERSIST(CH2_BANDITS)
    LOAD_WAIT_PERSIST(CH2_BAZBA)
    STAL(60)
    MUSC(BGM_DEFENSE)
    HIGHLIGHT_CHARACTER(CHARACTER_BONE, 60)
    TEXT(Chapter_02_Scene_02_Convo_01)
    MOVE_WAIT(24, CHARACTER_BAZBA, 14, 9)
    ERASE(CHARACTER_BAZBA)
    HIGHLIGHT_COORDINATES(12, 3, 60)
    MUSC(BGM_TENSION)
    TEXT_BG(0x2, Chapter_02_Scene_03_Convo_01)
    HIGHLIGHT_CHARACTER(CHARACTER_BONE, 60)
    TEXT(Chapter_02_Scene_04_Convo_01)
    MOVE_WAIT(24, CHARACTER_BONE, 9, 14)
    MOVE_POSITION_WAIT(0, 12, 5, 12, 3)
    SOUN(SFX_VILLAGE_DESTROYED)
    TILECHANGE_COORDINATES(12, 2)
    SOUN(SFX_WEAPON_BROKE)
    NOTIFY(NOTIFY_village_was_destroyed, SFX_NULL)
    LOAD_WAIT_PERSIST(CH2_ROSS_GARCIA)
    SET_UNIT_HP(CHARACTER_ROSS, 5)
    HIGHLIGHT_CHARACTER(CHARACTER_ROSS, 60)
    TEXT(Chapter_02_Scene_05_Convo_01)
    LOAD_WAIT_PERSIST(CH2_PLAYER_UNITS)
    HIGHLIGHT_CHARACTER(CHARACTER_TANA, 60)
    TEXT(Chapter_02_Scene_06_Convo_01)
    MOVE_WAIT(24, CHARACTER_TANA, 2, 3)
    HIGHLIGHT_CHARACTER(CHARACTER_TANA, 60)
    TEXT(Chapter_02_Scene_06_Convo_02)
    HIGHLIGHT_CHARACTER(CHARACTER_MOULDER, 60)
    TEXT(Chapter_02_Scene_06_Convo_03)
    NOFADE
    // ASMC(InitShopStock) // Look in External/LimitedShopStock/Installer.event for details
    ENDA
};

static const EventScr EventScr_Ending[] = {
    MUSC(BGM_VICTORY)
    TEXT_BG_HIDE_MAP(0x1D, Chapter_02_Scene_07_Convo_01)
    FADE_OUT_SCREEN(16)
    TEXT_BG_HIDE_MAP(0x4, Chapter_02_Scene_08_Convo_01)
    FADE_OUT_SCREEN(16)
    MUSCSLOW(MUTE)
    SET_BACKGROUND(0x20)
    TEXT(Chapter_02_Scene_09_Convo_01)
    FADE_TO_WHITE(4)
    SET_BACKGROUND(0x16)
    MUSC(BGM_BIRDS_CHIRPING)
    BROWNBOXTEXT(THREE_YEARS_AGO, 2, 2)
    TEXT(Chapter_02_Scene_10_Convo_01)
    FADE_TO_WHITE(4)
    TEXT_BG_HIDE_MAP(0x20, Chapter_02_Scene_11_Convo_01)
    FADE_OUT_SCREEN(16)
    TEXT_BG_HIDE_MAP(0x20, Chapter_02_Scene_12_Convo_01)
    FADE_OUT_SCREEN(16)
    NEXT_CHAPTER_WITH_MAP(0x3) // Chapter 3 - Bandits of Borgo
    ENDA
};

/**
 * Misc events
 */
static const EventListScr EventScr_Talk_ROSS_EIRIKA[] = {
    CHANGE_MUSIC_SAVE_PREVIOUS_MUSIC(BGM_COMRADES)
    STAL(33)
    TEXT(Chapter_02_Ross_Eirika)
    RESTORE_PREVIOUS_MUSIC
    CHANGE_TO_BLUE(CHARACTER_ROSS)
    NOFADE
    ENDA
};

static const EventListScr EventScr_Talk_ROSS_GARCIA[] = {
    CHANGE_MUSIC_SAVE_PREVIOUS_MUSIC(BGM_COMRADES)
    STAL(33)
    ConvoEvent(Chapter_02_Ross_Garcia)
    RESTORE_PREVIOUS_MUSIC
    NOFADE
    ENDA
};

static const EventListScr EventListScr_VILLAGE_1_TALK[] = {
    CHECK_ACTIVE
    SVAL(EVT_SLOT_1, CHARACTER_EIRIKA)
    BEQ(0x0, EVT_SLOT_C, EVT_SLOT_1)
    GOTO(0x1)

LABEL(0x0)
    HOUSE_EVENT_NO_END(0x2, Chapter_02_Village_01_Eirika)
    GOTO(0x2)

LABEL(0x1)
    HOUSE_EVENT_NO_END(0x2, Chapter_02_Village_01_Generic)
    GOTO(0x3)

LABEL(0x2)
    CALL(EventScr_RemoveBGIfNeeded) // This is vital, the game crashes without it for this event
    GIVE_SKILL_SCROLL_TO(SID_GoldDigger, CHARACTER_EVT_ACTIVE)
    GOTO(0x4)

LABEL(0x3)
    CALL(EventScr_RemoveBGIfNeeded) // This is vital, the game crashes without it for this event
    GIVE_ITEM_TO(ITEM_REDGEM, CHARACTER_EVT_ACTIVE)
    GOTO(0x4)

LABEL(0x4)
    NOFADE
    ENDA
};

static const EventListScr EventListScr_VILLAGE_2_TALK[] = {
    HOUSE_EVENT_NO_END(0x2, Chapter_02_Village_02)
    CALL(EventScr_RemoveBGIfNeeded)
    GIVE_ITEM_TO(ITEM_ELIXIR, CHARACTER_EVT_ACTIVE)
    NOFADE
    ENDA
};

static const EventListScr EventListScr_VILLAGE_3_TALK[] = {
    HOUSE_EVENT_NO_END(0x2, Chapter_02_Village_03)
    CALL(EventScr_RemoveBGIfNeeded)
    GIVE_ITEM_TO(ITEM_ANTITOXIN, CHARACTER_EVT_ACTIVE)
    NOFADE
    ENDA
};

static const u16 ShopList_Event_WeaponShop[] = {
    ITEM_SWORD_SLIM,
    ITEM_SWORD_IRON,
    ITEM_LANCE_SLIM,
    ITEM_LANCE_IRON,
    ITEM_AXE_IRON,
};

/**
 * Event list
 */
static const EventListScr EventListScr_Turn[] = {
    END_MAIN
};

static const EventListScr EventListScr_Character[] = {
    CharacterEventBothWays(EVFLAG_TMP(7), EventScr_Talk_ROSS_EIRIKA, CHARACTER_ROSS, CHARACTER_EIRIKA)
    CharacterEventBothWays(EVFLAG_TMP(8), EventScr_Talk_ROSS_GARCIA, CHARACTER_ROSS, CHARACTER_GARCIA)
    END_MAIN
};

static const EventListScr EventListScr_Location[] = {
    VILLAGE(EVFLAG_TMP(9), EventListScr_VILLAGE_1_TALK, 1, 12)
    VILLAGE(EVFLAG_TMP(10), EventListScr_VILLAGE_2_TALK, 7, 2)
    VILLAGE(EVFLAG_TMP(11), EventListScr_VILLAGE_3_TALK, 4, 2)
    ARMORY(ShopList_Event_WeaponShop, 5, 7)
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

const struct ChapterEventGroup Chapter02Event = {
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
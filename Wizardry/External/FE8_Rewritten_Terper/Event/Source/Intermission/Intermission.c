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
static const struct REDA REDAs_NONE[] = { 
    { .x = 0, .y = 0, .b = 0x0000, .delayFrames = 0, },
};

static const struct UnitDefinition CH1_MOULDER_TANA[] = {
    UNIT_ENTRY(CHARACTER_MOULDER, CLASS_PRIEST, FACTION_ID_BLUE, NO_ITEM_DROP, NO_AUTOLEVEL, 1, 0, 0, 1, REDAs_NONE, 0, 0, 0, 0, ITEM_STAFF_HEAL, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_TANA, CLASS_PEGASUS_KNIGHT, FACTION_ID_BLUE, NO_ITEM_DROP, NO_AUTOLEVEL, 4, 0, 0, 1, REDAs_NONE, 0, 0, 0, 0, ITEM_LANCE_SLIM, ITEM_LANCE_JAVELIN, ITEM_VULNERARY),
    {},
};

/**
 * Main events
 */
static const EventScr EventScr_Beginning[] = {
    MUSC(BGM_VICTORY)
    SET_BACKGROUND(0xE)
    TEXTSTART
    TEXTSHOW(Chapter_01_Scene_11_Convo_01)
    TEXTEND
    SET_BACKGROUND(0x35)
    REMA
    MUSIC_FADEOUT(MUTE)
    TEXTCONT
    TEXTEND
    MUSC(BGM_INTO_THE_SHADOWS_OF_VICTORY)
    TEXTCONT
    FADE_OUT_SCREEN(8)
    TEXTEND
    FADE_IN_SCREEN(8)
    SET_BACKGROUND(0xE)
    MUSC(BGM_SORROW)
    TEXTSTART
    TEXTSHOW(Chapter_01_Scene_11_Convo_02)
    TEXTEND
    SET_BACKGROUND(0x35)
    MUSIC_FADEOUT(MUTE)
    FADE_OUT_SCREEN(8)
    MUSC(BGM_INTO_THE_SHADOWS_OF_VICTORY)
    FADE_IN_SCREEN(8)
    SET_BACKGROUND(0xE)
    TEXTSTART
    TEXTSHOW(Chapter_01_Scene_11_Convo_03)
    TEXTEND
    REMA
    GIVE_GOLD(5000)
    GIVE_SKILL_SCROLL_TO(SID_Supply, CHARACTER_EIRIKA)
    REMA
    FADE_OUT_SCREEN(16)
    LOAD_WAIT_PERSIST(CH1_MOULDER_TANA)
    NEXT_CHAPTER_WITH_MAP(CHAPTER_02)
    ENDA
};

static const EventScr EventScr_Ending[] = {
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
    END_MAIN
};

static const EventListScr EventListScr_Misc[] = {
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

const struct ChapterEventGroup IntermissionEvent = {
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

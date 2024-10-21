#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

/**
 * Udefs
 */
static const struct REDA REDA_Eirika[] = {
    {
        .x = 10,
        .y = 5,
        .b = -1,
        .delayFrames = 32,
    }
};

static const struct REDA REDA_Seth[] = {
    {
        .x = 9,
        .y = 5,
        .b = -1,
        .delayFrames = 8,
    }
};

static const struct UnitDefinition UnitDef_Ally1[] = {
    {
        .charIndex = CHARACTER_EIRIKA,
        .classIndex = CLASS_EIRIKA_LORD,
        .allegiance = FACTION_ID_BLUE,
        .autolevel = true,
        .level = 1,
        .xPosition = 9,
        .yPosition = 5,
        .redaCount = 1,
        .redas = REDA_Eirika,
        .items = {
            ITEM_SWORD_RAPIER,
            ITEM_VULNERARY,
        },
    },
    {
        .charIndex = CHARACTER_SETH,
        .classIndex = CLASS_PALADIN,
        .autolevel = true,
        .level = 1,
        .xPosition = 8,
        .yPosition = 5,
        .redaCount = 1,
        .redas = REDA_Seth,
        .items = {
            ITEM_SWORD_STEEL,
            ITEM_LANCE_SILVER,
        },
    },
    {}
};

static const struct REDA REDA_ONeill[] = {
    {
        .x = 4,
        .y = 3,
        .b = -1,
    }
};

static const struct UnitDefinition UnitDef_Enemy1[] = {
    {
        .charIndex = CHARACTER_ONEILL,
        .classIndex = CLASS_FIGHTER,
        .autolevel = true,
        .allegiance = FACTION_ID_RED,
        .level = 4,
        .xPosition = 3,
        .yPosition = 2,
        .redaCount = 1,
        .redas = REDA_ONeill,
        .items = {
            ITEM_AXE_IRON,
        },
        .ai = { 0, 4, 9, 0 },
    },
    {
        .charIndex = CHARACTER_SOLDIER_83,
        .classIndex = CLASS_FIGHTER,
        .autolevel = true,
        .allegiance = FACTION_ID_RED,
        .level = 4,
        .xPosition = 1,
        .yPosition = 1,
        .redaCount = 1,
        .redas = REDA_ONeill,
        .items = {
            ITEM_AXE_IRON,
        },
        .ai = { 0, 4, 9, 0 },
    },
    {}
};

/**
 * Main events
 */
static const EventScr EventScr_Beginning[] = {
    LOAD1(0x1, UnitDef_Ally1)
    ENUN
    LOAD1(0x1, UnitDef_Enemy1)
    ENUN
    
    // Text(Chapter_00_Scene_01_Convo_01)
    // Text(Chapter_00_Scene_01_Convo_02)
    // Text(Chapter_00_Scene_01_Convo_03)
    // Text(Chapter_00_Scene_01_Convo_04)

    // Text(Chapter_00_Scene_02_Convo_01)
    // Text(Chapter_00_Scene_02_Convo_02)
    // Text(Chapter_00_Scene_02_Convo_03)
    // Text(Chapter_00_Scene_02_Convo_04)
    // Text(Chapter_00_Scene_02_Convo_05)

    // Text(Chapter_01_Scene_01_Convo_01)
    // Text(Chapter_01_Scene_02_Convo_01)
    // Text(Chapter_01_Scene_02_Convo_02)
    // Text(Chapter_01_Scene_03_Convo_01)
    // Text(Chapter_01_Scene_04_Convo_01)
    // Text(Chapter_01_Scene_05_Convo_01)
    // Text(Chapter_01_Scene_06_Convo_01)
    // Text(Chapter_01_Scene_06_Convo_02)
    // Text(Chapter_01_Scene_06_Convo_03)

    // Text(Chapter_02_Scene_01_Convo_01)
    // Text(Chapter_02_Scene_02_Convo_01)
    // Text(Chapter_02_Scene_03_Convo_01)
    // Text(Chapter_02_Scene_04_Convo_01)
    // Text(Chapter_02_Scene_05_Convo_01)
    // Text(Chapter_02_Scene_06_Convo_01)
    // Text(Chapter_02_Scene_06_Convo_02)
    // Text(Chapter_02_Scene_06_Convo_03)

    // Text(Chapter_02_Scene_07_Convo_01)
    // Text(Chapter_02_Scene_08_Convo_01)
    Text(Chapter_02_Scene_09_Convo_01)
    Text(Chapter_02_Scene_10_Convo_01)
    Text(Chapter_02_Scene_11_Convo_01)
    Text(Chapter_02_Scene_12_Convo_01)
    Text(Chapter_02_Scene_13_Convo_01)

    NoFade
    ENDA
};

static const EventScr EventScr_Ending[] = {
    MNC2(1)
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

static const EventListScr EventListScr_Character[] = {
    CharacterEventBothWays(EVFLAG_TMP(7), EventScr_Talk_EirikaSeth, CHARACTER_EIRIKA, CHARACTER_SETH)
    END_MAIN
};

static const EventListScr EventListScr_Location[] = {
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

const struct ChapterEventGroup ThisEvent = {
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

    .playerUnitsInNormal = UnitDef_Ally1,
    .playerUnitsInHard   = UnitDef_Ally1,

    .playerUnitsChoice1InEncounter = NULL,
    .playerUnitsChoice2InEncounter = NULL,
    .playerUnitsChoice3InEncounter = NULL,

    .enemyUnitsChoice1InEncounter = NULL,
    .enemyUnitsChoice2InEncounter = NULL,
    .enemyUnitsChoice3InEncounter = NULL,

    .beginningSceneEvents = EventScr_Beginning,
    .endingSceneEvents    = EventScr_Ending,
};

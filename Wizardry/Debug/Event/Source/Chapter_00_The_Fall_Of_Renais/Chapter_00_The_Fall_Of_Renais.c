#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "jester_headers/macros.h"
#include "jester_headers/soundtrack-ids.h"
#include "jester_headers/maps.h"

/**
 * Udefs
 */
// static const struct REDA REDA_Eirika[] = {
//     {
//         .x = 10,
//         .y = 5,
//         .b = -1,
//         .delayFrames = 32,
//     }
// };

// static const struct REDA REDA_Seth[] = {
//     {
//         .x = 9,
//         .y = 5,
//         .b = -1,
//         .delayFrames = 8,
//     }
// };

static const struct REDA REDA_Soldier[] = {
    {
        .x = 9,
        .y = 11,
        .b = -1,
        .delayFrames = 0,
    },
    {
        .x = 13,
        .y = 11,
        .b = -1,
        .delayFrames = 0,
    },
    {
        .x = 13,
        .y = 6,
        .b = -1,
        .delayFrames = 0,
    }
};

static const struct UnitDefinition Renais_Castle_Allies[] = {
    {
        .charIndex = CHARACTER_FADO,
        .classIndex = 0x73,
        .allegiance = FACTION_ID_BLUE,
        .autolevel = true,
        .level = 1,
        .xPosition = 13,
        .yPosition = 3,
    },
    {
        .charIndex = CHARACTER_EIRIKA,
        .classIndex = CLASS_EIRIKA_LORD,
        .allegiance = FACTION_ID_BLUE,
        .autolevel = true,
        .level = 1,
        .xPosition = 14,
        .yPosition = 4,
    },
    {
        .charIndex = CHARACTER_SETH,
        .classIndex = CLASS_PALADIN,
        .autolevel = true,
        .level = 1,
        .xPosition = 15,
        .yPosition = 4,
    },
    {
        .charIndex = CHARACTER_FRANZ,
        .classIndex = CLASS_CAVALIER,
        .autolevel = true,
        .level = 1,
        .xPosition = 11,
        .yPosition = 4,
    },
    {
        .charIndex = CHARACTER_MOULDER,
        .classIndex = CLASS_GENERAL,
        .autolevel = true,
        .level = 1,
        .xPosition = 11,
        .yPosition = 7,
    },
    {
        .charIndex = CHARACTER_VANESSA,
        .classIndex = CLASS_GENERAL,
        .autolevel = true,
        .level = 1,
        .xPosition = 15,
        .yPosition = 7,
    },
    {
        .charIndex = CHARACTER_GILLIAM,
        .classIndex = CLASS_ARMOR_KNIGHT,
        .autolevel = true,
        .level = 1,
        .xPosition = 7,
        .yPosition = 14,
    },
    {
        .charIndex = CHARACTER_TANA,
        .classIndex = CLASS_ARMOR_KNIGHT,
        .autolevel = true,
        .level = 1,
        .xPosition = 10,
        .yPosition = 14,
    },
    {}
};

static const struct UnitDefinition Panicked_Soldier_Allies[] = {
        {
        .charIndex = CHARACTER_EPHRAIM,
        .classIndex = CLASS_SOLDIER,
        .autolevel = true,
        .level = 1,
        .xPosition = 9,
        .yPosition = 15,
        .redaCount = 3,
        .redas = REDA_Soldier,
    },
};

static const struct REDA REDAs_Druid1_Enemy[] = {
    {
        .x = 7,
        .y = 4,
        .b = -1,
    }
};
static const struct REDA REDAs_Druid2_Enemy[] = {
    {
        .x = 6,
        .y = 6,
        .b = -1,
    }
};
static const struct REDA REDAs_Druid3_Enemy[] = {
    {
        .x = 19,
        .y = 3,
        .b = -1,
    }
};
static const struct REDA REDAs_Druid4_Enemy[] = {
    {
        .x = 20,
        .y = 6,
        .b = -1,
    }
};
static const struct REDA REDAs_GreatKnight1_Enemy[] = {
    {
        .x = 10,
        .y = 4,
        .b = -1,
    }
};
static const struct REDA REDAs_GreatKnight2_Enemy[] = {
    {
        .x = 16,
        .y = 4,
        .b = -1,
    }
};
static const struct REDA REDAs_Paladin1_Enemy[] = {
    {
        .x = 10,
        .y = 6,
        .b = -1,
    }
};
static const struct REDA REDAs_Paladin2_Enemy[] = {
    {
        .x = 16,
        .y = 6,
        .b = -1,
    }
};
static const struct REDA REDAs_MageKnight1_Enemy[] = {
    {
        .x = 10,
        .y = 8,
        .b = -1,
    }
};
static const struct REDA REDAs_MageKnight2_Enemy[] = {
    {
        .x = 16,
        .y = 8,
        .b = -1,
    }
};
static const struct REDA REDAs_Vigarde_Enemy[] = {
    {
        .x = 13,
        .y = 6,
        .b = -1,
    }
};
static const struct REDA REDAs_Lyon_Enemy[] = {
    {
        .x = 12,
        .y = 7,
        .b = -1,
    }
};

static const struct UnitDefinition Renais_Castle_Enemy1[] = {
    {
        .charIndex = CHARACTER_SOLDIER_80,
        .classIndex = CLASS_DRUID,
        .autolevel = true,
        .allegiance = FACTION_ID_RED,
        .level = 4,
        .xPosition = 5,
        .yPosition = 1,
        .redaCount = 1,
        .redas = REDAs_Druid1_Enemy,
        .ai = { 0, 4, 9, 0 },
    },
    {
        .charIndex = CHARACTER_SOLDIER_80,
        .classIndex = CLASS_DRUID,
        .autolevel = true,
        .allegiance = FACTION_ID_RED,
        .level = 4,
        .xPosition = 5,
        .yPosition = 2,
        .redaCount = 1,
        .redas = REDAs_Druid2_Enemy,
        .ai = { 0, 4, 9, 0 },
    },
    {
        .charIndex = CHARACTER_SOLDIER_80,
        .classIndex = CLASS_DRUID,
        .autolevel = true,
        .allegiance = FACTION_ID_RED,
        .level = 4,
        .xPosition = 19,
        .yPosition = 10,
        .redaCount = 1,
        .redas = REDAs_Druid3_Enemy,
        .ai = { 0, 4, 9, 0 },
    },
    {
        .charIndex = CHARACTER_SOLDIER_80,
        .classIndex = CLASS_DRUID,
        .autolevel = true,
        .allegiance = FACTION_ID_RED,
        .level = 4,
        .xPosition = 20,
        .yPosition = 10,
        .redaCount = 1,
        .redas = REDAs_Druid4_Enemy,
        .ai = { 0, 4, 9, 0 },
    },
    {}
};

static const struct UnitDefinition Renais_Castle_Enemy2[] = {
    {
        .charIndex = CHARACTER_SOLDIER_83,
        .classIndex = CLASS_GREAT_KNIGHT,
        .autolevel = true,
        .allegiance = FACTION_ID_RED,
        .level = 4,
        .xPosition = 5,
        .yPosition = 1,
        .redaCount = 1,
        .redas = REDAs_GreatKnight1_Enemy,
        .ai = { 0, 4, 9, 0 },
    },
    {
        .charIndex = CHARACTER_SOLDIER_83,
        .classIndex = CLASS_GREAT_KNIGHT,
        .autolevel = true,
        .allegiance = FACTION_ID_RED,
        .level = 4,
        .xPosition = 5,
        .yPosition = 2,
        .redaCount = 1,
        .redas = REDAs_GreatKnight2_Enemy,
        .ai = { 0, 4, 9, 0 },
    },
    {
        .charIndex = CHARACTER_SOLDIER_84,
        .classIndex = CLASS_PALADIN,
        .autolevel = true,
        .allegiance = FACTION_ID_RED,
        .level = 4,
        .xPosition = 19,
        .yPosition = 10,
        .redaCount = 1,
        .redas = REDAs_Paladin1_Enemy,
        .ai = { 0, 4, 9, 0 },
    },
    {
        .charIndex = CHARACTER_SOLDIER_84,
        .classIndex = CLASS_PALADIN,
        .autolevel = true,
        .allegiance = FACTION_ID_RED,
        .level = 4,
        .xPosition = 20,
        .yPosition = 10,
        .redaCount = 1,
        .redas = REDAs_Paladin2_Enemy,
        .ai = { 0, 4, 9, 0 },
    },
    {
        .charIndex = CHARACTER_SOLDIER_85,
        .classIndex = CLASS_MAGE_KNIGHT_F,
        .autolevel = true,
        .allegiance = FACTION_ID_RED,
        .level = 4,
        .xPosition = 10,
        .yPosition = 10,
        .redaCount = 1,
        .redas = REDAs_MageKnight1_Enemy,
        .ai = { 0, 4, 9, 0 },
    },
    {
        .charIndex = CHARACTER_SOLDIER_85,
        .classIndex = CLASS_MAGE_KNIGHT_F,
        .autolevel = true,
        .allegiance = FACTION_ID_RED,
        .level = 4,
        .xPosition = 16,
        .yPosition = 10,
        .redaCount = 1,
        .redas = REDAs_MageKnight2_Enemy,
        .ai = { 0, 4, 9, 0 },
    },
    {}
};

static const struct UnitDefinition Renais_Castle_Enemy3[] = {
    {
        .charIndex = CHARACTER_VIGARDE,
        .classIndex = CLASS_GENERAL,
        .autolevel = true,
        .allegiance = FACTION_ID_RED,
        .level = 4,
        .xPosition = 13,
        .yPosition = 10,
        .redaCount = 1,
        .redas = REDAs_Vigarde_Enemy,
        .ai = { 0, 4, 9, 0 },
    },
    {
        .charIndex = CHARACTER_LYON,
        .classIndex = CLASS_NECROMANCER,
        .autolevel = true,
        .allegiance = FACTION_ID_GREEN,
        .level = 4,
        .xPosition = 12,
        .yPosition = 11,
        .redaCount = 1,
        .redas = REDAs_Lyon_Enemy,
        .ai = { 0, 4, 9, 0 },
    },
    {}
};

/**
 * Main events
 */
static const EventScr EventScr_Beginning[] = {
    LOAD1(0x1, Renais_Castle_Allies) // Load units and move them if possible
    ENUN // Wait for units to finish moving before continuing
    LOAD_MAP_XY(17, 12, CHAPTER_16) // Load coordinates and the map for this chapter
    FADU(16) // Fade in from black in 1 second
    MUSC(BGM_RAID) // Set BGM to RAID
    BROWNBOXTEXT(BB_Renais_Castle, 2, 2) // Set location text pop up
    LOAD1(0x1, Panicked_Soldier_Allies) // Load units and move them if possible
    ENUN // Wait for units to finish moving before continuing
    CAMERA(17, 2) // Set camera to these coordinates
    CUMO(CHARACTER_EPHRAIM, 60) // Set flashing cursor on unit for a number of frames
    Text(Chapter_00_Scene_01_Convo_01) // Play this dialogue
    MOVE_WAIT(0, CHARACTER_EPHRAIM, 13, 11) // Move unit to chosen coordinates at given speed then wait
    ERASE(CHARACTER_EPHRAIM) // Erase given unit
    MOVE_1STEP_WAIT(0, CHARACTER_EIRIKA, MOVE_LEFT) // Move unit 1 step in given direction and wait
    CUMO(CHARACTER_EIRIKA, 60) // Set flashing cursor on unit for a number of frames
    Text(Chapter_00_Scene_01_Convo_02)
    MOVEONTO_WAIT(0, CHARACTER_SETH, CHARACTER_EIRIKA) // Move unit to another unit and wait    
    ERASE(CHARACTER_EIRIKA) // Erase given unit
    CUMO(CHARACTER_SETH, 60) // Set flashing cursor on unit for a number of frames
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
    LOAD1(0x1, Renais_Castle_Enemy1) // Load units and move them if possible
    ENUN // Wait for units to finish moving before continuing
    LOAD1(0x1, Renais_Castle_Enemy2) // Load units and move them if possible
    ENUN // Wait for units to finish moving before 
    LOAD1(0x1, Renais_Castle_Enemy3) // Load units and move them if possible
    ENUN // Wait for units to finish moving before continuing
    CUMO(CHARACTER_FADO, 60) // Set flashing cursor on unit for a number of frames
    Text(Chapter_00_Scene_01_Convo_04)
    FADI(2) // Fade out to black in 8 seconds
    REMA // Remove the current map
    EVBIT_F(2) // Hook point for skipping
    CLEAR_ALL_UNITS // Clear all units currently deployed
    LOAD_MAP_XY(0, 0, 0x40) // Load coordinates and the map for this chapter
    FADU(16) // Fade back in from black at fastest speed
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

    // .playerUnitsInNormal = UnitDef_Ally1,
    // .playerUnitsInHard   = UnitDef_Ally1,

    .playerUnitsChoice1InEncounter = NULL,
    .playerUnitsChoice2InEncounter = NULL,
    .playerUnitsChoice3InEncounter = NULL,

    .enemyUnitsChoice1InEncounter = NULL,
    .enemyUnitsChoice2InEncounter = NULL,
    .enemyUnitsChoice3InEncounter = NULL,

    .beginningSceneEvents = EventScr_Beginning,
    .endingSceneEvents    = EventScr_Ending,
};

#include "common-chax.h"
#include "jester_headers/soundtrack-ids.h"
#include "constants/texts.h"

const struct ROMChapterData Chapter07 = {
    .map = {
       .obj1Id = 0x1, // Object type
       .obj2Id= 0,
       .paletteId = 0x2, // Palette (Plist)
       .tileConfigId = 0x3, // Tile Config (Plist)
       .mainLayerId = 0x25, // Map pointer. In blocks of 4 per chapter
       .objAnimId = 0x5, // Tile animation 1
       .paletteAnimId = 0,
       .changeLayerId = 0x26, // Tile changes
    },
    .initialFogLevel = 0,
    .hasPrepScreen = FALSE,
    .chapTitleId = 0x08, // Chapter 7 - Waterside Renvall
    .chapTitleIdInHectorStory = 0,
    .initialPosX = 8,   // Starting position of the camera
    .initialPosY = 16,
    .initialWeather = WEATHER_FINE,
    .battleTileSet = 6,
    .easyModeLevelMalus = 0,
    .difficultModeLevelBonus = 0,
    .normalModeLevelMalus = 1,
    .mapBgmIds = {
        [MAP_BGM_BLUE] = BGM_FOLLOW_ME,
        [MAP_BGM_RED] = BGM_SHADOW_OF_THE_ENEMY,
        [MAP_BGM_GREEN] = BGM_NULL,
        [MAP_BGM_BLUE_HECTOR] = 13,
        [MAP_BGM_RED_HECTOR] = 21,
        [MAP_BGM_GREEN_HECTOR] = 15,
        [MAP_BGM_BLUE_GREEN_ALT] = 65535,
        [MAP_BGM_RED_ALT] = 65535,
        [MAP_BGM_PROLOGUE_LYN] = 11,
        [MAP_BGM_PROLOGUE] = 65535,
        [MAP_BGM_PROLOGUE_HECTOR] = 65535,
    },
    .mapCrackedWallHeath = 50,
    .mapEventDataId = 0x27, // Event ID (Plist)
    .gmapEventId = 0xE, // Chapter Number
    .victorySongEnemyThreshold = BGM_THE_VALIANT,
    .fadeToBlack = TRUE,
    .statusObjectiveTextId = CC_SEIZE_THE_CASTLEGATE,
    .goalWindowTextId = 414,
    .goalWindowDataType = GOAL_TYPE_SEIZE,
    .goalWindowEndTurnNumber = 0,
    .protectCharacterIndex = 0,
    .destPosX = 255,
    .destPosY = 0,
    .unk91 = 29,
    .unk92 = 0,
    .unk93 = 0,
};
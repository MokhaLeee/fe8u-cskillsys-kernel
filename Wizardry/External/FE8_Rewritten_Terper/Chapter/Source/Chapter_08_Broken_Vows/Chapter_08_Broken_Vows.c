#include "common-chax.h"
#include "jester_headers/soundtrack-ids.h"
#include "constants/texts.h"

const struct ROMChapterData Chapter08 = {
    .map = {
       .obj1Id = 0x18, // Obect type
       .obj2Id= 0,
       .paletteId = 0x19, // Palette (Plist)
       .tileConfigId = 0x1A, // Tile Conig (Plist)
       .mainLayerId = 0x28, // Map pointer. In blocks of 4 per chapter
       .objAnimId = 0x1C, // Tile animation 1
       .paletteAnimId = 0,
       .changeLayerId = 0x29, // Tile changes
    },
    .initialFogLevel = 0,
    .hasPrepScreen = FALSE,
    .chapTitleId = 0x09, // Chapter 8 - Broken Vows
    .chapTitleIdInHectorStory = 0,
    .initialPosX = 17,   // Starting position of the camera
    .initialPosY = 17,
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
    .mapEventDataId = 0x2A, // Event ID (Plist)
    .gmapEventId = 0x10, // Chapter Number
    .victorySongEnemyThreshold = BGM_THE_VALIANT,
    .fadeToBlack = TRUE,
    .statusObjectiveTextId = CC_SEIZE_THE_THRONE,
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
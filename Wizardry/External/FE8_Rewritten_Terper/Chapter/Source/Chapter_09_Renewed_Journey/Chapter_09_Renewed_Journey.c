#include "common-chax.h"
#include "jester_headers/soundtrack-ids.h"
#include "constants/texts.h"

const struct ROMChapterData Chapter09 = {
    .map = {
       .obj1Id = 0xE, // Object type
       .obj2Id= 0,
       .paletteId = 0xF, // Palette (Plist)
       .tileConfigId = 0x10, // Tile Config (Plist)
       .mainLayerId = 0x2B, // Map pointer. In blocks of 4 per chapter
       .objAnimId = 0x12, // Tile animation 1
       .paletteAnimId = 0,
       .changeLayerId = 0x2C, // Tile changes
    },
    .initialFogLevel = 0,
    .hasPrepScreen = FALSE,
    .chapTitleId = 0x0A, // Chapter 9 - Renewed Journey
    .chapTitleIdInHectorStory = 0,
    .initialPosX = 20,   // Starting position of the camera
    .initialPosY = 4,
    .initialWeather = WEATHER_FINE,
    .battleTileSet = 6,
    .easyModeLevelMalus = 0,
    .difficultModeLevelBonus = 0,
    .normalModeLevelMalus = 1,
    .mapBgmIds = {
        [MAP_BGM_BLUE] = BGM_RISE_ABOVE,
        [MAP_BGM_RED] = BGM_ASSAULT,
        [MAP_BGM_GREEN] = BGM_BINDING_VOW,
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
    .mapEventDataId = 0x2D, // Event ID (Plist)
    .gmapEventId = 0x12, // Chapter Number
    .victorySongEnemyThreshold = BGM_THE_VALIANT,
    .fadeToBlack = TRUE,
    .statusObjectiveTextId = CC_DEFEAT_ALL_ENEMIES,
    .goalWindowTextId = 414,
    .goalWindowDataType = GOAL_TYPE_DEFEAT_ALL,
    .goalWindowEndTurnNumber = 0,
    .protectCharacterIndex = 0,
    .destPosX = 255,
    .destPosY = 0,
    .unk91 = 29,
    .unk92 = 0,
    .unk93 = 0,
};
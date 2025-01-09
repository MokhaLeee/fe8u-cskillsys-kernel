#include "common-chax.h"
#include "jester_headers/soundtrack-ids.h"
#include "constants/texts.h"

const struct ROMChapterData Chapter05x = {
    .map = {
       .obj1Id = 0x18,
       .obj2Id= 0,
       .paletteId = 0x19,
       .tileConfigId = 0x1A,
       .mainLayerId = 0x1B, // Map pointer. In blocks of 4 per chapter
       .objAnimId = 0x1C,
       .paletteAnimId = 0,
       .changeLayerId = 0x1D,
    },
    .initialFogLevel = 0,
    .hasPrepScreen = FALSE,
    .chapTitleId = 0x05, // Chapter 5x - Unbroken Heart
    .chapTitleIdInHectorStory = 0,
    .initialPosX = 1,   // Starting position of the camera
    .initialPosY = 0,
    .initialWeather = WEATHER_FINE,
    .battleTileSet = 6,
    .easyModeLevelMalus = 0,
    .difficultModeLevelBonus = 0,
    .normalModeLevelMalus = 1,
    .mapBgmIds = {
        [MAP_BGM_BLUE] = BGM_FOLLOW_ME,
        [MAP_BGM_RED] = BGM_SHADOW_OF_THE_ENEMY,
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
    .mapEventDataId = 0x1E,
    .gmapEventId = 0xA,
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
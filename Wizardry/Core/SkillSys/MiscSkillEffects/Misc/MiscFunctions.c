#include "common-chax.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"


//! FE8U: 0x080B1F64
LYN_REPLACE_CHECK(SetGameOption);
void SetGameOption(u8 index, u8 newValue) {

    switch (index) {
        case GAME_OPTION_ANIMATION:
            switch (newValue) {
                case 0:
                    gPlaySt.config.animationType = 0;
                    return;

                case 1:
                    gPlaySt.config.animationType = 3;
                    return;

                case 2:
                    gPlaySt.config.animationType = 1;
                    return;

                case 3:
                    gPlaySt.config.animationType = 2;
                    return;
            }

            // fallthrough

        case GAME_OPTION_TERRAIN:
            gPlaySt.config.disableTerrainDisplay = newValue;

            break;

        case GAME_OPTION_UNIT:
            gPlaySt.config.unitDisplayType = newValue;

            break;

        case GAME_OPTION_AUTOCURSOR:
            gPlaySt.config.autoCursor = newValue;

            break;

        case GAME_OPTION_TEXT_SPEED:
            gPlaySt.config.textSpeed = newValue;

            break;

        case GAME_OPTION_GAME_SPEED:
            gPlaySt.config.gameSpeed = newValue;

            break;

        case GAME_OPTION_MUSIC:
            gPlaySt.config.disableBgm = newValue;

            break;

        case GAME_OPTION_SOUND_EFFECTS:
            gPlaySt.config.disableSoundEffects = newValue;

            break;

        case GAME_OPTION_WINDOW_COLOR:
            gPlaySt.config.windowColor = newValue;

            break;

        case GAME_OPTION_COMBAT:
            gPlaySt.config.battleForecastType = newValue;

            break;

        case GAME_OPTION_SUBTITLE_HELP:
            gPlaySt.config.noSubtitleHelp = newValue;

            break;

        case GAME_OPTION_AUTOEND_TURNS:
            gPlaySt.config.disableAutoEndTurns = newValue;

            break;

        case GAME_OPTION_UNIT_COLOR:
            gPlaySt.config.unitColor = newValue;

            break;

        case GAME_OPTION_OBJECTIVE:
            gPlaySt.config.disableGoalDisplay = newValue;

            break;

        case GAME_OPTION_CONTROLLER:
            gPlaySt.config.controller = newValue;

            break;

        case GAME_OPTION_RANK_DISPLAY:
            gPlaySt.config.rankDisplay = newValue;

            break;

    }

    return;
}
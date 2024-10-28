#pragma once

#include "eventscript.h"

// #define TutText(text) "SVAL 0xB 0xFFFFFFFF; TUTORIALTEXTBOXSTART; TEXTSHOW text; TEXTEND; REMA" // Centered 
// #define TutText_B(text) "SVAL 0xB 0x00B0FFFF; TUTORIALTEXTBOXSTART; TEXTSHOW text; TEXTEND; REMA" // Bottom-Centered 
// #define TutText_M(text) "SVAL 0xB 0x280028; TUTORIALTEXTBOXSTART; TEXTSHOW text; TEXTEND; REMA" // Bottom-Centered 
// #define TutTextAutoFade(background,textID) "MUSI; SMOV 0x2 background; CALL $9EE2E8; TutText(textID); MUNO; CALL $9EE2C4"
//#define TutText(background,textID) "MUSI; SMOV 0x2 background; CALL $9EE2E8; TutText(textID); MUNO"

// Create a tutorial popup
#define TUTORIALTEXT(msg) \
    SVAL(0xB, 0xFFFFFFFF) \
    TUTORIALTEXTBOXSTART \
    TEXTSHOW(msg) \
    TEXTEND \
    REMA

// Focus the cursor on a character for a set number of frames then erase it
#define CUMO(character, frames) \
    CUMO_CHAR(character) \
    STAL(frames) \
    CURE

// Load a chapter map and set the camera position
#define LOAD_MAP_XY(x, y, chapterID) \
    LOMA(chapterID) \
    CAMERA(x, y)

// DISA with a more obvious name (takes character ID as an argument)
#define ERASE EvtRemoveUnit

// Move unit and wait for them to finish
#define MOVE_WAIT(speed, character, x, y) \
    MOVE(speed, character, x, y) \
    ENUN

// Move unit twice and wait

#define MOVE_TWICE_WAIT(speed, character, x1, y1, x2, y2) \
    MOVE(speed, character, x1, y1) \
    MOVE(speed, character, x2, y2) \
    ENUN

enum {
    MOVE_LEFT = 0,
    MOVE_RIGHT = 1,
    MOVE_DOWN = 2,
    MOVE_UP = 3,
};

#define MOVE_1STEP_WAIT(speed, character, direction) \
    MOVE_1STEP(speed, character, direction) \
    ENUN

// Move unit to another unit and wait for them to finish
#define MOVEONTO_WAIT(speed, unit_id, unit_target) \
    MOVEONTO(speed, unit_id, unit_target) \
    ENUN

// Remove all units
#define CLEAR_ALL_UNITS \
    CLEA \
    CLEN \
    CLEE

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
#define HIGHLIGHT(character, frames) \
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
    ENUN \
    MOVE(speed, character, x2, y2) \
    ENUN

// Move unit thrice and wait
#define MOVE_THRICE_WAIT(speed, character, x1, y1, x2, y2, x3, y3) \
    MOVE(speed, character, x1, y1) \
    ENUN \
    MOVE(speed, character, x2, y2) \
    ENUN \
    MOVE(speed, character, x3, y3) \
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

// Set the chosen unit's HP total
#define SET_UNIT_HP(character, hp) \
    SVAL(EVT_SLOT_1, hp) \
    SET_HP(character)

// Combine the wait and the loading to reduce tedium for loading units
// P.S. I have no idea what 'thing' is supposed to do
#define LOAD_WAIT(thing, loaded_units) \
    LOAD1(thing, loaded_units) \
    ENUN

// So apparently Nintendlord mixed these two up back in the day
// and nobody ever made macros to fix it???
#define FADE_IN_SCREEN(number) FADU(number)
#define FADE_OUT_SCREEN(number) FADI(number)

#define GIVE_ITEM_TO(item, character) \
    SVAL(EVT_SLOT_3, item) \
    GIVEITEMTO(character)

#define GIVE_SKILL_SCROLL_TO(skill_id, character) \
    SVAL(EVT_SLOT_3, (skill_id << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL) \
    GIVEITEMTO(character)


#define UNIT_ENTRY(charID, classID, allegianceID, levelAuto, lvl, xpos, ypos, rCount, rArray, ai1, ai2, ai3, ai4, ...) \
    { \
        .charIndex = charID, \
        .classIndex = classID, \
        .allegiance = allegianceID, \
        .autolevel = levelAuto, \
        .level = lvl, \
        .xPosition = xpos, \
        .yPosition = ypos, \
        .redaCount = rCount, \
        .redas = rArray, \
        .ai = { ai1, ai2, ai3, ai4 }, \
        .items = {  __VA_ARGS__ } \
    }

// Where you end up after pressing START
#define SKIP_POINT EVBIT_F(2)

// Reduce volume of BGM
#define REDUCE_VOLUME MUSI
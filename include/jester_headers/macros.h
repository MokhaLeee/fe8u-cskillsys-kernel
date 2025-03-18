#pragma once

#include "eventscript.h"
#include "./jester_headers/miscellaenous.h"
#include "EAstdlib.h"

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

#define TEXT_CHANGE_BGM(messageID, BGM_ID) \
    TEXTSHOW(messageID) \
    TEXTEND \
    MUSC(BGM_ID) \
    TEXTCONT \
    TEXTEND \
    REMA

#define TEXT_NO_REMA(messageID) \
    TEXTSTART \
    TEXTSHOW(messageID) \
    TEXTEND

// Focus the cursor on a character for a set number of frames then erase it
#define HIGHLIGHT_CHARACTER(character, frames) \
    CUMO_CHAR(character) \
    STAL(frames) \
    CURE

// Focus the cursor on a character for a set number of frames then erase it
#define HIGHLIGHT_COORDINATES(x, y, frames) \
    CUMO_AT(x, y) \
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
    CLEE \
    CLEN \
    SVAL(EVT_SLOT_B, 0x00000000)

// Remove all player units
#define CLEAR_PLAYER_UNITS \
    CLEA

// Remove all enemy units
# define CLEAR_ENEMY_UNITS \
    CLEE

// Set the chosen unit's HP total
#define SET_UNIT_HP(character, hp) \
    SVAL(EVT_SLOT_1, hp) \
    SET_HP(character)

// Protect these loaded units from CLEAR_ALL_UNITS
#define LOAD_WAIT_PERSIST(loaded_units) \
    LOAD1(0x1, loaded_units) \
    ENUN

// Units can be erased by CLEAR_ALL_UNITS
#define LOAD_WAIT(loaded_units) \
    LOAD2(0x1, loaded_units) \
    ENUN

#define LOAD_WAIT_DEPLOYMENT_ORDER(loaded_units) \
    LOAD3(0x1, loaded_units) \
    ENUN

// So apparently Nintendlord mixed these two up back in the day
// and nobody ever made macros to fix it???
#define FADE_TO_BLACK(number) FADU(number)
#define FADE_FROM_BLACK(number) FADI(number)

#define FADE_TO_WHITE(number) FAWI(number)
#define FADE_FROM_WHITE(number) FAWU(number)

#define GIVE_ITEM_TO(item, character) \
    SVAL(EVT_SLOT_3, item) \
    GIVEITEMTO(character)

#define GIVE_SKILL_SCROLL_TO(skill_id, character) \
    SVAL(EVT_SLOT_3, (skill_id << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL) \
    GIVEITEMTO(character)

/*
** Item drops are defined in the unitDefinition struct as u16 so I
** can't use enum, which is u32 by default. Easier to create seperate definitions
*/ 
#define NO_ITEM_DROP (0)
#define ITEM_DROP (1) // Only valid values for a 1-bit field


enum {
    NO_AUTOLEVEL = 0,
    AUTOLEVEL = 1,
};


#define UNIT_ENTRY(charID, classID, allegianceID, dropItem, levelAuto, lvl, xpos, ypos, rCount, rArray, ai1, ai2, ai3, ai4, ...) \
    { \
        .charIndex = charID, \
        .classIndex = classID, \
        .allegiance = allegianceID, \
        .itemDrop = dropItem, \
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

#define MAKE_CAMERA_FOLLOW_MOVING_UNITS EVBIT_T(9)
#define STOP_CAMERA_FOLLOW_MOVING_UNITS EVBIT_F(9)

// Reduce volume of BGM
#define REDUCE_VOLUME MUSI

// Go to next chapter
#define NEXT_CHAPTER_WITHOUT_MAP(Chapter_ID) MNC2(Chapter_ID)
#define NEXT_CHAPTER_WITH_MAP(Chapter_ID) MNCH(Chapter_ID)

// Set flag
#define SET_FLAG ENUT

#define MUSIC_FADEOUT(bgmID) MUSCMID(bgmID)

#define COORDS(xcoord,ycoord) (ycoord<<16)|xcoord

#define MOVE_POSITION_WAIT(speed, sourceX, sourceY, destX, destY) \
    SVAL(0xB, COORDS(sourceX, sourceY)) \
    MOVE(speed, 0xFFFE, destX, destY) \
    ENUN

#define HOUSE_EVENT(background, message) \
    MUSI \
    TEXT_BG(background, message) \
    MUNO \
    NOFADE \
    ENDA

#define HOUSE_EVENT_NO_END(background, message) \
    MUSI \
    TEXT_BG(background, message) \
    MUNO

#define REDUCE_VOLUME MUSI
#define RESTORE_VOLUME MUNO

#define QUINTESSANCE_STEALING \
    ASMC(StartQuintessenceStealEffect) \
    STAL(32) \
    ASMC(ArchiveCurrentPalettes) \
    SOUN(0x125) \
    ASMC(FE7J_EventE5_Sim_Type_A) \
    ASMC(FE7J_EventE5_Sim_Type_B) \
    ASMC(FE7J_EventE5_Sim_Type_A) \
    ASMC(FE7J_EventE5_Sim_Type_B) \
    ASMC(fe7j_sub_80121FC) \
    ASMC(QuintessenceFx_Goto_C) \
    STAL(16) \
    ASMC(EndQuintessenceStealEffect)

#define ESCAPE_TILE(characterID, EventListscr, x, y) \
    LOCA(characterID, EventListscr, x, y, TILE_COMMAND_VISIT) \

#define ESCAPE_LOGIC(x, y) \
    SET_ENDTURN(0xFFFF) \
    ERASE(0xFFFF) \
    SET_ACTIVE(0xFFFF) \
    SET_CURSOR(x, y) \
    CHECK_EVENTID_ \
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0) \
    ENUF_SLOT2 \
    CHECK_PLAYERS \
    SVAL(EVT_SLOT_7, 0) \
    BEQ(0x0, EVT_SLOT_C, EVT_SLOT_7) \
    NOFADE \
    ENDA \
LABEL(0x0) \
    ENUT(0x3) \
    CALL(EventScr_Ending)


#define GIVE_GOLD(gold) \
    SVAL(0x3, gold) \
    GIVEITEMTOMAIN(0)

#define ActiveUnit (-1)
// #define CoordsSlotB (-2)
// #define UnitInSlot2 (-3)


// #define TILECHANGE_ACTIVE_UNIT
//     TILECHANGE(ActiveUnit)

#define TILECHANGE_COORDINATES(x, y) \
    SVAL(0xB, COORDS(x, y)) \
    TILECHANGE(ActiveUnit)

#define CHANGE_TO_BLUE(unitID) EvtChangeFaction(unitID, FACTION_ID_BLUE)
#define CHANGE_TO_GREEN(unitID) EvtChangeFaction(unitID, FACTION_ID_GREEN)
#define CHANGE_TO_RED(unitID) EvtChangeFaction(unitID, FACTION_ID_RED)

#define CHANGE_MUSIC_SAVE_PREVIOUS_MUSIC(musicID) MUSS(musicID)
#define RESTORE_PREVIOUS_MUSIC MURE(0x2)

enum {
    BLACK_BACKGROUND = 0x35
};

#define TEXT_BG_HIDE_MAP(bg, msg) \
    SET_BACKGROUND(bg) \
    TEXTSTART \
    TEXTSHOW(msg) \
    TEXTEND \
    SET_BACKGROUND(BLACK_BACKGROUND) \
    REMA

#define MOVE_CAMERA_TO_UNIT_CENTER(unit) CAMERA2_CAHR(unit)
#define MOVE_CAMERA_TO_UNIT(unit) CAMERA_CAHR(unit)
#define MOVE_CAMERA_TO_POSITION_CENTER(x, y) CAMERA2(x, y)
#define MOVE_CAMERA_TO_POSITION(x, y) CAMERA(x, y)

#define MOVE_ONTO_LEADER(characterId) \
    SVAL(EVT_SLOT_2, characterId) \
    CHECK_ALIVE(-3) \
    BEQ(0x0, EVT_SLOT_C, EVT_SLOT_0) \
    CHECK_DEPLOYED(-3) \
    BEQ(0x0, EVT_SLOT_C, EVT_SLOT_0) \
    GOTO(0x63) \
LABEL(0x0) \
    MOVEONTO(0, -3, 0) \
    ENUN \
    REMU(-3) \
LABEL(0x63) \
    ENDA

#define MOVE_CLOSEST_ENUN(frames, unitID, x, y) \
    MOVE_CLOSEST(frames, unitID, x, y) \
    ENUN

#define CHECK_IF_TRAINEE_OR_LEVEL_CAPPED(characterId, level, class) \
    SVAL(EVT_SLOT_3, characterId) \
    SVAL(EVT_SLOT_4, level) \
    SVAL(EVT_SLOT_5, class) \
    ASMC(IsTraineeLevelCappedOrPromoted)

enum {
    Overworld_Eirika = 0x0002,
};

#define KILL_AT_COORDINATES_ENUN(x, y) \
    SVAL(EVT_SLOT_B, COORDS(x, y)) \
    KILL(CHAR_EVT_POSITION_AT_SLOTB) \
    DISA_IF(CHAR_EVT_POSITION_AT_SLOTB) \
    ENUN

/* 
** It's a bit broken right now. I can't set it to trigger automatically
** and if it doesn't see a unit in a column it'll crash after completing
** the animation
*/
#define UseArrowTrap(x, turnCountdown, turnInterval) \
    SVAL(EVT_SLOT_3, x) \
    SVAL(EVT_SLOT_4, turnCountdown) \
    SVAL(EVT_SLOT_5, turnInterval) \
    SVAL(EVT_SLOT_9, 99) \
    ASMC(AddArrowTrap)

#define BREAKSTONE_TARGET_UNIT \
    SVAL(EVT_SLOT_9, 99) \
    STARTFADE \
    EvtColorFadeSetup(0x0, 0x20, 8, 128, 128, 128) \
    STAL(30) \
    BREAKSTONE(CHAR_EVT_POSITION_AT_SLOTB) \
    EvtColorFadeSetup(0x0, 0x20, 4, 256, 256, 256) \
    NOFADE \
    ENDA

#define WARP_CHARACTER_TO_COORDINATES(characterID, x, y) \
    SVAL(EVT_SLOT_2, characterID) \
    MOVE_CLOSEST(0xffff, CHAR_EVT_SLOT2, x, y) \
    CALL(EventScr_UnitWarpIN)

#define WARP_CHARACTER_OUT(characterID) \
    SVAL(EVT_SLOT_2, characterID) \
    CALL(EventScr_UnitWarpOUT)

void AddTrapASMC(void);

#define ADD_TRAP_EVENT(x, y, trapID, terrainType) \
    SVAL(EVT_SLOT_2, (x|(y<<16))) \
    SVAL(EVT_SLOT_3, trapID) \
    SVAL(EVT_SLOT_4, terrainType) \
    ASMC(AddTrapASMC)

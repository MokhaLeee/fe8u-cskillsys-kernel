#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "jester_headers/macros.h"
#include "jester_headers/soundtrack-ids.h"
#include "jester_headers/maps.h"
#include "jester_headers/flags.h"
#include "jester_headers/miscellaenous.h"
#include "EAstdlib.h"

static const struct REDA REDAs_NULL[] = {
    { } 
};

/**
 * Ally unit and REDA definitions
 */
static const struct REDA REDAs_TANA[] = { 
    { .x = 4, .y = 2, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_EIRIKA[] = {
    { .x = 5, .y = 2, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_SETH[] = {
    { .x = 6, .y = 1, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_ROSS[] = { 
    { .x = 2, .y = 2, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_GILLIAM[] = {
    { .x = 3, .y = 3, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_MOULDER[] = {
    { .x = 5, .y = 1, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_NEIMI[] = {
    { .x = 3, .y = 1, .b = -1, .delayFrames = 0, },
};

static const struct REDA REDAs_COLM[] = {
    { .x = 4, .y = 0, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH4_PLAYER_UNITS[] = {
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 5, 2, 1, REDAs_EIRIKA, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 6, 0, 1, REDAs_SETH, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_LANCE_SILVER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_ROSS, CLASS_JOURNEYMAN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 2, 2, 0, 1, REDAs_ROSS, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_GILLIAM, CLASS_ARMOR_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 3, 0, 1, REDAs_GILLIAM, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_MOULDER, CLASS_PRIEST, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 5, 0, 1, REDAs_MOULDER, 0, 0, 0, 0, ITEM_STAFF_HEAL, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_TANA, CLASS_PEGASUS_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 0, 4, 0, 1, REDAs_TANA, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_NEIMI, CLASS_ARCHER_F, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 3, 0, 1, REDAs_NEIMI, 0, 0, 0, 0, ITEM_BOW_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_COLM, CLASS_THIEF, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 4, 0, 1, REDAs_COLM, 0, 0, 0, 0, ITEM_BOW_IRON, ITEM_VULNERARY),
    {},
};

static const struct REDA REDAs_ARTUR[] = {
    { .x = 8, .y = 6, .b = -1, .delayFrames = 0, },
    { .x = 8, .y = 3, .b = -1, .delayFrames = 0, },
    { .x = 7, .y = 3, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH4_ARTUR[] = {
    UNIT_ENTRY(CHARACTER_ARTUR, CLASS_MONK, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 2, 9, 10, 3, REDAs_ARTUR, 0, 0, 0, 0, ITEM_LIGHT_LIGHTNING, ITEM_VULNERARY),
    {},
};

static const struct REDA REDAs_LUTE[] = {
    { .x = 1, .y = 12, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH4_LUTE[] = {
    UNIT_ENTRY(CHARACTER_LUTE, CLASS_MAGE_F, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 1, 11, 1, REDAs_LUTE, 0, 0, 0, 0, ITEM_ANIMA_FIRE, ITEM_VULNERARY),
    {},
};

static const struct UnitDefinition CH4_EIRIKA_SETH[] = {
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 8, 5, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_ANIMA_FIRE, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 8, 4, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_ANIMA_FIRE, ITEM_VULNERARY),
    {},
};

/**
 * Enemy unit and REDA definitions
 */

static const struct UnitDefinition CH4_ENEMY_UNITS[] = {
    UNIT_ENTRY(CHARACTER_MONSTER_B7, CLASS_BONEWALKER_BOW, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 11, 6, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_BOW_IRON), 
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 8, 14, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 7, 9, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 5, 8, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_B7, CLASS_BONEWALKER_BOW, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 13, 7, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_BOW_IRON),
    UNIT_ENTRY(CHARACTER_MONSTER_AC, CLASS_BONEWALKER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 2, 7, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_MONSTER_AC, CLASS_BONEWALKER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 3, 10, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 0, 4, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_B7, CLASS_BONEWALKER_BOW, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 12, 8, 0, REDAs_NULL, 0, 12, 0xC, 0, ITEM_BOW_IRON), 
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 5, 13, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 11, 4, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_AC, CLASS_BONEWALKER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 1, 14, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 10, 12, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 11, 14, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_ENTOMBED_CH4, CLASS_ENTOUMBED, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 13, 13, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_MONSTER_FETIDCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_B7, CLASS_BONEWALKER_BOW, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 13, 11, 0, REDAs_NULL, 0, 3, 0xC, 0, ITEM_BOW_IRON),   
    {}
};

static const struct REDA REDAs_BONEWALKER1_ENEMY[] = {
    { .x = 13, .y = 9, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH4_ENEMY_REINFORCEMENTS_BONEWALKERS[] = {
    UNIT_ENTRY(CHARACTER_MONSTER_AC, CLASS_BONEWALKER, FACTION_ID_RED,  NO_ITEM_DROP, AUTOLEVEL, 1, 14, 9, 1, REDAs_BONEWALKER1_ENEMY, 0, 0, 0xC, 0, ITEM_LANCE_IRON), 
    UNIT_ENTRY(CHARACTER_MONSTER_AC, CLASS_BONEWALKER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 14, 8, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_MONSTER_AC, CLASS_BONEWALKER, FACTION_ID_RED,  NO_ITEM_DROP, AUTOLEVEL, 2, 14, 6, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_LANCE_IRON),
    {}
};

static const struct REDA REDAs_REVENANT1_ENEMY[] = {
    { .x = 2, .y = 0, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_REVENANT2_ENEMY[] = {
    { .x = 0, .y = 2, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_REVENANT3_ENEMY[] = {
    { .x = 0, .y = 1, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH4_ENEMY_REINFORCEMENTS_REVENANTS[] = {
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED,  NO_ITEM_DROP, NO_AUTOLEVEL, 2, 0, 0, 0, REDAs_NULL, 0, 0, 0xC, 20, ITEM_MONSTER_FETIDCLW, ITEM_VULNERARY), 
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 0, 0, 1, REDAs_REVENANT1_ENEMY, 0, 0, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED,  NO_ITEM_DROP, AUTOLEVEL, 2, 0, 0, 1, REDAs_REVENANT2_ENEMY, 0, 0, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    UNIT_ENTRY(CHARACTER_MONSTER_AA, CLASS_REVENANT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 0, 0, 1, REDAs_REVENANT3_ENEMY, 0, 0, 0xC, 0, ITEM_MONSTER_ROTTENCLW),
    {}
};

/**
 * Green unit and REDA definitions
 */

static const struct REDA REDAs_LARACHEL1[] = {
    { .x = 13, .y = 2, .b = -1 },
};
static const struct REDA REDAs_RENNAC1[] = {
    { .x = 14, .y = 1, .b = -1 },
};
static const struct REDA REDAs_DOZLA1[] = {
    { .x = 12, .y = 1, .b = -1 },
};

static const struct UnitDefinition CH4_LARACHEL_APPEARS[] = {
    UNIT_ENTRY(CHARACTER_LARACHEL, CLASS_TROUBADOUR, FACTION_ID_GREEN, NO_ITEM_DROP, NO_AUTOLEVEL, 2, 13, 0, 1, REDAs_LARACHEL1,  6, 5, 8, 1, ITEM_SWORD_IRON, ITEM_LOCKPICK, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_RENNAC, CLASS_ROGUE, FACTION_ID_GREEN, NO_ITEM_DROP, NO_AUTOLEVEL, 2, 14, 0, 1, REDAs_RENNAC1,  6, 5, 8, 1, ITEM_SWORD_IRON, ITEM_LOCKPICK, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_DOZLA, CLASS_BERSERKER, FACTION_ID_GREEN, NO_ITEM_DROP, NO_AUTOLEVEL, 2, 12, 0, 1, REDAs_DOZLA1,  6, 5, 8, 1, ITEM_SWORD_IRON, ITEM_LOCKPICK, ITEM_VULNERARY),
    {}
};

static const struct REDA REDAs_LARACHEL_2[] = {
    { .x = 13, .y = 5, .b = -1 },
    { .x = 10, .y = 5, .b = -1 },
};
static const struct REDA REDAs_RENNAC_2[] = {
    { .x = 11, .y = 4, .b = -1, .delayFrames = 16},
};
static const struct REDA REDAs_DOZLA_2[] = {
    { .x = 11, .y = 6, .b = -1, .delayFrames = 16 },
};

static const struct UnitDefinition CH4_LARACHEL_APPEARS_2[] = {
    UNIT_ENTRY(CHARACTER_LARACHEL, CLASS_TROUBADOUR, FACTION_ID_GREEN, NO_ITEM_DROP, NO_AUTOLEVEL, 2, 14, 5, 2, REDAs_LARACHEL_2, 0, 0, 0xC, 20, ITEM_SWORD_IRON, ITEM_LOCKPICK, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_RENNAC, CLASS_ROGUE, FACTION_ID_GREEN, NO_ITEM_DROP, NO_AUTOLEVEL, 2, 14, 5, 1, REDAs_RENNAC_2, 0, 0, 0xC, 20, ITEM_SWORD_IRON, ITEM_LOCKPICK, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_DOZLA, CLASS_BERSERKER, FACTION_ID_GREEN, NO_ITEM_DROP, NO_AUTOLEVEL, 2, 14, 6, 1, REDAs_DOZLA_2, 0, 0, 0xC, 20, ITEM_SWORD_IRON, ITEM_LOCKPICK, ITEM_VULNERARY),
    {}
};

static const struct UnitDefinition CH4_GARCIA_APPEARS[] = {
    UNIT_ENTRY(CHARACTER_GARCIA, CLASS_FIGHTER, FACTION_ID_GREEN, NO_ITEM_DROP, NO_AUTOLEVEL, 5, 14, 7, 0, REDAs_NULL,  0, 0xA, 0, 0, ITEM_AXE_IRON, ITEM_AXE_HANDAXE, ITEM_VULNERARY),
    {}
};

/**
 * Main events
 */
static const EventScr EventScr_Beginning[] = {
    LOAD_WAIT_PERSIST(CH4_PLAYER_UNITS)
    SOUN(SFX_BIRDS_CHIRPING)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    Text_BG(0x23, Chapter_04_Scene_01_Convo_01)
    FADE_OUT_SCREEN(16)
    CLEAN
    LOAD_WAIT_PERSIST(CH4_ENEMY_UNITS)
    FADE_IN_SCREEN(16)
    MUSC(BGM_TENSION)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    Text(Chapter_04_Scene_02_Convo_01)
    FADE_OUT_SCREEN(16)
    MOVE_CAMERA_TO_POSITION(0, 10)
    FADE_IN_SCREEN(16)
    REDUCE_VOLUME
    HIGHLIGHT_COORDINATES(1, 11, 60)
    SetBackground(0x2)
    TEXTSTART
    TEXTSHOW(Chapter_04_Scene_03_Convo_01)
    TEXTEND
    RESTORE_VOLUME
    FADE_OUT_SCREEN(16)
    CLEAN
    MOVE_CAMERA_TO_POSITION(0, 0)
    REMA
    FADE_IN_SCREEN(16)
    LOAD_WAIT_PERSIST(CH4_ARTUR)
    HIGHLIGHT_CHARACTER(CHARACTER_ARTUR, 60)
    Text(Chapter_04_Scene_04_Convo_01)
    MOVE_POSITION_WAIT(24, 11, 6, 9, 3)
    HIGHLIGHT_CHARACTER(CHARACTER_ARTUR, 60)
    Text(Chapter_04_Scene_04_Convo_02)
    // Make enemy the active unit again
    MOVE_POSITION_WAIT(24, 9, 3, 9, 3)
    // Battle scene
    StartBattle
    NormalDamage(1, 0) // Artur attacks
    NormalDamage(0, 0) // Enemy attacks 
    CriticalHit(1, 20) // Artur criticals for 20 damage
    EndAttack
    FIGHT(CHAR_EVT_ACTIVE_UNIT, CHARACTER_ARTUR, 0, false)
    KILL(CHAR_EVT_ACTIVE_UNIT)
    ERASE(CHAR_EVT_ACTIVE_UNIT)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    Text(Chapter_04_Scene_04_Convo_03)
    CALL(EventScr_08591FD8) // Prep screen
    MOVE_CAMERA_TO_POSITION(0, 0)
    FADE_IN_SCREEN(16)
    MUSC(BGM_DISTANT_ROADS)
    HIGHLIGHT_CHARACTER(CHARACTER_ARTUR, 60)
    Text(Chapter_04_Scene_05_Convo_01)
    NoFade
    ENDA
};

static const EventScr EventScr_Ending[] = {
    MUSC(BGM_VICTORY)
/* If Lute wasn't recruited, then have her join here */
    CHECK_EVENTID(EVFLAG_TMP(0x8))
	SVAL(EVT_SLOT_0, 0x1)
	BEQ(0x1, EVT_SLOT_C, EVT_SLOT_0)
    GOTO(0x2)
LABEL(0x1)
    TEXT_BG_HIDE_MAP(0x2, Chapter_04_Scene_06_Convo_01)
    LOAD_WAIT_PERSIST(CH4_LUTE)
    GOTO(0x2)
LABEL(0x2)
    MUSC(BGM_INTO_THE_SHADOWS_OF_VICTORY)
    TEXT_BG_HIDE_MAP(0x18, Chapter_04_Scene_07_Convo_01)
    TEXT_BG_HIDE_MAP(0x18, Chapter_04_Scene_08_Convo_01)
    FADE_OUT_SCREEN(16)
    MUSC(BGM_NULL)
    CLEAN
    MOVE_CAMERA_TO_POSITION(7, 7)
    CLEAR_ALL_UNITS
    LOAD_WAIT(CH4_EIRIKA_SETH)
    FADE_IN_SCREEN(16)
    LOAD_WAIT(CH4_LARACHEL_APPEARS_2)
    MUSC(BGM_LAUGHTER)
    HIGHLIGHT_CHARACTER(CHARACTER_LARACHEL, 60)
    TEXT_BG_HIDE_MAP(0x18, Chapter_04_Scene_09_Convo_01)
    CHECK_IF_TRAINEE_OR_LEVEL_CAPPED(CHARACTER_ROSS, 10, CLASS_JOURNEYMAN)
    BEQ(0x3, EVT_SLOT_C, EVT_SLOT_0)
    SetBackground(0x19)
    TEXTSTART
    TEXTSHOW(Chapter_04_Scene_10_Convo_01)
    TEXTEND
    REMA
    GIVE_SKILL_SCROLL_TO(SID_Aptitude, CHARACTER_ROSS)
    GOTO(0x3)
LABEL(0x3)
    SetBackground(BLACK_BACKGROUND)
    REMA
    NEXT_CHAPTER_WITH_MAP(0x5) // Chapter 5 - The Empire's Reach
    ENDA
};

/**
 * Misc events
 */

static const EventListScr EventScr_CH4_BONEWALKERS_APPEAR[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 2)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    STAL(15)
    LOAD_WAIT_PERSIST(CH4_ENEMY_REINFORCEMENTS_BONEWALKERS)
    GOTO(0x1)

LABEL(0x0)
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF_SLOT2
    GOTO(0x1)

LABEL(0x1)
    NoFade
    ENDA
};

static const EventListScr EventScr_CH4_REVENANTS_APPEAR[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 3)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    STAL(15)
    LOAD_WAIT_PERSIST(CH4_ENEMY_REINFORCEMENTS_REVENANTS)
    GOTO(0x1)

LABEL(0x0)
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF_SLOT2
    GOTO(0x1)

LABEL(0x1)
    NoFade
    ENDA
};

static const EventListScr EventScr_CH4_LARACHEL_APPEARS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 2)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    CAMERA(7, 0)
    STAL(15)
    LOAD_WAIT_PERSIST(CH4_LARACHEL_APPEARS)
    MUSC(BGM_LAUGHTER)
    HIGHLIGHT_CHARACTER(CHARACTER_LARACHEL, 60)
    Text(Chapter_04_LARACHEL)
    MOVE(24, CHARACTER_LARACHEL, 15, 2)
    MOVE(24, CHARACTER_DOZLA, 15, 1)
    MOVE_WAIT(24, CHARACTER_RENNAC, 15, 1)
    CLEN // Remove NPCs
    GOTO(0x1)

LABEL(0x0)
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF_SLOT2
    GOTO(0x1)

LABEL(0x1)
    NoFade
    ENDA
};

static const EventListScr EventScr_CH4_GARCIA_APPEARS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 3)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)

    CHECK_ALIVE(CHARACTER_ROSS)
    BEQ(0x1, EVT_SLOT_C, EVT_SLOT_0)
    CHECK_DEPLOYED(CHARACTER_ROSS)
    BEQ(0x1, EVT_SLOT_C, EVT_SLOT_0)

    STAL(15)
    MOVE_CAMERA_TO_UNIT(CHARACTER_ROSS)
    HIGHLIGHT_CHARACTER(CHARACTER_ROSS, 60)
    Text(Chapter_04_ROSS)
    GOTO(0x1)

LABEL(0x0)
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF_SLOT2
    GOTO(0x2)

LABEL(0x1)
    LOAD_WAIT_PERSIST(CH4_GARCIA_APPEARS)
    MUSC(BGM_RAID)
    MOVE_CAMERA_TO_UNIT(CHARACTER_GARCIA)
    HIGHLIGHT_CHARACTER(CHARACTER_GARCIA, 60)
    Text(Chapter_04_GARCIA)
    GOTO(0x2)

LABEL(0x2)
    NoFade
    ENDA
};

static const EventListScr EventScr_Talk_ROSS_GARCIA[] = {
    CHANGE_MUSIC_SAVE_PREVIOUS_MUSIC(BGM_COMRADES)
    STAL(33)
    Text(Chapter_04_ROSS_GARCIA)
    RESTORE_PREVIOUS_MUSIC
    CHANGE_TO_BLUE(CHARACTER_GARCIA)
    NoFade
    ENDA
};

static const EventListScr EventListScr_VILLAGE_1_TALK[] = {
    HOUSE_EVENT_NO_END(0x2, Chapter_04_VILLAGE_OLD_MAN)
    CALL(EventScr_RemoveBGIfNeeded)
    GIVE_ITEM_TO(ITEM_AXE_IRON, CHARACTER_EVT_ACTIVE)
    NoFade
    ENDA
};

static const EventListScr EventListScr_VILLAGE_2_TALK[] = {
    MUSC(BGM_COMRADES)
    STAL(33)
    CHECK_ACTIVE
    SVAL(EVT_SLOT_1, CHARACTER_ARTUR)
    BEQ(0x0, EVT_SLOT_C, EVT_SLOT_1)
    SVAL(EVT_SLOT_1, CHARACTER_EIRIKA)
    BEQ(0x1, EVT_SLOT_C, EVT_SLOT_1) 
    GOTO(0x2)

LABEL(0x0)
    HOUSE_EVENT_NO_END(0x2, Chapter_04_VILLAGE_LUTE_ARTUR)
    GOTO(0x3)

LABEL(0x1)
    HOUSE_EVENT_NO_END(0x2, Chapter_04_VILLAGE_LUTE_EIRIKA)
    GOTO(0x3)

LABEL(0x2)
    HOUSE_EVENT_NO_END(0x2, Chapter_04_VILLAGE_LUTE_GENERIC)
    GOTO(0x3)

LABEL(0x3)
    CALL(EventScr_RemoveBGIfNeeded)
    LOAD_WAIT_PERSIST(CH4_LUTE)
    NoFade
    ENDA
};


/**
 * Event list
 */

static const EventListScr EventListScr_Turn[] = {
    AFEV(EVFLAG_TMP(7), EventScr_CH4_LARACHEL_APPEARS, 0)
    AFEV(EVFLAG_TMP(8), EventScr_CH4_GARCIA_APPEARS, 0)
    AFEV(EVFLAG_TMP(9), EventScr_CH4_BONEWALKERS_APPEAR, 0)
    AFEV(EVFLAG_TMP(10), EventScr_CH4_REVENANTS_APPEAR, 0)
    END_MAIN
};

static const EventListScr EventListScr_Character[] = {
    CharacterEventBothWays(EVFLAG_TMP(11), EventScr_Talk_ROSS_GARCIA, CHARACTER_ROSS, CHARACTER_GARCIA)
    END_MAIN
};

static const EventListScr EventListScr_Location[] = {
    Village(EVFLAG_TMP(12), EventListScr_VILLAGE_1_TALK, 8, 2)
    Village(EVFLAG_TMP(13), EventListScr_VILLAGE_2_TALK, 1, 11)
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

const struct ChapterEventGroup Chapter04Event = {
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

    .playerUnitsInNormal = CH4_PLAYER_UNITS,
    .playerUnitsInHard   = CH4_PLAYER_UNITS,

    .playerUnitsChoice1InEncounter = NULL,
    .playerUnitsChoice2InEncounter = NULL,
    .playerUnitsChoice3InEncounter = NULL,

    .enemyUnitsChoice1InEncounter = NULL,
    .enemyUnitsChoice2InEncounter = NULL,
    .enemyUnitsChoice3InEncounter = NULL,

    .beginningSceneEvents = EventScr_Beginning,
    .endingSceneEvents    = EventScr_Ending,
};
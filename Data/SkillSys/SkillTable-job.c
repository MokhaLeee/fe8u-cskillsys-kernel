#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u8 gConstSkillTable_Job[0x100][2] = {
    [CLASS_PALADIN] = {
        SKILL_INDEX_REAL(SID_HoneCavalry),
    },

    [CLASS_PALADIN_F] = {
        SKILL_INDEX_REAL(SID_HoneCavalry),
    },

    [CLASS_THIEF] = {
        SKILL_INDEX_REAL(SID_Shade),
    },

    [CLASS_GENERAL] = {
        SKILL_INDEX_REAL(SID_GreatShield),
        SKILL_INDEX_REAL(SID_HoneArmor),
    },

    [CLASS_GENERAL_F] = {
        SKILL_INDEX_REAL(SID_GreatShield),
        SKILL_INDEX_REAL(SID_HoneArmor),
    },

    [CLASS_SWORDMASTER] = {
        SKILL_INDEX_REAL(SID_Crit),
        SKILL_INDEX_REAL(SID_InfinityEdge),
    },

    [CLASS_SWORDMASTER_F] = {
        SKILL_INDEX_REAL(SID_Crit),
        SKILL_INDEX_REAL(SID_InfinityEdge),
    },

    [CLASS_ASSASSIN] = {
        SKILL_INDEX_REAL(SID_Lethality),
        SKILL_INDEX_REAL(SID_Shade),
    },

    [CLASS_ASSASSIN_F] = {
        SKILL_INDEX_REAL(SID_Lethality),
        SKILL_INDEX_REAL(SID_Shade),
    },

    [CLASS_SNIPER] = {
        SKILL_INDEX_REAL(SID_WatchfulEye),
        SKILL_INDEX_REAL(SID_SureShot),
    },

    [CLASS_SNIPER_F] = {
        SKILL_INDEX_REAL(SID_WatchfulEye),
        SKILL_INDEX_REAL(SID_SureShot),
    },

    [CLASS_WYVERN_LORD] = {
        SKILL_INDEX_REAL(SID_HoneFlier),
    },

    [CLASS_WYVERN_LORD_F] = {
        SKILL_INDEX_REAL(SID_HoneFlier),
    },

    [CLASS_WYVERN_KNIGHT] = {
        SKILL_INDEX_REAL(SID_HoneFlier),
    },

    [CLASS_WYVERN_KNIGHT_F] = {
        SKILL_INDEX_REAL(SID_HoneFlier),
    },

    [CLASS_SUMMONER] = {
        SKILL_INDEX_REAL(SID_Summon),
    },

    [CLASS_SUMMONER_F] = {
        SKILL_INDEX_REAL(SID_Summon),
    },

    [CLASS_ROGUE] = {
        SKILL_INDEX_REAL(SID_LockTouch),
        SKILL_INDEX_REAL(SID_ShadePlus),
    },

    [CLASS_FIGHTER] = {
        SKILL_INDEX_REAL(SID_Teleportation),
    },

    [CLASS_BERSERKER] = {
        SKILL_INDEX_REAL(SID_Crit),
        SKILL_INDEX_REAL(SID_InfinityEdge),
    },

    [CLASS_BARD] = {
        SKILL_INDEX_REAL(SID_Dance),
    },

    [CLASS_PEGASUS_KNIGHT] = {
        SKILL_INDEX_REAL(SID_HoneFlier),
    },

    [CLASS_DANCER] = {
        SKILL_INDEX_REAL(SID_Dance),
    },
};

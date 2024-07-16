#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u16 gConstSkillTable_Job[0x100][2] = {
    [CLASS_EIRIKA_LORD] = {
        SID_Pass,
        SID_CA_Galeforce,
    },

    [CLASS_PALADIN] = {
        SID_HoneCavalry,
    },

    [CLASS_PALADIN_F] = {
        SID_HoneCavalry,
    },

    [CLASS_THIEF] = {
        SID_Shade,
    },

    [CLASS_GENERAL] = {
        SID_GreatShield,
        SID_HoneArmor,
    },

    [CLASS_GENERAL_F] = {
        SID_GreatShield,
        SID_HoneArmor,
    },

    [CLASS_SWORDMASTER] = {
        SID_Crit,
        SID_InfinityEdge,
    },

    [CLASS_SWORDMASTER_F] = {
        SID_Crit,
        SID_InfinityEdge,
    },

    [CLASS_ASSASSIN] = {
        SID_Lethality,
        SID_Shade,
    },

    [CLASS_ASSASSIN_F] = {
        SID_Lethality,
        SID_Shade,
    },

    [CLASS_SNIPER] = {
        SID_WatchfulEye,
        SID_SureShot,
    },

    [CLASS_SNIPER_F] = {
        SID_WatchfulEye,
        SID_SureShot,
    },

    [CLASS_WYVERN_LORD] = {
        SID_HoneFlier,
    },

    [CLASS_WYVERN_LORD_F] = {
        SID_HoneFlier,
    },

    [CLASS_WYVERN_KNIGHT] = {
        SID_HoneFlier,
    },

    [CLASS_WYVERN_KNIGHT_F] = {
        SID_HoneFlier,
    },

    [CLASS_SUMMONER] = {
        SID_Summon,
    },

    [CLASS_SUMMONER_F] = {
        SID_Summon,
    },

    [CLASS_ROGUE] = {
        SID_LockTouch,
        SID_ShadePlus,
    },

    [CLASS_FIGHTER] = {
        SID_Teleportation,
    },

    [CLASS_BERSERKER] = {
        SID_Crit,
        SID_InfinityEdge,
    },

    [CLASS_BARD] = {
        SID_Dance,
    },

    [CLASS_PEGASUS_KNIGHT] = {
        SID_HoneFlier,
    },

    [CLASS_DANCER] = {
        SID_Dance,
    },

    [CLASS_SOLDIER] = {
#if (defined(SID_Obstruct) && COMMON_SKILL_VALID(SID_Obstruct))
        SID_Obstruct,
#endif
    },
};

#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u16 gConstSkillTable_Person[0x100][2] = {
    [CHARACTER_EIRIKA] = {
        SID_Reflex,
        SID_InitSpectrum,
    },

    [CHARACTER_EPHRAIM] = {
        SID_InitNimble,
        SID_COMBAT_Detonate,
    },

    [CHARACTER_SALEH] = {
        SID_Thunderstorm,
        SID_RallyMovement,
    },

    [CHARACTER_VANESSA] = {
        SID_Lifetaker,
    },

    [CHARACTER_TANA] = {
        SID_AlertStancePlus,
    },

    [CHARACTER_MARISA] = {
        SID_DoubleLion,
        SID_Adept
    },

    [CHARACTER_TETHYS] = {
        SID_LightRune,
        SID_Mine,
    },

    [CHARACTER_LYON_CC] = {
        SID_AversaNight,
    },

    [CHARACTER_LYON] = {
        SID_AversaNight,
    },
};

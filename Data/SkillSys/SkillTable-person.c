#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u16 gConstSkillTable_Person[0x100][2] = {
    [CHARACTER_EIRIKA] = {
        SID_Supply,
        SID_Counter,
    },

    [CHARACTER_EPHRAIM] = {
        SID_Armsthrift,
    },

    [CHARACTER_LYON_CC] = {
        SID_AversaNight,
    },

    [CHARACTER_LYON] = {
        SID_AversaNight,
    },
};

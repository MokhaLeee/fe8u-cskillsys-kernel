#include "common-chax.h"
#include "strmag.h"

const struct UnitMagicInfo gMagicPInfos[0x100] = {
    [CHARACTER_EIRIKA] = {
        .base = 0,
        .growth = 40,
    },
    [CHARACTER_LUTE] = {
        .base = 5,
        .growth = 75,
    }
};

const struct UnitMagicInfo gMagicJInfos[0x100] = {
    [CLASS_EIRIKA_LORD] = {
        .base = 3,
        .growth = 40,
        .cap = 20,
        .bonus = 0,
    },
    [CLASS_MAGE_F] = {
        .base = 4,
        .growth = 60,
        .cap = 27,
    },
};

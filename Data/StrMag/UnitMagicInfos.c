#include "global.h"
#include "bmunit.h"
#include "constants/characters.h"
#include "constants/classes.h"

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

FEB_IDENTIFIER(gMagicPInfos);

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

FEB_IDENTIFIER(gMagicJInfos);

#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u8 gConstSkillPTable[0x100][2] = {
    [CHARACTER_EIRIKA] = {SID_Supply},
    [CHARACTER_SETH] = {SID_Vantage},
};

const u8 gConstSkillJTable[0x100][2] = {
    [CLASS_ARCHER] = {SID_FlashingBlade, SID_Galeforce},
    [CLASS_SNIPER] = {SID_RangeBonusBow1},
    [CLASS_SNIPER_F] = {SID_RangeBonusBow1},
};

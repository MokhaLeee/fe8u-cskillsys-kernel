#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u8 gConstSkillPTable[0x100][2] = {
#if (defined(SID_Supply) && (SID_Supply < MAX_SKILL_NUM))
    [CHARACTER_EIRIKA] = {SID_Supply},
#endif

#if (defined(SID_Vantage) && (SID_Vantage < MAX_SKILL_NUM))
    [CHARACTER_SETH] = {SID_Vantage},
#endif
};

const u8 gConstSkillJTable[0x100][2] = {
#if (defined(SID_FlashingBlade) && (SID_FlashingBlade < MAX_SKILL_NUM))
    [CLASS_ARCHER] = {SID_FlashingBlade, SID_Galeforce},
#endif

#if (defined(SID_RangeBonusBow1) && (SID_RangeBonusBow1 < MAX_SKILL_NUM))
    [CLASS_SNIPER] = {SID_RangeBonusBow1},
#endif

#if (defined(SID_RangeBonusBow1) && (SID_RangeBonusBow1 < MAX_SKILL_NUM))
    [CLASS_SNIPER_F] = {SID_RangeBonusBow1},
#endif
};

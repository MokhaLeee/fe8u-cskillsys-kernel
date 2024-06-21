#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u8 gConstSkillPTable[0x100][2] = {
    [CHARACTER_EIRIKA] = {
#if (defined(SID_Supply) && (SID_Supply < MAX_SKILL_NUM))
        SID_Supply,
#endif
#if (defined(SID_Colossus) && (SID_Colossus < MAX_SKILL_NUM))
        SID_Colossus
#endif
    },

    [CHARACTER_SETH] = {
#if (defined(SID_Vantage) && (SID_Vantage < MAX_SKILL_NUM))
        SID_Vantage
#endif
    },
};

const u8 gConstSkillJTable[0x100][2] = {
    [CLASS_ARCHER] = {
#if (defined(SID_FlashingBlade) && (SID_FlashingBlade < MAX_SKILL_NUM))
        SID_FlashingBlade,
#endif
#if (defined(SID_Galeforce) && (SID_Galeforce < MAX_SKILL_NUM))
        SID_Galeforce
#endif
    },

    [CLASS_PALADIN] = {
#if (defined(SID_HoneCavalry) && (SID_HoneCavalry < MAX_SKILL_NUM))
        SID_HoneCavalry
#endif
    },

    [CLASS_PALADIN_F] = {
#if (defined(SID_HoneCavalry) && (SID_HoneCavalry < MAX_SKILL_NUM))
        SID_HoneCavalry
#endif
    },

    [CLASS_SNIPER] = {
#if (defined(SID_RangeBonusBow1) && (SID_RangeBonusBow1 < MAX_SKILL_NUM))
        SID_RangeBonusBow1
#endif
    },

    [CLASS_SNIPER_F] = {
#if (defined(SID_RangeBonusBow1) && (SID_RangeBonusBow1 < MAX_SKILL_NUM))
        SID_RangeBonusBow1
#endif
    },

    [CLASS_PEGASUS_KNIGHT] = {
#if (defined(SID_HoneFlier) && (SID_HoneFlier < MAX_SKILL_NUM))
        SID_HoneFlier
#endif
    },
};

#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const struct SkillPreloadPConf gSkillPreloadPData[0x100] = {
	[CHARACTER_EIRIKA] = {{
#if (defined(SID_SpdBonus) && COMMON_SKILL_VALID(SID_SpdBonus))
		[0] = SID_SpdBonus,
#endif

#if (defined(SID_PosReturn) && COMMON_SKILL_VALID(SID_PosReturn))
		[1] = SID_PosReturn,
#endif

#if (defined(SID_RightfulGod) && COMMON_SKILL_VALID(SID_RightfulGod))
		[10] = SID_RightfulGod,
#endif
	}},

	[CHARACTER_EPHRAIM] = {{
#if (defined(SID_StrBonus) && COMMON_SKILL_VALID(SID_StrBonus))
		[0] = SID_StrBonus,
#endif
	}},

	[CHARACTER_SETH] = {{
#if (defined(SID_Vantage) && COMMON_SKILL_VALID(SID_Vantage))
		[0] = SID_Vantage,
#endif
	}},

	[CHARACTER_INNES] = {{
#if (defined(SID_RangeBonusBow2) && COMMON_SKILL_VALID(SID_RangeBonusBow2))
		[0] = SID_RangeBonusBow2,
#endif

#if (defined(SID_Lunge) && COMMON_SKILL_VALID(SID_Lunge))
		[1] = SID_Lunge,
#endif
	}},

	[CHARACTER_SALEH] = {{
#if (defined(SID_RangeBonusBMag1) && COMMON_SKILL_VALID(SID_RangeBonusBMag1))
		[0] = SID_RangeBonusBMag1,
#endif

#if (defined(SID_RangeBonusBMag2) && COMMON_SKILL_VALID(SID_RangeBonusBMag2))
		[1] = SID_RangeBonusBMag2,
#endif

#if (defined(SID_Lunge) && COMMON_SKILL_VALID(SID_Lunge))
		[2] = SID_Lunge,
#endif
	}},

	[CHARACTER_MYRRH] = {{
#if (defined(SID_RangeBonusBMag1) && COMMON_SKILL_VALID(SID_RangeBonusBMag1))
		[10] = SID_RangeBonusBMag1,
#endif
	}},
};

const struct SkillPreloadJConf gSkillPreloadJData[0x100] = {
	[CLASS_SNIPER] = {{
#if (defined(SID_RangeBonusBow2) && COMMON_SKILL_VALID(SID_RangeBonusBow2))
		[10] = SID_RangeBonusBow2,
#endif
	}},

	[CLASS_SNIPER_F] = {{
#if (defined(SID_RangeBonusBow2) && COMMON_SKILL_VALID(SID_RangeBonusBow2))
		[10] = SID_RangeBonusBow2,
#endif
	}},

	[CLASS_MANAKETE_MYRRH] = {{
#if (defined(SID_SuperLuck) && COMMON_SKILL_VALID(SID_SuperLuck))
		[10] = SID_SuperLuck
#endif
	}},
};

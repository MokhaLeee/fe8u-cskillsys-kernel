#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u16 gConstSkillTable_Person[0x100][2] = {
	[CHARACTER_EIRIKA] = {
#if (defined(SID_Supply) && COMMON_SKILL_VALID(SID_Supply))
		SID_Supply,
#endif

#if (defined(SID_InitSpectrum) && COMMON_SKILL_VALID(SID_InitSpectrum))
		SID_InitSpectrum,
#endif
	},

	[CHARACTER_EPHRAIM] = {
#if (defined(SID_Blacksmith) && COMMON_SKILL_VALID(SID_Blacksmith))
		SID_Blacksmith,
#endif

#if (defined(SID_COMBAT_Detonate) && COMMON_SKILL_VALID(SID_COMBAT_Detonate))
		SID_COMBAT_Detonate,
#endif
	},

	[CHARACTER_NATASHA] = {
#if (defined(SID_Latona) && COMMON_SKILL_VALID(SID_Latona))
		SID_Latona,
#endif

#if (defined(SID_YuneWhispers) && COMMON_SKILL_VALID(SID_YuneWhispers))
		SID_YuneWhispers,
#endif
	},

	[CHARACTER_SALEH] = {
#if (defined(SID_Thunderstorm) && COMMON_SKILL_VALID(SID_Thunderstorm))
		SID_Thunderstorm,
#endif

#if (defined(SID_RallyMovement) && COMMON_SKILL_VALID(SID_RallyMovement))
		SID_RallyMovement,
#endif
	},

	[CHARACTER_VANESSA] = {
#if (defined(SID_Lifetaker) && COMMON_SKILL_VALID(SID_Lifetaker))
		SID_Lifetaker,
#endif

#if (defined(SID_Perch) && COMMON_SKILL_VALID(SID_Perch))
		SID_Perch,
#endif
	},

	[CHARACTER_TANA] = {
#if (defined(SID_AlertStancePlus) && COMMON_SKILL_VALID(SID_AlertStancePlus))
		SID_AlertStancePlus,
#endif

#if (defined(SID_OverKill) && COMMON_SKILL_VALID(SID_OverKill))
		SID_OverKill,
#endif
	},

	[CHARACTER_MARISA] = {
#if (defined(SID_DoubleLion) && COMMON_SKILL_VALID(SID_DoubleLion))
		SID_DoubleLion,
#endif

#if (defined(SID_Adept) && COMMON_SKILL_VALID(SID_Adept))
		SID_Adept
#endif
	},

	[CHARACTER_TETHYS] = {
#if (defined(SID_LightRune) && COMMON_SKILL_VALID(SID_LightRune))
		SID_LightRune,
#endif

#if (defined(SID_Mine) && COMMON_SKILL_VALID(SID_Mine))
		SID_Mine,
#endif
	},

	[CHARACTER_KNOLL] = {
#if (defined(SID_FreeSpirit) && COMMON_SKILL_VALID(SID_FreeSpirit))
		SID_FreeSpirit,
#endif
	},

	[CHARACTER_LYON_CC] = {
#if (defined(SID_AversaNight) && COMMON_SKILL_VALID(SID_AversaNight))
		SID_AversaNight,
#endif
	},

	[CHARACTER_LYON] = {
#if (defined(SID_AversaNight) && COMMON_SKILL_VALID(SID_AversaNight))
		SID_AversaNight,
#endif
	},
};

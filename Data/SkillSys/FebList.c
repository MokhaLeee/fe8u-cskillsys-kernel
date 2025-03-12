#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

/**
 * If you want use this list, make sure that the following config has been uncommented:
 *
 * CONFIG_FIT_OLD_SKILLSYS_LIST
 */

struct CharLvupSkillEnt const *const CharLevelUpSkillTable[0x100] = {
	[CHARACTER_EIRIKA] = (const struct CharLvupSkillEnt []) {
#if (defined(SID_DoubleLion) && EQUIPE_SKILL_VALID(SID_DoubleLion))
		{ 1, SID_DoubleLion },
#endif

#if (defined(SID_Desperation) && EQUIPE_SKILL_VALID(SID_Desperation))
		{ 10, SID_Desperation },
#endif

		{ 0 }
	},

	[CHARACTER_SETH] = (const struct CharLvupSkillEnt []) {
#if (defined(SID_FaireSword) && EQUIPE_SKILL_VALID(SID_FaireSword))
		{ 20, SID_FaireSword },
#endif

		{ 0 }
	},
};

struct CharLvupSkillEnt const *const ClassLevelUpSkillTable[0x100] = {
	[CLASS_EIRIKA_LORD] = (const struct CharLvupSkillEnt []) {
#if (defined(SID_DoubleLion) && EQUIPE_SKILL_VALID(SID_DoubleLion))
		{ JOB_SKILL_LEVEL_CONFIG(10, LOAD_JOB_SKILL_CONFIG_NONE), SID_DoubleLion },
#endif

		{ 0 }
	},

	[CLASS_PALADIN] = (const struct CharLvupSkillEnt []) {
#if (defined(SID_HeavyBlade) && EQUIPE_SKILL_VALID(SID_HeavyBlade))
		{ JOB_SKILL_LEVEL_PROMOTE, SID_HeavyBlade },
#endif

		{ 0 }
	},

	[CLASS_BRIGAND] = (const struct CharLvupSkillEnt []) {
#if (defined(SID_Frenzy) && EQUIPE_SKILL_VALID(SID_Frenzy))
		{ JOB_SKILL_LEVEL_CONFIG(1, LOAD_JOB_SKILL_CONFIG_ENEMY_ONLY), SID_Frenzy },
#endif

		{ 0 }
	},
};

const u8 ClassSkillTable[0x100] = {
#if (defined(SID_PushStrength) && EQUIPE_SKILL_VALID(SID_PushStrength))
	[CLASS_EIRIKA_MASTER_LORD] = SID_PushStrength,
#endif

#if (defined(SID_FuryPlus) && EQUIPE_SKILL_VALID(SID_FuryPlus))
	[CLASS_PALADIN] = SID_FuryPlus,
#endif

#if (defined(SID_FuryPlus) && EQUIPE_SKILL_VALID(SID_FuryPlus))
	[CLASS_BRIGAND] = SID_FuryPlus,
#endif
};

const u8 PersonalSkillTable[0x100] = {
#if (defined(SID_FortressDef) && EQUIPE_SKILL_VALID(SID_FortressDef))
	[CHARACTER_AMELIA] = SID_FortressDef,
#endif

#if (defined(SID_LifeAndDeath) && EQUIPE_SKILL_VALID(SID_LifeAndDeath))
	[CHARACTER_MARISA] = SID_LifeAndDeath,
#endif
};

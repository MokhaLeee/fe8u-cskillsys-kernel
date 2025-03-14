#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u16 gConstSkillTable_Weapon[0x100][2] = {
	[ITEM_SWORD_IRON] = {
#if (defined(SID_COMBAT_Hexblade) && COMMON_SKILL_VALID(SID_COMBAT_Hexblade))
		SID_COMBAT_Hexblade,
#endif
	},

	[ITEM_SWORD_SLIM] = {
#if (defined(SID_RuinedBladePlus) && COMMON_SKILL_VALID(SID_RuinedBladePlus))
		SID_RuinedBladePlus,
#endif
	},

	[ITEM_SWORD_SILVER] = {
#if (defined(SID_COMBAT_BaneOfMonsters) && COMMON_SKILL_VALID(SID_COMBAT_BaneOfMonsters))
		SID_COMBAT_BaneOfMonsters,
#endif

#if (defined(SID_DoubleLion) && EQUIPE_SKILL_VALID(SID_DoubleLion))
		SID_DoubleLion,
#endif
	},

	[ITEM_SWORD_RAPIER] = {
#if (defined(SID_COMBAT_Grounder) && COMMON_SKILL_VALID(SID_COMBAT_Grounder))
		SID_COMBAT_Grounder,
#endif
	},
};

#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u16 gConstSkillTable_Item[0x100][2] = {
#ifdef CONFIG_USE_DEBUG
	[ITEM_SWORD_SLIM] = {
#if (defined(SID_RuinedBladePlus) && COMMON_SKILL_VALID(SID_RuinedBladePlus))
		SID_RuinedBladePlus,
#endif
	},

	[ITEM_DARK_NOSFERATU] = {
#if (defined(SID_COMBAT_CrimsonStrike) && COMMON_SKILL_VALID(SID_COMBAT_CrimsonStrike))
		SID_COMBAT_CrimsonStrike,
#endif
	},
#endif /* CONFIG_USE_DEBUG */
};

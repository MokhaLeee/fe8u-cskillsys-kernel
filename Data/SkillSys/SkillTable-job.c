#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u16 gConstSkillTable_Job[0x100][2] = {
	[CLASS_EPHRAIM_LORD] = {
#if (defined(SID_COMBAT_VitalReckoning) && COMMON_SKILL_VALID(SID_COMBAT_VitalReckoning))
		SID_COMBAT_VitalReckoning,
#endif
	},

	[CLASS_EIRIKA_LORD] = {
#if (defined(SID_Pass) && COMMON_SKILL_VALID(SID_Pass))
		SID_Pass,
#endif

#if (defined(SID_DivineReprieve) && COMMON_SKILL_VALID(SID_DivineReprieve))
		SID_DivineReprieve,
#endif
	},

	[CLASS_EPHRAIM_MASTER_LORD] = {
#if (defined(SID_SpectrumBoost) && COMMON_SKILL_VALID(SID_SpectrumBoost))
		SID_SpectrumBoost,
#endif
	},

	[CLASS_PALADIN] = {
#if (defined(SID_HoneCavalry) && COMMON_SKILL_VALID(SID_HoneCavalry))
		SID_HoneCavalry,
#endif

#if (defined(SID_COMBAT_Gamble) && COMMON_SKILL_VALID(SID_COMBAT_Gamble))
		SID_COMBAT_Gamble,
#endif
	},

	[CLASS_PALADIN_F] = {
#if (defined(SID_HoneCavalry) && COMMON_SKILL_VALID(SID_HoneCavalry))
		SID_HoneCavalry,
#endif
	},

	[CLASS_THIEF] = {
#if (defined(SID_Shade) && COMMON_SKILL_VALID(SID_Shade))
		SID_Shade,
#endif
	},

	[CLASS_GENERAL] = {
#if (defined(SID_GreatShield) && COMMON_SKILL_VALID(SID_GreatShield))
		SID_GreatShield,
#endif

#if (defined(SID_HoneArmor) && COMMON_SKILL_VALID(SID_HoneArmor))
		SID_HoneArmor,
#endif
	},

	[CLASS_GENERAL_F] = {
#if (defined(SID_GreatShield) && COMMON_SKILL_VALID(SID_GreatShield))
		SID_GreatShield,
#endif

#if (defined(SID_HoneArmor) && COMMON_SKILL_VALID(SID_HoneArmor))
		SID_HoneArmor,
#endif
	},

	[CLASS_MYRMIDON_F] = {
#if (defined(SID_Astra) && COMMON_SKILL_VALID(SID_Astra))
		SID_Astra,
#endif

#if (defined(SID_FatalTen) && COMMON_SKILL_VALID(SID_FatalTen))
		SID_FatalTen,
#endif
	},

	[CLASS_SWORDMASTER] = {
#if (defined(SID_Crit) && COMMON_SKILL_VALID(SID_Crit))
		SID_Crit,
#endif

#if (defined(SID_InfinityEdge) && COMMON_SKILL_VALID(SID_InfinityEdge))
		SID_InfinityEdge,
#endif
	},

	[CLASS_SWORDMASTER_F] = {
#if (defined(SID_Crit) && COMMON_SKILL_VALID(SID_Crit))
		SID_Crit,
#endif

#if (defined(SID_InfinityEdge) && COMMON_SKILL_VALID(SID_InfinityEdge))
		SID_InfinityEdge,
#endif
	},

	[CLASS_ASSASSIN] = {
#if (defined(SID_Lethality) && COMMON_SKILL_VALID(SID_Lethality))
		SID_Lethality,
#endif

#if (defined(SID_Shade) && COMMON_SKILL_VALID(SID_Shade))
		SID_Shade,
#endif
	},

	[CLASS_ASSASSIN_F] = {
#if (defined(SID_Lethality) && COMMON_SKILL_VALID(SID_Lethality))
		SID_Lethality,
#endif

#if (defined(SID_Shade) && COMMON_SKILL_VALID(SID_Shade))
		SID_Shade,
#endif
	},

	[CLASS_SNIPER] = {
#if (defined(SID_WatchfulEye) && COMMON_SKILL_VALID(SID_WatchfulEye))
		SID_WatchfulEye,
#endif

#if (defined(SID_SureShot) && COMMON_SKILL_VALID(SID_SureShot))
		SID_SureShot,
#endif
	},

	[CLASS_SNIPER_F] = {
#if (defined(SID_WatchfulEye) && COMMON_SKILL_VALID(SID_WatchfulEye))
		SID_WatchfulEye,
#endif

#if (defined(SID_SureShot) && COMMON_SKILL_VALID(SID_SureShot))
		SID_SureShot,
#endif
	},

	[CLASS_WYVERN_LORD] = {
#if (defined(SID_HoneFlier) && COMMON_SKILL_VALID(SID_HoneFlier))
		SID_HoneFlier,
#endif
	},

	[CLASS_WYVERN_LORD_F] = {
#if (defined(SID_HoneFlier) && COMMON_SKILL_VALID(SID_HoneFlier))
		SID_HoneFlier,
#endif
	},

	[CLASS_WYVERN_KNIGHT] = {
#if (defined(SID_HoneFlier) && COMMON_SKILL_VALID(SID_HoneFlier))
		SID_HoneFlier,
#endif
	},

	[CLASS_WYVERN_KNIGHT_F] = {
#if (defined(SID_HoneFlier) && COMMON_SKILL_VALID(SID_HoneFlier))
		SID_HoneFlier,
#endif
	},

	[CLASS_SUMMONER] = {
#if (defined(SID_Summon) && COMMON_SKILL_VALID(SID_Summon))
		SID_Summon,
#endif
	},

	[CLASS_SUMMONER_F] = {
#if (defined(SID_Summon) && COMMON_SKILL_VALID(SID_Summon))
		SID_Summon,
#endif
	},

	[CLASS_ROGUE] = {
#if (defined(SID_LockTouch) && COMMON_SKILL_VALID(SID_LockTouch))
		SID_LockTouch,
#endif

#if (defined(SID_ShadePlus) && COMMON_SKILL_VALID(SID_ShadePlus))
		SID_ShadePlus,
#endif
	},

	[CLASS_FIGHTER] = {
#if (defined(SID_Teleportation) && COMMON_SKILL_VALID(SID_Teleportation))
		SID_Teleportation,
#endif
	},

	[CLASS_BERSERKER] = {
#if (defined(SID_Crit) && COMMON_SKILL_VALID(SID_Crit))
		SID_Crit,
#endif

#if (defined(SID_InfinityEdge) && COMMON_SKILL_VALID(SID_InfinityEdge))
		SID_InfinityEdge,
#endif
	},

	[CLASS_BARD] = {
#if (defined(SID_Dance) && COMMON_SKILL_VALID(SID_Dance))
		SID_Dance,
#endif
	},

	[CLASS_PEGASUS_KNIGHT] = {
#if (defined(SID_HoneFlier) && COMMON_SKILL_VALID(SID_HoneFlier))
		SID_HoneFlier,
#endif
	},

	[CLASS_DANCER] = {
#if (defined(SID_Dance) && COMMON_SKILL_VALID(SID_Dance))
		SID_Dance,
#endif

#if (defined(SID_GoddessDance) && COMMON_SKILL_VALID(SID_GoddessDance))
		SID_GoddessDance,
#endif
	},

	[CLASS_SOLDIER] = {
#if (defined(SID_Obstruct) && COMMON_SKILL_VALID(SID_Obstruct))
		SID_Obstruct,
#endif
	},
};

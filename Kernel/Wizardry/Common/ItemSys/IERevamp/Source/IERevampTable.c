#include "common-chax.h"
#include "item-sys.h"

struct IERevamp const IERevampTable[IER_MAX] = {
	[IER_STAFF_HEAL] = {
		.usability      = IER_Usability_AdjacentHeal,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_STAFF_MEND] = {
		.usability      = IER_Usability_AdjacentHeal,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_STAFF_RECOVER] = {
		.usability      = IER_Usability_AdjacentHeal,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_STAFF_PHYSIC] = {
		.usability      = IER_Usability_RangedHeal,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_STAFF_FORTIFY] = {
		.usability      = IER_Usability_RangedHeal,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_STAFF_RESTORE] = {
		.usability      = IER_Usability_Restore,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_STAFF_SILENCE] = {
		.usability      = IER_Usability_Silence,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_STAFF_SLEEP] = {
		.usability      = IER_Usability_Sleep,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_STAFF_BERSERK] = {
		.usability      = IER_Usability_Berserk,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_STAFF_WARP] = {
		.usability      = IER_Usability_Warp,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_STAFF_RESCUE] = {
		.usability      = IER_Usability_Rescue,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_STAFF_TORCH] = {
		.usability      = IER_Usability_Torch,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_STAFF_REPAIR] = {
		.usability      = IER_Usability_Hammerne,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_STAFF_UNLOCK] = {
		.usability      = IER_Usability_Unlock,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_STAFF_BARRIER] = {
		.usability      = IER_Usability_Barrier,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},

	/**
	 * Boosters
	 */
	[IER_BOOSTER_HP] = {
		.usability      = IER_Usability_Booster,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_BOOSTER_POW] = {
		.usability      = IER_Usability_Booster,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_BOOSTER_SKL] = {
		.usability      = IER_Usability_Booster,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_BOOSTER_SPD] = {
		.usability      = IER_Usability_Booster,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_BOOSTER_LCK] = {
		.usability      = IER_Usability_Booster,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_BOOSTER_DEF] = {
		.usability      = IER_Usability_Booster,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_BOOSTER_RES] = {
		.usability      = IER_Usability_Booster,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_BOOSTER_MOV] = {
		.usability      = IER_Usability_Booster,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_BOOSTER_CON] = {
		.usability      = IER_Usability_Booster,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},

	/**
	 * Promotion
	 */
	[IER_HEROCREST] = {
		.usability      = IER_Usability_Promotion,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_KNIGHTCREST] = {
		.usability      = IER_Usability_Promotion,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_ORIONSBOLT] = {
		.usability      = IER_Usability_Promotion,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_ELYSIANWHIP] = {
		.usability      = IER_Usability_Promotion,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_GUIDINGRING] = {
		.usability      = IER_Usability_Promotion,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},

	/**
	 * Key
	 */
	[IER_CHESTKEY] = {
		.usability      = IER_Usability_ChestKey,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_DOORKEY] = {
		.usability      = IER_Usability_DoorKey,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_LOCKPICK] = {
		.usability      = IER_Usability_LockPick,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},

	/**
	 * Vulnerary
	 */
	[IER_VULNERARY] = {
		.usability      = IER_Usability_Vulnerary,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_ELIXIR] = {
		.usability      = IER_Usability_Vulnerary,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},

	[IER_PUREWATER] = {
		.usability      = IER_Usability_PureWater,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_ANTITOXIN] = {
		.usability      = IER_Usability_Antitoxin,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_TORCH] = {
		.usability      = IER_Usability_Torch,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_CHESTKEY_BUNDLE] = {
		.usability      = IER_Usability_ChestKey,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_MINE] = {
		.usability      = IER_Usability_Mine,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_LIGHTRUNE] = {
		.usability      = IER_Usability_LightRune,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_FILLAS_MIGHT] = {
		.usability      = IER_Usability_DanceRing,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_NINISS_GRACE] = {
		.usability      = IER_Usability_DanceRing,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_THORS_IRE] = {
		.usability      = IER_Usability_DanceRing,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_SETS_LITANY] = {
		.usability      = IER_Usability_DanceRing,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},

	/**
	 * Promotion
	 */
	[IER_MASTERSEAL] = {
		.usability      = IER_Usability_Promotion,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_METISSTOME] = {
		.usability      = IER_Usability_MetisStone,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_HEAVENSEAL] = {
		.usability      = IER_Usability_Promotion,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_OCEANSEAL] = {
		.usability      = IER_Usability_Promotion,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_LUNARBRACE] = {
		.usability      = IER_Usability_Promotion,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_SOLARBRACE] = {
		.usability      = IER_Usability_Promotion,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},

	/**
	 * Vulnerary
	 */
	[IER_VULNERARY_2] = {
		.usability      = IER_Usability_Vulnerary,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},

	[IER_UNK_GREENNOTE] = {
		.usability      = NULL,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_UNK_REDNOTE] = {
		.usability      = NULL,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_JUNAFRUIT] = {
		.usability      = IER_Usability_JunaFruit,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_CHAX_STAFF_LATONA] = {
		.usability      = NULL,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_CHAX_SKILL_SCROLL] = {
		.usability      = NULL,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
	[IER_CHAX_STAFF_NIGHTMARE] = {
		.usability      = NULL,
		.prep_usability = NULL,
		.effect         = NULL,
		.action_effect  = NULL,
		.prep_effect    = NULL,
	},
};

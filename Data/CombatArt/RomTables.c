#include "common-chax.h"
#include "combat-art.h"
#include "constants/combat-arts.h"

const struct CombatArtRomTable gCombatArtRomPTable[0x100] = {
	[CHARACTER_EIRIKA] = {
		.cid_sword = {
			[WPN_LEVEL_E] = CID_WrathStrike,
		},
	},
	[CHARACTER_INNES] = {
		.cid_bow = {
			[WPN_LEVEL_C] = CID_Encloser,
			[WPN_LEVEL_S] = CID_EncloserPlus,
		},
	},
};

const struct CombatArtRomTable gCombatArtRomJTable[0x100] = {
	[CLASS_EIRIKA_LORD] = {
		.cid_sword = {
			[WPN_LEVEL_E] = CID_Grounder,
		},
	},
	[CLASS_EIRIKA_MASTER_LORD] = {
		.cid_sword = {
			[WPN_LEVEL_E] = CID_Grounder,
		},
	},
	[CLASS_SNIPER] = {
		.cid_bow = {
			[WPN_LEVEL_C] = CID_BreakShot,
			[WPN_LEVEL_A] = CID_HuntersVolley,
			[WPN_LEVEL_S] = CID_BreakShotPlus,
		},
	},
	[CLASS_SNIPER_F] = {
		.cid_bow = {
			[WPN_LEVEL_C] = CID_BreakShot,
			[WPN_LEVEL_A] = CID_HuntersVolley,
			[WPN_LEVEL_S] = CID_BreakShotPlus,
		},
	},
};

const struct CombatArtRomTable gCombatArtDefaultTable = {
	.cid_sword = {
		[WPN_LEVEL_D] = CID_WrathStrike,
		[WPN_LEVEL_C] = CID_Soulblade,
		[WPN_LEVEL_B] = CID_FoudroyantStrike,
	},
	.cid_lance = {
		[WPN_LEVEL_D] = CID_TempestLance,
		[WPN_LEVEL_C] = CID_MonsterPiercer,
		[WPN_LEVEL_B] = CID_FrozenLance,
	},
	.cid_axe = {
		[WPN_LEVEL_D] = CID_Smash,
		[WPN_LEVEL_C] = CID_FocusedStrike,
		[WPN_LEVEL_B] = CID_HelmSplitter,
		[WPN_LEVEL_S] = CID_FlickeringFlower,
	},
	.cid_bow = {
		[WPN_LEVEL_D] = CID_CurvedShot,
		[WPN_LEVEL_C] = CID_Deadeye,
		[WPN_LEVEL_B] = CID_WaningShot,
		[WPN_LEVEL_A] = CID_Encloser,
		[WPN_LEVEL_S] = CID_WaningShotPlus,
	},
};

#include "common-chax.h"
#include "strmag.h"

const struct UnitMagicInfo gMagicPInfos[0x100] = {
	[CHARACTER_EIRIKA] = {
		.base = 0,
		.growth = 30,
	},
	[CHARACTER_SETH] = {
		.base = 5,
		.growth = 15,
	},
	[CHARACTER_GILLIAM] = {
		.base = 0,
		.growth = 10,
	},
	[CHARACTER_FRANZ] = {
		.base = 1,
		.growth = 20,
	},
	[CHARACTER_MOULDER] = {
		.base = 3,
		.growth = 40,
	},
	[CHARACTER_VANESSA] = {
		.base = 2,
		.growth = 40,
	},
	[CHARACTER_ROSS] = {
		.base = 0,
		.growth = 10,
	},
	[CHARACTER_NEIMI] = {
		.base = 1,
		.growth = 20,
	},
	[CHARACTER_COLM] = {
		.base = 0,
		.growth = 10,
	},
	[CHARACTER_GARCIA] = {
		.base = 0,
		.growth = 10,
	},
	[CHARACTER_INNES] = {
		.base = 3,
		.growth = 15,
	},
	[CHARACTER_LUTE] = {
		.base = 5,
		.growth = 65,
	},
	[CHARACTER_NATASHA] = {
		.base = 1,
		.growth = 60,
	},
	[CHARACTER_CORMAG] = {
		.base = 0,
		.growth = 10,
	},
	[CHARACTER_EPHRAIM] = {
		.base = 0,
		.growth = 10,
	},
	[CHARACTER_FORDE] = {
		.base = 2,
		.growth = 25,
	},
	[CHARACTER_KYLE] = {
		.base = 0,
		.growth = 10,
	},
	[CHARACTER_AMELIA] = {
		.base = 3,
		.growth = 30,
	},
	[CHARACTER_ARTUR] = {
		.base = 5,
		.growth = 50,
	},
	[CHARACTER_GERIK] = {
		.base = 0,
		.growth = 30,
	},
	[CHARACTER_TETHYS] = {
		.base = 4,
		.growth = 40,
	},
	[CHARACTER_MARISA] = {
		.base = 0,
		.growth = 10,
	},
	[CHARACTER_SALEH] = {
		.base = 11,
		.growth = 30,
	},
	[CHARACTER_EWAN] = {
		.base = 2,
		.growth = 45,
	},
	[CHARACTER_LARACHEL] = {
		.base = 5,
		.growth = 50,
	},
	[CHARACTER_DOZLA] = {
		.base = 2,
		.growth = 25,
	},
	[CHARACTER_RENNAC] = {
		.base = 5,
		.growth = 35,
	},
	[CHARACTER_DUESSEL] = {
		.base = 2,
		.growth = 10,
	},
	[CHARACTER_MYRRH] = {
		.base = 6,
		.growth = 50,
	},
	[CHARACTER_KNOLL] = {
		.base = 10,
		.growth = 50,
	},
	[CHARACTER_JOSHUA] = {
		.base = 2,
		.growth = 30,
	},
	[CHARACTER_SYRENE] = {
		.base = 4,
		.growth = 40,
	},
	[CHARACTER_TANA] = {
		.base = 2,
		.growth = 30,
	},
	[CHARACTER_LYON_CC] = {
		.base = 13,
		.growth = 50,
	},
	[CHARACTER_ORSON_CC] = {
		.base = 0,
		.growth = 0,
	},
	[CHARACTER_GLEN_CC] = {
		.base = 0,
		.growth = 10,
	},
	[CHARACTER_SELENA_CC] = {
		.base = 10,
		.growth = 40,
	},
	[CHARACTER_VALTER_CC] = {
		.base = 4,
		.growth = 20,
	},
	[CHARACTER_RIEV_CC] = {
		.base = 10,
		.growth = 45,
	},
	[CHARACTER_CAELLACH_CC] = {
		.base = 0,
		.growth = 0,
	},
	[CHARACTER_FADO_CC] = {
		.base = 1,
		.growth = 10,
	},
	[CHARACTER_ISMAIRE_CC] = {
		.base = 2,
		.growth = 20,
	},
	[CHARACTER_HAYDEN_CC] = {
		.base = 0,
		.growth = 10,
	},
	[CHARACTER_LYON] = {
		.base = 13,
		.growth = 50,
	},
	[CHARACTER_MORVA] = {
		.base = 12,
		.growth = 65,
	},
	[CHARACTER_ORSON_CH5X] = {
		.base = 0,
		.growth = 0,
	},
	[CHARACTER_VALTER] = {
		.base = 4,
		.growth = 20,
	},
	[CHARACTER_SELENA] = {
		.base = 10,
		.growth = 40,
	},
	[CHARACTER_VALTER_PROLOGUE] = {
		.base = 4,
		.growth = 20,
	},
	[CHARACTER_BREGUET] = {
		.base = 0,
		.growth = 0,
	},
	[CHARACTER_BONE] = {
		.base = 0,
		.growth = 0,
	},
	[CHARACTER_BAZBA] = {
		.base = 0,
		.growth = 0,
	},
	[CHARACTER_SAAR] = {
		.base = 0,
		.growth = 0,
	},
	[CHARACTER_NOVALA] = {
		.base = 8,
		.growth = 50,
	},
	[CHARACTER_MURRAY] = {
		.base = 0,
		.growth = 0,
	},
	[CHARACTER_TIRADO] = {
		.base = 0,
		.growth = 0,
	},
	[CHARACTER_BINKS] = {
		.base = 0,
		.growth = 0,
	},
	[CHARACTER_PABLO] = {
		.base = 8,
		.growth = 50,
	},
	[CHARACTER_AIAS] = {
		.base = 0,
		.growth = 0,
	},
	[CHARACTER_CARLYLE] = {
		.base = 0,
		.growth = 0,
	},
	[CHARACTER_CAELLACH] = {
		.base = 0,
		.growth = 10,
	},
	[CHARACTER_PABLO_2] = {
		.base = 8,
		.growth = 50,
	},
	[CHARACTER_RIEV] = {
		.base = 10,
		.growth = 45,
	},
	[CHARACTER_GHEB] = {
		.base = 0,
		.growth = 0,
	},
	[CHARACTER_BERAN] = {
		.base = 0,
		.growth = 0,
	},
	[CHARACTER_BANDIT_CH5] = {
		.base = 0,
		.growth = 0,
	},
	[CHARACTER_ONEILL] = {
		.base = 0,
		.growth = 10,
	},
	[CHARACTER_GLEN] = {
		.base = 0,
		.growth = 10,
	},
	[CHARACTER_ZONTA] = {
		.base = 0,
		.growth = 0,
	},
	[CHARACTER_VIGARDE] = {
		.base = 2,
		.growth = 15,
	},
	[CHARACTER_LYON_FINAL] = {
		.base = 20,
		.growth = 20,
	},
	[CHARACTER_ORSON] = {
		.base = 0,
		.growth = 10,
	},
	[CHARACTER_FADO] = {
		.base = 1,
		.growth = 10,
	},
	[CHARACTER_HAYDEN] = {
		.base = 0,
		.growth = 10,
	},
	[CHARACTER_MANSEL] = {
		.base = 11,
		.growth = 40,
	},
	[CHARACTER_KLIMT] = {
		.base = 5,
		.growth = 40,
	},
	[CHARACTER_DARA] = {
		.base = 15,
		.growth = 40,
	},
	[CHARACTER_ISMAIRE] = {
		.base = 2,
		.growth = 20,
	},
	[CHARACTER_MESSENGER] = {
		.base = 5,
		.growth = 40,
	},
};

const struct UnitMagicInfo gMagicJInfos[0x100] = {
	[CLASS_EPHRAIM_LORD] = {
		.base = 2,
		.growth = 40,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_EIRIKA_LORD] = {
		.base = 3,
		.growth = 40,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_EPHRAIM_MASTER_LORD] = {
		.base = 3,
		.growth = 40,
		.cap = 22,
		.bonus = 3,
	},
	[CLASS_EIRIKA_MASTER_LORD] = {
		.base = 3,
		.growth = 40,
		.cap = 25,
		.bonus = 3,
	},
	[CLASS_CAVALIER] = {
		.base = 0,
		.growth = 10,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_CAVALIER_F] = {
		.base = 1,
		.growth = 10,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_PALADIN] = {
		.base = 1,
		.growth = 10,
		.cap = 22,
		.bonus = 1,
	},
	[CLASS_PALADIN_F] = {
		.base = 1,
		.growth = 10,
		.cap = 23,
		.bonus = 1,
	},
	[CLASS_ARMOR_KNIGHT] = {
		.base = 0,
		.growth = 0,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_ARMOR_KNIGHT_F] = {
		.base = 0,
		.growth = 5,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_GENERAL] = {
		.base = 0,
		.growth = 0,
		.cap = 21,
		.bonus = 0,
	},
	[CLASS_GENERAL_F] = {
		.base = 0,
		.growth = 0,
		.cap = 22,
		.bonus = 0,
	},
	[CLASS_THIEF] = {
		.base = 1,
		.growth = 10,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_MANAKETE] = {
		.base = 3,
		.growth = 40,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_MERCENARY] = {
		.base = 1,
		.growth = 20,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_MERCENARY_F] = {
		.base = 1,
		.growth = 20,
		.cap = 20,
		.bonus = 0,
	},

	[CLASS_HERO] = {
		.base = 1,
		.growth = 20,
		.cap = 21,
		.bonus = 1,
	},
	[CLASS_HERO_F] = {
		.base = 1,
		.growth = 20,
		.cap = 21,
		.bonus = 1,
	},
	[CLASS_MYRMIDON] = {
		.base = 0,
		.growth = 10,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_MYRMIDON_F] = {
		.base = 0,
		.growth = 10,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_SWORDMASTER] = {
		.base = 1,
		.growth = 20,
		.cap = 21,
		.bonus = 0,
	},
	[CLASS_SWORDMASTER_F] = {
		.base = 1,
		.growth = 20,
		.cap = 21,
		.bonus = 0,
	},
	[CLASS_ASSASSIN] = {
		.base = 1,
		.growth = 20,
		.cap = 22,
		.bonus = 1,
	},
	[CLASS_ASSASSIN_F] = {
		.base = 1,
		.growth = 20,
		.cap = 22,
		.bonus = 1,
	},
	[CLASS_ARCHER] = {
		.base = 0,
		.growth = 10,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_ARCHER_F] = {
		.base = 0,
		.growth = 10,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_SNIPER] = {
		.base = 1,
		.growth = 20,
		.cap = 21,
		.bonus = 0,
	},
	[CLASS_SNIPER_F] = {
		.base = 1,
		.growth = 20,
		.cap = 21,
		.bonus = 0,
	},
	[CLASS_RANGER] = {
		.base = 1,
		.growth = 20,
		.cap = 21,
		.bonus = 0,
	},
	[CLASS_RANGER_F] = {
		.base = 1,
		.growth = 20,
		.cap = 21,
		.bonus = 0,
	},
	[CLASS_WYVERN_RIDER] = {
		.base = 0,
		.growth = 10,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_WYVERN_RIDER_F] = {
		.base = 0,
		.growth = 10,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_WYVERN_LORD] = {
		.base = 1,
		.growth = 20,
		.cap = 21,
		.bonus = 1,
	},
	[CLASS_WYVERN_LORD_F] = {
		.base = 1,
		.growth = 20,
		.cap = 21,
		.bonus = 0,
	},
	[CLASS_WYVERN_KNIGHT] = {
		.base = 2,
		.growth = 25,
		.cap = 22,
		.bonus = 1,
	},
	[CLASS_WYVERN_KNIGHT_F] = {
		.base = 2,
		.growth = 25,
		.cap = 22,
		.bonus = 1,
	},

	[CLASS_MAGE] = {
		.base = 1,
		.growth = 20,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_MAGE_F] = {
		.base = 1,
		.growth = 20,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_SAGE] = {
		.base = 5,
		.growth = 45,
		.cap = 28,
		.bonus = 1,
	},
	[CLASS_SAGE_F] = {
		.base = 5,
		.growth = 45,
		.cap = 30,
		.bonus = 1,
	},
	[CLASS_MAGE_KNIGHT] = {
		.base = 4,
		.growth = 40,
		.cap = 24,
		.bonus = 2,
	},
	[CLASS_MAGE_KNIGHT_F] = {
		.base = 3,
		.growth = 40,
		.cap = 25,
		.bonus = 2,
	},
	[CLASS_BISHOP] = {
		.base = 4,
		.growth = 35,
		.cap = 25,
		.bonus = 2,
	},
	[CLASS_BISHOP_F] = {
		.base = 4,
		.growth = 35,
		.cap = 25,
		.bonus = 1,
	},
	[CLASS_SHAMAN] = {
		.base = 2,
		.growth = 50,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_SHAMAN_F] = {
		.base = 3,
		.growth = 45,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_DRUID] = {
		.base = 6,
		.growth = 55,
		.cap = 29,
		.bonus = 0,
	},
	[CLASS_DRUID_F] = {
		.base = 7,
		.growth = 55,
		.cap = 29,
		.bonus = 2,
	},
	[CLASS_SUMMONER] = {
		.base = 5,
		.growth = 50,
		.cap = 27,
		.bonus = 2,
	},
	[CLASS_SUMMONER_F] = {
		.base = 5,
		.growth = 50,
		.cap = 27,
		.bonus = 0,
	},
	[CLASS_ROGUE] = {
		.base = 1,
		.growth = 25,
		.cap = 23,
		.bonus = 2,
	},
	[CLASS_GREAT_KNIGHT] = {
		.base = 1,
		.growth = 20,
		.cap = 21,
		.bonus = 0,
	},
	[CLASS_GREAT_KNIGHT_F] = {
		.base = 1,
		.growth = 20,
		.cap = 21,
		.bonus = 0,
	},
	[CLASS_JOURNEYMAN_T2] = {
		.base = 0,
		.growth = 10,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_PUPIL_T2] = {
		.base = 4,
		.growth = 35,
		.cap = 29,
		.bonus = 0,
	},
	[CLASS_RECRUIT_T2] = {
		.base = 1,
		.growth = 20,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_MANAKETE_2] = {
		.base = 0,
		.growth = 25,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_MANAKETE_MYRRH] = {
		.base = 0,
		.growth = 25,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_JOURNEYMAN] = {
		.base = 0,
		.growth = 10,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_PUPIL] = {
		.base = 1,
		.growth = 55,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_FIGHTER] = {
		.base = 0,
		.growth = 0,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_WARRIOR] = {
		.base = 0,
		.growth = 10,
		.cap = 21,
		.bonus = 0,
	},
	[CLASS_BRIGAND] = {
		.base = 2,
		.growth = 20,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_PIRATE] = {
		.base = 1,
		.growth = 15,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_BERSERKER] = {
		.base = 2,
		.growth = 25,
		.cap = 21,
		.bonus = 0,
	},
	[CLASS_MONK] = {
		.base = 1,
		.growth = 30,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_PRIEST] = {
		.base = 1,
		.growth = 30,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_BARD] = {
		.base = 1,
		.growth = 45,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_RECRUIT] = {
		.base = 0,
		.growth = 20,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_PEGASUS_KNIGHT] = {
		.base = 3,
		.growth = 20,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_FALCON_KNIGHT] = {
		.base = 4,
		.growth = 30,
		.cap = 23,
		.bonus = 1,
	},
	[CLASS_CLERIC] = {
		.base = 1,
		.growth = 30,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_TROUBADOUR] = {
		.base = 1,
		.growth = 20,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_VALKYRIE] = {
		.base = 4,
		.growth = 35,
		.cap = 25,
		.bonus = 2,
	},
	[CLASS_DANCER] = {
		.base = 1,
		.growth = 35,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_SOLDIER] = {
		.base = 0,
		.growth = 10,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_NECROMANCER] = {
		.base = 9,
		.growth = 30,
		.cap = 30,
		.bonus = 0,
	},
	[CLASS_FLEET] = {
		.base = 0,
		.growth = 0,
		.cap = 20,
		.bonus = 0,
	},

	[CLASS_PHANTOM] = {
		.base = 2,
		.growth = 20,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_REVENANT] = {
		.base = 0,
		.growth = 0,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_ENTOUMBED] = {
		.base = 0,
		.growth = 0,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_BONEWALKER] = {
		.base = 1,
		.growth = 15,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_BONEWALKER_BOW] = {
		.base = 1,
		.growth = 15,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_WIGHT] = {
		.base = 2,
		.growth = 25,
		.cap = 22,
		.bonus = 1,
	},
	[CLASS_WIGHT_BOW] = {
		.base = 2,
		.growth = 2,
		.cap = 22,
		.bonus = 1,
	},
	[CLASS_BAEL] = {
		.base = 0,
		.growth = 0,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_ELDER_BAEL] = {
		.base = 0,
		.growth = 0,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_CYCLOPS] = {
		.base = 0,
		.growth = 0,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_MAUTHEDOOG] = {
		.base = 0,
		.growth = 0,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_GWYLLGI] = {
		.base = 0,
		.growth = 0,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_TARVOS] = {
		.base = 0,
		.growth = 0,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_MAELDUIN] = {
		.base = 0,
		.growth = 0,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_MOGALL] = {
		.base = 2,
		.growth = 45,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_ARCH_MOGALL] = {
		.base = 6,
		.growth = 55,
		.cap = 29,
		.bonus = 0,
	},
	[CLASS_GORGON] = {
		.base = 8,
		.growth = 60,
		.cap = 29,
		.bonus = 0,
	},
	[CLASS_GARGOYLE] = {
		.base = 0,
		.growth = 0,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_DEATHGOYLE] = {
		.base = 0,
		.growth = 20,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_DRACO_ZOMBIE] = {
		.base = 1,
		.growth = 50,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_DEMON_KING] = {
		.base = 23,
		.growth = 50,
		.cap = 50,
		.bonus = 0,
	},
	[CLASS_CYCLOPS_2] = {
		.base = 0,
		.growth = 0,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_ELDER_BAEL_2] = {
		.base = 0,
		.growth = 0,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_JOURNEYMAN_T1] = {
		.base = 0,
		.growth = 10,
		.cap = 20,
		.bonus = 0,
	},
	[CLASS_PUPIL_T1] = {
		.base = 2,
		.growth = 55,
		.cap = 20,
		.bonus = 1,
	},
};

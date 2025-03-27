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

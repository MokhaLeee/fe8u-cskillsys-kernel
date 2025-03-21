#include "common-chax.h"
#include "battle-system.h"

const s8 gCriticalBonus[0x100] = {
	[CLASS_SWORDMASTER] = 15,
	[CLASS_SWORDMASTER_F] = 15,
	[CLASS_BERSERKER] = 10,
	[CLASS_CAVALIER] = -10,
	[CLASS_CAVALIER_F] = -10,
	[CLASS_PALADIN] = -10,
	[CLASS_PALADIN_F] = -10,
};

const s8 gSilencerBonus[0x100] = {
	[CLASS_SWORDMASTER] = 1,
	[CLASS_SWORDMASTER_F] = 1,
	[CLASS_ASSASSIN] = 20,
	[CLASS_ASSASSIN_F] = 20,
};

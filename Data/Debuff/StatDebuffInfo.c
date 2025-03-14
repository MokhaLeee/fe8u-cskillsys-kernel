#include "common-chax.h"
#include "debuff.h"

const struct DebuffInfo gStatDebuffInfos[UNIT_STAT_DEBUFF_MAX] = {
	[UNIT_STAT_BUFF_RING_ATK] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.battle_status = { .atk = 10 }
	},
	[UNIT_STAT_BUFF_RING_DEF] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.battle_status = { .def = 10 }
	},
	[UNIT_STAT_BUFF_RING_CRT] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.battle_status = { .crit = 10 }
	},
	[UNIT_STAT_BUFF_RING_AVO] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.battle_status = { .avo = 10 }
	},

	[UNIT_STAT_DEBUFF_POW] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
		.unit_status = { .pow = -4 },
	},
	[UNIT_STAT_DEBUFF_MAG] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
		.unit_status = { .mag = -4 },
	},
	[UNIT_STAT_DEBUFF_SKL] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
		.unit_status = { .skl = -4 },
	},
	[UNIT_STAT_DEBUFF_SPD] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
		.unit_status = { .spd = -4 },
	},
	[UNIT_STAT_DEBUFF_LCK] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
		.unit_status = { .lck = -4 },
	},
	[UNIT_STAT_DEBUFF_DEF] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
		.unit_status = { .def = -4 },
	},
	[UNIT_STAT_DEBUFF_RES] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
		.unit_status = { .res = -4 },
	},
	[UNIT_STAT_DEBUFF_MOV] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
		.unit_status = { .mov = -4 },
	},

	[UNIT_STAT_BUFF_POW] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .pow = 4 },
	},
	[UNIT_STAT_BUFF_MAG] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .mag = 4 },
	},
	[UNIT_STAT_BUFF_SKL] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .skl = 4 },
	},
	[UNIT_STAT_BUFF_SPD] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .spd = 4 },
	},
	[UNIT_STAT_BUFF_LCK] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .lck = 4 },
	},
	[UNIT_STAT_BUFF_DEF] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .def = 4 },
	},
	[UNIT_STAT_BUFF_RES] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .res = 4 },
	},
	[UNIT_STAT_BUFF_MOV] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .mov = 4 },
	},

	[UNIT_STAT_BUFF_INIT_POW] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_NO_TICK, /* tick it on manual */
		.unit_status = { .pow = 7 },
	},
	[UNIT_STAT_BUFF_INIT_MAG] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_NO_TICK, /* tick it on manual */
		.unit_status = { .mag = 7 },
	},
	[UNIT_STAT_BUFF_INIT_SKL] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_NO_TICK, /* tick it on manual */
		.unit_status = { .skl = 7 },
	},
	[UNIT_STAT_BUFF_INIT_SPD] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_NO_TICK, /* tick it on manual */
		.unit_status = { .spd = 7 },
	},
	[UNIT_STAT_BUFF_INIT_LCK] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_NO_TICK, /* tick it on manual */
		.unit_status = { .lck = 7 },
	},
	[UNIT_STAT_BUFF_INIT_DEF] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_NO_TICK, /* tick it on manual */
		.unit_status = { .def = 7 },
	},
	[UNIT_STAT_BUFF_INIT_RES] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_NO_TICK, /* tick it on manual */
		.unit_status = { .res = 7 },
	},
	[UNIT_STAT_BUFF_INIT_MOV] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_NO_TICK, /* tick it on manual */
		.unit_status = { .mov = 7 },
	},

	[UNIT_STAT_BUFF_OATHROUSE_POW] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .pow = 4 },
	},
	[UNIT_STAT_BUFF_OATHROUSE_MAG] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .mag = 4 },
	},
	[UNIT_STAT_BUFF_OATHROUSE_SKL] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .skl = 4 },
	},
	[UNIT_STAT_BUFF_OATHROUSE_SPD] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .spd = 4 },
	},
	[UNIT_STAT_BUFF_OATHROUSE_LCK] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .lck = 4 },
	},
	[UNIT_STAT_BUFF_OATHROUSE_DEF] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .def = 4 },
	},
	[UNIT_STAT_BUFF_OATHROUSE_RES] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .res = 4 },
	},
	[UNIT_STAT_BUFF_OATHROUSE_MOV] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .mov = 4 },
	},

	[UNIT_STAT_BUFF_ROUSE_POW] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .pow = 4 },
	},
	[UNIT_STAT_BUFF_ROUSE_MAG] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .mag = 4 },
	},
	[UNIT_STAT_BUFF_ROUSE_SKL] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .skl = 4 },
	},
	[UNIT_STAT_BUFF_ROUSE_SPD] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .spd = 4 },
	},
	[UNIT_STAT_BUFF_ROUSE_LCK] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .lck = 4 },
	},
	[UNIT_STAT_BUFF_ROUSE_DEF] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .def = 4 },
	},
	[UNIT_STAT_BUFF_ROUSE_RES] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .res = 4 },
	},
	[UNIT_STAT_BUFF_ROUSE_MOV] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .mov = 4 },
	},

	[UNIT_STAT_BUFF_RALLY_POW] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .pow = 4 },
	},
	[UNIT_STAT_BUFF_RALLY_MAG] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .mag = 4 },
	},
	[UNIT_STAT_BUFF_RALLY_SKL] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .skl = 4 },
	},
	[UNIT_STAT_BUFF_RALLY_SPD] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .spd = 4 },
	},
	[UNIT_STAT_BUFF_RALLY_LCK] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .lck = 4 },
	},
	[UNIT_STAT_BUFF_RALLY_DEF] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .def = 4 },
	},
	[UNIT_STAT_BUFF_RALLY_RES] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .res = 4 },
	},
	[UNIT_STAT_BUFF_RALLY_MOV] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .mov = 4 },
	},

	[UNIT_STAT_DEBUFF_YuneWhispers] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
		.unit_status = {
			.pow = -6,
			.mag = -6,
			.spd = -6,
		},
	},

	[UNIT_STAT_BUFF_JOB_HONE] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = {
			.pow = 6,
			.mag = 6,
			.skl = 6,
			.spd = 6,
		},
	},
	[UNIT_STAT_BUFF_JOB_FORTIFY] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = {
			.lck = 6,
			.def = 6,
			.res = 6,
		},
	},

	[UNIT_STAT_DEBUFF_AversaNight] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
		.unit_status = {
			.pow = -3,
			.mag = -3,
			.spd = -3,
			.def = -3,
			.res = -3,
		},
	},

	[UNIT_STAT_BUFF_PLUSMINUS] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = {
			.pow = 4,
			.mag = 4,
			.skl = 4,
			.spd = 4,
			.lck = 4,
			.def = 4,
			.res = 4,
		},
	},

	[UNIT_STAT_BUFF_INDOOR_MARCH_MOV] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .mov = 2 },
	},

	[UNIT_STAT_BUFF_NATURE_RUSH_MOV] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .mov = 2 },
	},

	[UNIT_STAT_BUFF_KEEPUP] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .mov = 2 },
	},

	[UNIT_STAT_BUFF_ARMOR_MARCH] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .mov = 2 },
	},

	[UNIT_STAT_BUFF_STRIDE] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = { .mov = 3 },
	},

	[UNIT_STAT_DEBUFF_CHILL_POW] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_NO_TICK, /* tick it on manual */
		.unit_status = { .pow = -7 },
	},
	[UNIT_STAT_DEBUFF_CHILL_MAG] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_NO_TICK, /* tick it on manual */
		.unit_status = { .mag = -7 },
	},
	[UNIT_STAT_DEBUFF_CHILL_SKL] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_NO_TICK, /* tick it on manual */
		.unit_status = { .skl = -7 },
	},
	[UNIT_STAT_DEBUFF_CHILL_SPD] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_NO_TICK, /* tick it on manual */
		.unit_status = { .spd = -7 },
	},
	[UNIT_STAT_DEBUFF_CHILL_LCK] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_NO_TICK, /* tick it on manual */
		.unit_status = { .lck = -7 },
	},
	[UNIT_STAT_DEBUFF_CHILL_DEF] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_NO_TICK, /* tick it on manual */
		.unit_status = { .def = -7 },
	},
	[UNIT_STAT_DEBUFF_CHILL_RES] = {
		.positive_type = STATUS_DEBUFF_NEGATIVE,
		.tick_type = STATUS_DEBUFF_NO_TICK, /* tick it on manual */
		.unit_status = { .res = -7 },
	},

	[UNIT_STAT_BUFF_GLORIFIER] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_NO_TICK, /* tick it on manual */
		.unit_status = {
			.pow = 3,
			.mag = 3,
			.skl = 3,
			.spd = 3,
			.lck = 3,
			.def = 3,
			.res = 3,
		},
	},

	[UNIT_STAT_BUFF_FREESPIRIT] = {
		.positive_type = STATUS_DEBUFF_POSITIVE,
		.tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
		.unit_status = {
			.pow = 5,
			.mag = 5,
			.skl = 5,
			.spd = 5,
			.lck = 5,
			.def = 5,
			.res = 5,
		},
	},
};

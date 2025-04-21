#pragma once

enum MapTaskConfIndex {
	MTSKCONF_HPBAR,
	MTSKCONF_COMM_BUFF,
	MTSKCONF_COMM_DEBUFF,
	MTSKCONF_RESCUING,
	MTSKCONF_BOSS,
	MTSKCONF_PROTECTION,
};

#include "common-chax.h"

struct MapTaskConf {
	u16 x, y;
	u16 *obj;
};

// extern struct MapTaskConf const gMapTaskConfs[];
extern struct MapTaskConf const *const gpMapTaskConfs;

// extern u8 const gHpBarChr[];
extern u8 const *const gpHpBarChr;

extern struct Vec2u MapTaskVec;

void MapTaskPutOamHi(int type, int oam2);

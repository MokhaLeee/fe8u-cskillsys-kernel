#pragma once

#include "common-chax.h"

struct UnitListHeader {
	const void *pinfo;
	const void *jinfo;
	s8 level;
	s8 uid;
	u8 items[5];

	u8 _pad_;
};

bool JudgeUnitList(struct Unit *unit, struct UnitListHeader *ref);
void WriteUnitList(struct Unit *unit, struct UnitListHeader *out);

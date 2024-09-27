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

extern bool (*_JudgeUnitList)(struct Unit *unit, struct UnitListHeader * ref);
extern void (*_WriteUnitList)(struct Unit *unit, struct UnitListHeader * out);

#define JudgeUnitList _JudgeUnitList
#define WriteUnitList _WriteUnitList

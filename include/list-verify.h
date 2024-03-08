#pragma once

#include "global.h"
#include "bmunit.h"
#include "common-chax.h"

struct UnitListHeader {
    s8 uid;
    u8 pid, jid, level;
    u8 weapon;
    u8 _pad_[11];
};

bool JudgeUnitListHeader(struct Unit * unit, struct UnitListHeader * ref);
void WriteUnitListHeader(struct Unit * unit, struct UnitListHeader * out);

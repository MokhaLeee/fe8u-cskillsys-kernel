#pragma once

#include "common-chax.h"

struct UnitListHeader {
    s8 uid;
    u8 pid, jid, level;
    u8 items[UNIT_ITEM_COUNT];

    u8 _pad_[7];
};

bool JudgeUnitListHeader(struct Unit * unit, struct UnitListHeader * ref);
void WriteUnitListHeader(struct Unit * unit, struct UnitListHeader * out);

#pragma once

#include "global.h"
#include "bmunit.h"

/**
 * We use unit->_u3A and unit->_u3B
 * These 16 bits is saved in suspand data
 */

enum unit_expa_sus_bitfile_idx {
    UES_BIT_LEGENDARY_SKILL_USED,
    UES_BIT_LEGENDARY_SKILL_ACTIVE,
    UNIT_EXPA_SUS_BIT2,
    UNIT_EXPA_SUS_BIT3,
    UNIT_EXPA_SUS_BIT4,
    UNIT_EXPA_SUS_BIT5,
    UNIT_EXPA_SUS_BIT6,
    UNIT_EXPA_SUS_BIT7,
    UNIT_EXPA_SUS_BIT8,
    UNIT_EXPA_SUS_BIT9,
    UNIT_EXPA_SUS_BIT10,
    UNIT_EXPA_SUS_BIT11,
    UNIT_EXPA_SUS_BIT12,
    UNIT_EXPA_SUS_BIT13,
    UNIT_EXPA_SUS_BIT14,
    UNIT_EXPA_SUS_BIT15,

    UES_BIT_MAX
};

void SetBitUES(struct Unit * unit, int bit);
void ClearBitUES(struct Unit * unit, int bit);
bool CheckBitUES(struct Unit * unit, int bit);

void ResetUnitsExpaSus(void);

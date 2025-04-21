#pragma once

#include "common-chax.h"

/**
 * We use unit->_u3A and unit->_u3B
 * These 16 bits is saved in suspand data
 */

enum unit_expa_sus_bitfile_idx {
	UES_BIT_LEGENDARY_SKILL_USED,
	UES_BIT_LEGENDARY_SKILL_ACTIVE,
	UES_BIT_GUARDBEAR_SKILL_USED,
	UES_BIT_TSZUKU_SKILL_USED,
	UES_BIT_TELEPOTRATION_SKILL_USED,
	UES_BIT_ACTED,
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

void SetBitUES(struct Unit *unit, int bit);
void ClearBitUES(struct Unit *unit, int bit);
bool CheckBitUES(struct Unit *unit, int bit);

void ResetUnitsExpaSus(void);

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
    UES_BIT_SWARP_SKILL_USED,
    UES_BIT_SWAP_SKILL_USED,
    UES_BIT_ROLL12_SKILL_USED,
    UES_BIT_CAPTURE_SKILL_USED,
    UES_BIT_TURNCOAT,
    UES_BIT_RESURRECTION_SKILL_USED,
    UES_BIT_SHADOWFLASH_SKILL_USED,
    UES_BIT_TWIN_CRESTS_SKILL_USED,
    UES_BIT_DEATHBLIGHT_SKILL_USED,
    UES_BIT_LOADSTAR_RUSH_SKILL_USED,
    UNIT_EXPA_SUS_BIT15,

    UES_BIT_MAX
};

void SetBitUES(struct Unit * unit, int bit);
void SetBitUES_BU(struct BattleUnit * bu, int bit);
void ClearBitUES(struct Unit * unit, int bit);
bool CheckBitUES(struct Unit * unit, int bit);

void ResetUnitsExpaSus(void);

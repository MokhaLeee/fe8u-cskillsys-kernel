#include "common-chax.h"
#include "unit-expa.h"

bool PrePhsae_ClearMiscUES(ProcPtr proc)
{
    int uid;
    struct Unit * unit;

    for (uid = FACTION_BLUE + 1; uid < FACTION_BLUE + 1 + CONFIG_UNIT_AMT_ALLY; uid++)
    {
        unit = GetUnit(uid);
        if (UNIT_IS_VALID(unit))
        {
            ClearBitUES(unit, UES_BIT_GUARDBEAR_SKILL_USED);
            ClearBitUES(unit, UES_BIT_TSZUKU_SKILL_USED);
            ClearBitUES(unit, UES_BIT_TELEPOTRATION_SKILL_USED);
        }
    }

    for (uid = FACTION_GREEN + 1; uid < FACTION_GREEN + 1 + CONFIG_UNIT_AMT_ALLY; uid++)
    {
        unit = GetUnit(uid);
        if (UNIT_IS_VALID(unit))
        {
            ClearBitUES(unit, UES_BIT_GUARDBEAR_SKILL_USED);
            ClearBitUES(unit, UES_BIT_TSZUKU_SKILL_USED);
            ClearBitUES(unit, UES_BIT_TELEPOTRATION_SKILL_USED);
        }
    }

    for (uid = FACTION_RED + 1; uid < FACTION_RED + 1 + CONFIG_UNIT_AMT_ALLY; uid++)
    {
        unit = GetUnit(uid);
        if (UNIT_IS_VALID(unit))
        {
            ClearBitUES(unit, UES_BIT_GUARDBEAR_SKILL_USED);
            ClearBitUES(unit, UES_BIT_TSZUKU_SKILL_USED);
            ClearBitUES(unit, UES_BIT_TELEPOTRATION_SKILL_USED);
        }
    }
    return false;
}

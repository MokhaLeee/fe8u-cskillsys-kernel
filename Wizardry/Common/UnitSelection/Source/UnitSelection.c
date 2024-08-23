#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

//! FE8U = 0x0801D51C
LYN_REPLACE_CHECK(GetPlayerSelectKind);
int GetPlayerSelectKind(struct Unit * unit)
{
    u8 faction = gPlaySt.faction;

    if (!unit)
    {
        return PLAYER_SELECT_NOUNIT;
    }

    if (gBmSt.gameStateBits & BM_FLAG_PREPSCREEN)
    {
        if (!CanCharacterBePrepMoved(unit->pCharacterData->number))
        {
            return PLAYER_SELECT_4;
        }

        faction = FACTION_BLUE;
    }

    if (!unit)
    {
        return PLAYER_SELECT_NOUNIT;
    }

    if (UNIT_FACTION(unit) != faction)
    {
        return PLAYER_SELECT_NOCONTROL;
    }

    if (unit->state & US_UNSELECTABLE)
    {
        return PLAYER_SELECT_TURNENDED;
    }

    if (UNIT_CATTRIBUTES(unit) & CA_UNSELECTABLE)
    {
        return PLAYER_SELECT_TURNENDED;
    }

    if ((unit->statusIndex != UNIT_STATUS_SLEEP) && (unit->statusIndex != UNIT_STATUS_BERSERK))
    {
        return PLAYER_SELECT_CONTROL;
    }

#if (defined(SID_Comatose) && (COMMON_SKILL_VALID(SID_Comatose)))
    if (SkillTester(unit, SID_Comatose) && unit->statusIndex == UNIT_STATUS_SLEEP)
        return PLAYER_SELECT_CONTROL;
#endif

    return PLAYER_SELECT_NOCONTROL;
}


//! FE8U = 0x0801D70C
LYN_REPLACE_CHECK(MakeMoveunitForActiveUnit);
void MakeMoveunitForActiveUnit(void)
{
    if (!MuExists())
    {
        if (UNIT_FACTION(gActiveUnit) == gPlaySt.faction)
        {
#if (defined(SID_Comatose) && (COMMON_SKILL_VALID(SID_Comatose)))
            if (SkillTester(gActiveUnit, SID_Comatose) && gActiveUnit->statusIndex == UNIT_STATUS_SLEEP)
            {
                StartMu(gActiveUnit);
                HideUnitSprite(gActiveUnit);
            }
#endif
            if ((gActiveUnit->statusIndex != UNIT_STATUS_SLEEP) && (gActiveUnit->statusIndex != UNIT_STATUS_BERSERK))
            {
                StartMu(gActiveUnit);
                HideUnitSprite(gActiveUnit);
            }
        }
    }

    SetAutoMuDefaultFacing();

    return;
}
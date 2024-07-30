#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

static void _SetArmorMarchStatDebuff(struct Unit * unit)
{
    SetUnitStatDebuff(unit, UNIT_STAT_BUFF_ARMOR_MARCH);
}

bool PrePhase_TickArmorMarchSkillStatus(ProcPtr proc)
{
    int i, j;

    for (i = gPlaySt.faction + 1; i <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); ++i)
    {
        bool armor_march  = false;

        struct Unit * unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
            continue;

#if (defined(SID_ArmorMarch) && (COMMON_SKILL_VALID(SID_ArmorMarch)))
        if (SkillTester(unit, SID_ArmorMarch))
            armor_march = true;
#endif

        for (j = 0; j < ARRAY_COUNT_RANGE1x1; j++)
        {
            int _x = unit->xPos + gVecs_1x1[j].x;
            int _y = unit->yPos + gVecs_1x1[j].y;

            struct Unit * unit_ally = GetUnitAtPosition(_x, _y);
            if (!UNIT_IS_VALID(unit_ally))
                continue;

            if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;

            if (!AreUnitsAllied(unit->index, unit_ally->index))
                continue;

            if (!armor_march)
                continue;

            switch (unit_ally->pClassData->number) {
            case CLASS_ARMOR_KNIGHT:
            case CLASS_ARMOR_KNIGHT_F:
            case CLASS_GENERAL:
            case CLASS_GENERAL_F:
            case CLASS_GREAT_KNIGHT:
            case CLASS_GREAT_KNIGHT_F:
                _SetArmorMarchStatDebuff(unit);
                _SetArmorMarchStatDebuff(unit_ally);
                break;
            default:
                break;
            }
        }
    }
    return false;
}

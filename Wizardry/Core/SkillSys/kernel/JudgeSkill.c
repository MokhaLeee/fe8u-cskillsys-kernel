#include "global.h"
#include "bmunit.h"

#include "common-chax.h"
#include "skill-system.h"

static bool SkillTesterROM(struct Unit * unit, const u8 sid)
{
    if (gpConstSkillPTable[2 * UNIT_CHAR_ID(unit) + 0] == sid)
        return true;

    if (gpConstSkillPTable[2 * UNIT_CHAR_ID(unit) + 1] == sid)
        return true;

    if (gpConstSkillJTable[2 * UNIT_CLASS_ID(unit) + 0] == sid)
        return true;

    if (gpConstSkillJTable[2 * UNIT_CLASS_ID(unit) + 1] == sid)
        return true;

    return false;
}

static bool SkillTesterRAM(struct Unit * unit, const u8 sid)
{
    int i;
    u8 * list = UNIT_RAM_SKILLS(unit);
    for (i = 0; i < UNIT_RAM_SKILLS_LEN; i++)
        if (sid == list[i])
            return true;

    return false;
}

bool SkillTesterBasic(struct Unit * unit, const u8 sid)
{
    int ret;

    if (!UNIT_IS_VALID(unit) || !SKILL_VALID(sid))
        return false;

    ret = SkillTesterROM(unit, sid);
    if (ret == true)
        return true;

    ret = SkillTesterRAM(unit, sid);
    if (ret == true)
        return true;

    return false;
}

bool SkillTesterFast(struct Unit * unit, const u8 sid)
{
    int i;
    struct SkillList * list = GetUnitSkillList(unit);

    if (list)
    {
        for (i = 0; i < list->amt; i++)
        {
            if (list->sid[i] == sid)
                return true;
        }
    }
    return false;
}

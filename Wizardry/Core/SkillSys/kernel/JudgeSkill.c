#include "global.h"
#include "bmunit.h"

#include "common-chax.h"
#include "skill-system.h"

bool _SkillTester(struct Unit * unit, const u8 sid)
{
    u8 pid, jid;

    if (!UNIT_IS_VALID(unit) || !SKILL_VALID(sid))
        return false;

    /* RAM skill */
    if (UNIT_RAM_SKILLS(unit)[0] == sid) return true;
    if (UNIT_RAM_SKILLS(unit)[1] == sid) return true;
    if (UNIT_RAM_SKILLS(unit)[2] == sid) return true;
    if (UNIT_RAM_SKILLS(unit)[3] == sid) return true;
    if (UNIT_RAM_SKILLS(unit)[4] == sid) return true;
    if (UNIT_RAM_SKILLS(unit)[5] == sid) return true;

    /* ROM skill on pid */
    jid = UNIT_CLASS_ID(unit);
    if (gpConstSkillJTable[jid * 2 + 0] == sid) return true;
    if (gpConstSkillJTable[jid * 2 + 1] == sid) return true;

    /* ROM skill on jid */
    pid = UNIT_CHAR_ID(unit);
    if (gpConstSkillPTable[pid * 2 + 0] == sid) return true;
    if (gpConstSkillPTable[pid * 2 + 1] == sid) return true;

    return false;
}

bool _SkillTester_Unused(struct Unit * unit, const u8 sid)
{
    int i;
    struct SkillList * list;

    list = GetUnitSkillList(unit);
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

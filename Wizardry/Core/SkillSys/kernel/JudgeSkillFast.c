#include "common-chax.h"
#include "skill-system.h"

/**
 * Since I think the function SkillTesterFast()
 * is not fast enough
 *
 * So I decided to write a more fasten function.
 */
bool SkillTesterFastFast(struct Unit * unit, const u8 sid)
{
    #define CHECK_SKILL(val, sid)   \
    do {                            \
        if ((val) == (sid))         \
            return true;            \
    } while (0)

    int ref;
    u8 pid, jid;

    CHECK_SKILL(UNIT_RAM_SKILLS(unit)[0], sid);
    CHECK_SKILL(UNIT_RAM_SKILLS(unit)[1], sid);
    CHECK_SKILL(UNIT_RAM_SKILLS(unit)[2], sid);
    CHECK_SKILL(UNIT_RAM_SKILLS(unit)[3], sid);
    CHECK_SKILL(UNIT_RAM_SKILLS(unit)[4], sid);
    CHECK_SKILL(UNIT_RAM_SKILLS(unit)[5], sid);
    CHECK_SKILL(UNIT_RAM_SKILLS(unit)[6], sid);

    pid = UNIT_CHAR_ID(unit);
    ref = pid * 2;
    CHECK_SKILL(gpConstSkillPTable[ref + 0], sid);
    CHECK_SKILL(gpConstSkillPTable[ref + 1], sid);

    jid = UNIT_CLASS_ID(unit);
    ref = jid * 2;
    CHECK_SKILL(gpConstSkillJTable[ref + 0], sid);
    CHECK_SKILL(gpConstSkillJTable[ref + 1], sid);

    return false;
}

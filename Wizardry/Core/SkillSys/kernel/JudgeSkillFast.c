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
    const u8 * list;

    list = UNIT_RAM_SKILLS(unit);
    CHECK_SKILL(list[0], sid);
    CHECK_SKILL(list[1], sid);
    CHECK_SKILL(list[2], sid);
    CHECK_SKILL(list[3], sid);
    CHECK_SKILL(list[4], sid);
    CHECK_SKILL(list[5], sid);
    CHECK_SKILL(list[6], sid);

    jid = UNIT_CLASS_ID(unit);
    ref = jid * 2;
    list = gpConstSkillJTable;
    CHECK_SKILL(list[ref + 0], sid);
    CHECK_SKILL(list[ref + 1], sid);

    pid = UNIT_CHAR_ID(unit);
    ref = pid * 2;
    list = gpConstSkillPTable;
    CHECK_SKILL(list[ref + 0], sid);
    CHECK_SKILL(list[ref + 1], sid);

    return false;
}

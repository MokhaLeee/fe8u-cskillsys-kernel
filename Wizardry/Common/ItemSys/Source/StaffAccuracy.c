#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"
#include "constants/items.h"
#include "constants/classes.h"
#include "constants/characters.h"

#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "strmag.h"
#include "constants/skills.h"

int GetOffensiveStaffAccuracy(struct Unit * actor, struct Unit * target)
{
    int baseAccuracy = (MagGetter(actor) - ResGetter(target)) * 5;
    int unitSkill = SklGetter(actor);
    int distance = RECT_DISTANCE(actor->xPos, actor->yPos, target->xPos, target->yPos);
    int result = (baseAccuracy + 30 + unitSkill) - distance * 2;

    LIMIT_AREA(result, 0, 100);
    return result;
}

#include "common-chax.h"
#include "debug-event.h"
#include "skill-system.h"
#include "constants/skills.h"

void PrologueCallBack(void)
{
	struct Unit *unit = GetUnitFromCharId(CHARACTER_EPHRAIM);

	if (unit) {
#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
		UnitAddItem(unit, (SID_Fury << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL);
#endif
	}
}

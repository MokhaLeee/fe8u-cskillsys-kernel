#include "common-chax.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "action-expa.h"
#include "unit-expa.h"
#include "constants/texts.h"
#include "constants/skills.h"

#if defined(SID_Teleportation) && (COMMON_SKILL_VALID(SID_Teleportation))
STATIC_DECLAR bool AreAnyEnemyExists(void)
{
	int i;

	for (i = 1; i < 0xC0; i++) {
		struct Unit *unit = GetUnit(i);

		if (UnitOnMapAvaliable(unit) && !AreUnitsAllied(gActiveUnit->index, i))
			return true;
	}
	return false;
}

u8 Teleportation_Usability(const struct MenuItemDef *def, int number)
{
	if (gActiveUnit->state & US_CANTOING || CheckBitUES(gActiveUnit, UES_BIT_TSZUKU_SKILL_USED))
		return MENU_NOTSHOWN;

	if ((GetUnitCurrentHp(gActiveUnit) * 100) < (GetUnitMaxHp(gActiveUnit) * SKILL_EFF0(SID_Teleportation)))
		return MENU_DISABLED;

	if (!AreAnyEnemyExists())
		return MENU_DISABLED;

	return MENU_ENABLED;
}

u8 Teleportation_OnSelected(struct MenuProc *menu, struct MenuItemProc *item)
{
	struct Vec2 vec;

	if (item->availability == MENU_DISABLED) {
		MenuFrozenHelpBox(menu, MSG_MenuSkill_CommonFail);
		return MENU_ACT_SND6B;
	}

	if (GetTeleportationRandomPosition(gActiveUnit, &vec) == true) {
		gActionData.xMove = vec.x;
		gActionData.yMove = vec.y;
	} else {
		/* Yeah, Who said you can't teleport without movement LOL */
		gActionData.xMove = gActiveUnit->xPos;
		gActionData.yMove = gActiveUnit->yPos;
	}

	gActionData.subjectIndex = gActiveUnit->index;
	gActionData.unk08 = SID_Teleportation;
	gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;
	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}
#endif

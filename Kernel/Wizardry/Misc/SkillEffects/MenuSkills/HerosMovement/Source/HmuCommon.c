#include "common-chax.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "heros-movement.h"

const u16 HerosMovementSkills[HMU_TYPE_COUNT] = {
#if (defined(SID_Pivot) && COMMON_SKILL_VALID(SID_Pivot))
	[HMU_PIVOT] = SID_Pivot,
#endif

#if (defined(SID_Reposition) && COMMON_SKILL_VALID(SID_Reposition))
	[HMU_REPOSITION] = SID_Reposition,
#endif

#if (defined(SID_Swap) && COMMON_SKILL_VALID(SID_Swap))
	[HMU_SWAP] = SID_Swap,
#endif
};

STATIC_DECLAR bool HerosMovementSkillRequired(void)
{
	return !!(gpKernelDesigerConfig->heros_movement_skill_required);
}

static inline int GetHmuType(struct Unit *unit)
{
	return gpHerosMovementTypes[UNIT_CLASS_ID(gActiveUnit)];
}

u8 HerosMovement_UM_Usability(const struct MenuItemDef *def, int number)
{
	u16 sid;
	int hmu_type;
	const struct MenuItemDef *sdef;

	if (HerosMovementSkillRequired())
		return MENU_NOTSHOWN;

	hmu_type = GetHmuType(gActiveUnit);
	if (hmu_type == HMU_TYPE_INVALID)
		return MENU_NOTSHOWN;

	sid = HerosMovementSkills[hmu_type];
	if (!COMMON_SKILL_VALID(sid))
		return MENU_NOTSHOWN;

	/**
	 * If unit already hold the skill,
	 * then he may use this skill in skill submenu.
	 */
	if (SkillListTester(gActiveUnit, sid))
		return MENU_NOTSHOWN;

	sdef = GetSkillMenuInfo(sid);
	if (!sdef || !sdef->isAvailable)
		return MENU_NOTSHOWN;

	return sdef->isAvailable(sdef, number);
}

int HerosMovement_UM_OnDarw(struct MenuProc *menu, struct MenuItemProc *item)
{
	u16 sid = HerosMovementSkills[GetHmuType(gActiveUnit)];
	const struct MenuItemDef *def = GetSkillMenuInfo(sid);

	if (def->color)
		Text_SetColor(&item->text, def->color);

	if (item->availability == MENU_DISABLED)
		Text_SetColor(&item->text, TEXT_COLOR_SYSTEM_GRAY);
	if (def->nameMsgId)
		Text_DrawString(&item->text, GetStringFromIndex(def->nameMsgId));
	else if (gpKernelDesigerConfig->menu_skill_disp_msg_en_n == true)
		Text_DrawString(&item->text, GetMenuSkillNameStr(sid));
	else
		Text_DrawString(&item->text, def->name);

	PutText(
		&item->text,
		TILEMAP_LOCATED(BG_GetMapBuffer(menu->frontBg), item->xTile, item->yTile));

	return 0;
}

u8 HerosMovement_UM_Effect(struct MenuProc *menu, struct MenuItemProc *item)
{
	u16 sid = HerosMovementSkills[GetHmuType(gActiveUnit)];
	const struct MenuItemDef *def = GetSkillMenuInfo(sid);

	if (def && def->onSelected)
		return def->onSelected(menu, item);

	return 0;
}

int HerosMovement_UM_Hover(struct MenuProc *menu, struct MenuItemProc *item)
{
	u16 sid = HerosMovementSkills[GetHmuType(gActiveUnit)];
	const struct MenuItemDef *def = GetSkillMenuInfo(sid);

	if (def && def->onSwitchIn)
		return def->onSwitchIn(menu, item);

	return 0;
}

int HerosMovement_UM_Unhover(struct MenuProc *menu, struct MenuItemProc *item)
{
	u16 sid = HerosMovementSkills[GetHmuType(gActiveUnit)];
	const struct MenuItemDef *def = GetSkillMenuInfo(sid);

	if (def && def->onSwitchOut)
		return def->onSwitchOut(menu, item);

	return 0;
}

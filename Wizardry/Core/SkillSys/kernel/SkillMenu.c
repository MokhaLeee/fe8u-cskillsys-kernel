#include "common-chax.h"
#include "skill-system.h"

STATIC_DECLAR void GenerateUnitMenuSkillList(struct Unit *unit)
{
	int i, cnt;
	struct SkillList *list = GetUnitSkillList(unit);

	memset(UnitMenuSkills, 0, sizeof(UnitMenuSkills));

	cnt = 0;
	for (i = 0; i < list->amt; i++) {
		u16 sid;

		if (cnt > UNIT_MENU_SKILL_AMOUNT)
			break;

		sid = list->sid[i];
		if (GetSkillMenuInfo(sid)->isAvailable)
			UnitMenuSkills[cnt++] = sid;
	}
}

u8 MenuSkills_OnHelpBox(struct MenuProc *menu, struct MenuItemProc *item)
{
	if (IS_SKILL_MENU_ITEM(item->def)) {
		u16 sid = UnitMenuSkills[MENU_SKILL_INDEX(item->def)];

		StartHelpBox(item->xTile * 8, item->yTile * 8, GetSkillMenuInfo(sid)->helpMsgId);
		return 0;
	}

	MenuStdHelpBox(menu, item);
	return 0;
}

u8 MenuSkills_Usability(const struct MenuItemDef *self, int number)
{
	u16 sid;

	if (MENU_SKILL_INDEX(self) == 0) {
		/* On init */
		GenerateUnitMenuSkillList(gActiveUnit);
	}

	sid = UnitMenuSkills[MENU_SKILL_INDEX(self)];
	if (!COMMON_SKILL_VALID(sid) || !GetSkillMenuInfo(sid)->isAvailable)
		return MENU_NOTSHOWN;

	return GetSkillMenuInfo(sid)->isAvailable(self, number);
}

static int MenuSkills_StandardDraw(struct MenuProc *menu, struct MenuItemProc *item)
{
	const struct MenuItemDef *def = GetSkillMenuInfo(UnitMenuSkills[MENU_SKILL_INDEX(item->def)]);

	if (def->color)
		Text_SetColor(&item->text, def->color);

	if (item->availability == MENU_DISABLED)
		Text_SetColor(&item->text, TEXT_COLOR_SYSTEM_GRAY);

	if (!def->nameMsgId)
		Text_DrawString(&item->text, def->name);
	else
		Text_DrawString(&item->text, GetStringFromIndex(def->nameMsgId));

	PutText(
		&item->text,
		TILEMAP_LOCATED(BG_GetMapBuffer(menu->frontBg), item->xTile, item->yTile));

	return 0;
}

int MenuSkills_OnDraw(struct MenuProc *menu, struct MenuItemProc *item)
{
	u16 sid = UnitMenuSkills[MENU_SKILL_INDEX(item->def)];

	if (GetSkillMenuInfo(sid)->onDraw)
		return GetSkillMenuInfo(sid)->onDraw(menu, item);

	return MenuSkills_StandardDraw(menu, item);
}

u8 MenuSkills_OnSelected(struct MenuProc *menu, struct MenuItemProc *item)
{
	u16 sid = UnitMenuSkills[MENU_SKILL_INDEX(item->def)];

	if (GetSkillMenuInfo(sid)->onSelected)
		return GetSkillMenuInfo(sid)->onSelected(menu, item);

	return 0;
}

u8 MenuSkills_Idle(struct MenuProc *menu, struct MenuItemProc *item)
{
	u16 sid = UnitMenuSkills[MENU_SKILL_INDEX(item->def)];

	if (GetSkillMenuInfo(sid)->onIdle)
		return GetSkillMenuInfo(sid)->onIdle(menu, item);

	return 0;
}

int MenuSkills_Hover(struct MenuProc *menu, struct MenuItemProc *item)
{
	u16 sid = UnitMenuSkills[MENU_SKILL_INDEX(item->def)];

	if (GetSkillMenuInfo(sid)->onSwitchIn)
		return GetSkillMenuInfo(sid)->onSwitchIn(menu, item);

	return 0;
}

int MenuSkills_Unhover(struct MenuProc *menu, struct MenuItemProc *item)
{
	u16 sid = UnitMenuSkills[MENU_SKILL_INDEX(item->def)];

	if (GetSkillMenuInfo(sid)->onSwitchOut)
		return GetSkillMenuInfo(sid)->onSwitchOut(menu, item);

	return 0;
}

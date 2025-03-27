#include "common-chax.h"
#include "skill-system.h"

/**
 * Upper menu
 */
#ifdef CONFIG_MENU_SKILL_NOT_IN_UPPER

STATIC_DECLAR const struct MenuItemDef sUnitSkillMenuItems[] = {
	{
		"　特技",
		0x4EC,
		0,
		TEXT_COLOR_SYSTEM_WHITE,
		0x54,
		MenuSkills_Usability,
		MenuSkills_OnDraw,
		MenuSkills_OnSelected,
		MenuSkills_Idle,
		MenuSkills_Hover,
		MenuSkills_Unhover
	},
	{
		"　特技",
		0x4EC,
		1,
		TEXT_COLOR_SYSTEM_WHITE,
		0x55,
		MenuSkills_Usability,
		MenuSkills_OnDraw,
		MenuSkills_OnSelected,
		MenuSkills_Idle,
		MenuSkills_Hover,
		MenuSkills_Unhover
	},
	{
		"　特技",
		0x4EC,
		2,
		TEXT_COLOR_SYSTEM_WHITE,
		0x56,
		MenuSkills_Usability,
		MenuSkills_OnDraw,
		MenuSkills_OnSelected,
		MenuSkills_Idle,
		MenuSkills_Hover,
		MenuSkills_Unhover
	},
	{
		"　特技",
		0x4EC,
		3,
		TEXT_COLOR_SYSTEM_WHITE,
		0x57,
		MenuSkills_Usability,
		MenuSkills_OnDraw,
		MenuSkills_OnSelected,
		MenuSkills_Idle,
		MenuSkills_Hover,
		MenuSkills_Unhover
	},
	MenuItemsEnd
};

STATIC_DECLAR const struct MenuDef sUnitSkillMenuDef = {
	{1, 2, 13, 0},
	0,
	sUnitSkillMenuItems,
	0, 0, 0,
	ItemMenu_ButtonBPressed,
	MenuAutoHelpBoxSelect,
	MenuSkills_OnHelpBox
};

u8 UpperMenuSkill_Usability(const struct MenuItemDef *self, int number)
{
	int i;
	const struct MenuItemDef *items = sUnitSkillMenuDef.menuItems;

	for (i = 0; items[i].isAvailable != NULL; i++) {
		int ava = items[i].isAvailable(&items[i], i);

		if (ava != MENU_NOTSHOWN)
			return MENU_ENABLED;
	}
	return MENU_NOTSHOWN;
}

u8 UpperMenuSkill_OnSelected(struct MenuProc *menu, struct MenuItemProc *item)
{
#if 0
	StartOrphanMenuAdjusted(
		&sUnitSkillMenuDef,
		gBmSt.cursorTarget.x - gBmSt.camera.x,
		22, 1
	);
#else
	const struct MenuDef *def = &sUnitSkillMenuDef;
	struct MenuRect rect = def->rect;

	rect.w = gpKernelDesigerConfig->skill_sub_menu_width;

	if (item->xTile < 12)
		rect.x = 1;
	else
		rect.x = 22 + 7 - rect.w;

	StartMenuAt(def, rect, NULL);
#endif

	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

#endif /* MENU_SKILL_NOT_IN_UPPER */

/**
 * Ext
 */
STATIC_DECLAR void GenerateUnitMenuSkillList(struct Unit *unit)
{
	int i, cnt;
	struct SkillList *list;

#if 1
	list = GetUnitSkillList(unit);
#else
	BattleGenerateUiStats(unit, -1);
	list = GetUnitSkillList(&gBattleActor.unit);
#endif

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

STATIC_DECLAR int MenuSkills_StandardDraw(struct MenuProc *menu, struct MenuItemProc *item)
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

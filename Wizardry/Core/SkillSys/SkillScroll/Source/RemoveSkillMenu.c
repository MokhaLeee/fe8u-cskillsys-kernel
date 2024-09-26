#include "common-chax.h"
#include "icon-rework.h"
#include "skill-system.h"

STATIC_DECLAR const struct MenuItemDef RemoveSkillMenuItems[];
STATIC_DECLAR u8 RemoveSkillMenu_HelpBox(struct MenuProc *menu, struct MenuItemProc *item);
STATIC_DECLAR u8 RemoveSkillMenu_OnCancel(struct MenuProc *menu, struct MenuItemProc *item);

const struct MenuDef RemoveSkillMenuDef = {
	{1, 1, 14, 0},
	0,
	RemoveSkillMenuItems,
	0, 0, 0,
	RemoveSkillMenu_OnCancel,
	MenuAutoHelpBoxSelect,
	RemoveSkillMenu_HelpBox
};

STATIC_DECLAR u8 RemoveSkillMenu_HelpBox(struct MenuProc *menu, struct MenuItemProc *item)
{
	StartHelpBox(
		item->xTile * 8,
		item->yTile * 8,
		GetSkillDescMsg(UNIT_RAM_SKILLS(gActiveUnit)[MENU_SKILL_INDEX(item->def)])
	);
	return 0;
}

STATIC_DECLAR u8 RemoveSkillMenu_OnCancel(struct MenuProc *menu, struct MenuItemProc *item)
{
	/* Reset action */
	gActionData.unitActionType = 0;

	BG_Fill(gBG2TilemapBuffer, 0);
	BG_EnableSyncByMask(BG2_SYNC_BIT);
	HideMoveRangeGraphics();

	return ItemCommandEffect(menu, item);
}

STATIC_DECLAR u8 RemoveSkillMenu_Usability(const struct MenuItemDef *self, int number);
STATIC_DECLAR int RemoveSkillMenu_OnDraw(struct MenuProc *menu, struct MenuItemProc *item);
STATIC_DECLAR u8 RemoveSkillMenu_OnSelected(struct MenuProc *menu, struct MenuItemProc *item);

#define RemoveSkillMenuItem(i) \
{ \
	.helpMsgId = (i), \
	.isAvailable = RemoveSkillMenu_Usability, \
	.onDraw = RemoveSkillMenu_OnDraw, \
	.onSelected = RemoveSkillMenu_OnSelected, \
}

STATIC_DECLAR const struct MenuItemDef RemoveSkillMenuItems[] = {
	RemoveSkillMenuItem(0),
	RemoveSkillMenuItem(1),
	RemoveSkillMenuItem(2),
	RemoveSkillMenuItem(3),
	RemoveSkillMenuItem(4),
	RemoveSkillMenuItem(5),
	RemoveSkillMenuItem(6),

	{ 0 }
};

STATIC_DECLAR u8 RemoveSkillMenu_Usability(const struct MenuItemDef *self, int number)
{
	int sid = UNIT_RAM_SKILLS(gActiveUnit)[MENU_SKILL_INDEX(self)];

	if (EQUIPE_SKILL_VALID(sid))
		return MENU_ENABLED;

	return MENU_NOTSHOWN;
}

STATIC_DECLAR int RemoveSkillMenu_OnDraw(struct MenuProc *menu, struct MenuItemProc *item)
{
	int sid = UNIT_RAM_SKILLS(gActiveUnit)[MENU_SKILL_INDEX(item->def)];

	Text_SetColor(&item->text, TEXT_COLOR_SYSTEM_WHITE);
	Text_DrawString(&item->text, GetSkillNameStr(sid));
	DrawIcon(
		TILEMAP_LOCATED(gBG0TilemapBuffer, item->xTile, item->yTile),
		SKILL_ICON(sid), 0x4000);

	PutText(
		&item->text,
		TILEMAP_LOCATED(gBG0TilemapBuffer, item->xTile + 2, item->yTile));

	BG_EnableSyncByMask(BG0_SYNC_BIT);
	return 0;
}

STATIC_DECLAR u8 RemoveSkillMenu_OnSelected(struct MenuProc *menu, struct MenuItemProc *item)
{
	SetItemUseAction(gActiveUnit);
	gActionData.unk08 = MENU_SKILL_INDEX(item->def);
	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

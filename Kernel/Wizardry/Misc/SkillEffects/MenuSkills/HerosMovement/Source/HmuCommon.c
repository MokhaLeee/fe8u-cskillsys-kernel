#include "common-chax.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "heros-movement.h"

extern EWRAM_OVERLAY(0) struct {
	s8 type;
	u8 _pad_[3];
} sHerosMovementState;

const u16 HerosMovementSkills[HMU_TYPE_COUNT] = {
#if (defined(SID_Pivot) && COMMON_SKILL_VALID(SID_Pivot))
	[HMU_PIVOT] = SID_Pivot,
#endif
};

STATIC_DECLAR bool HerosMovementSkillRequired(void)
{
	return !!(gpKernelDesigerConfig->heros_movement_skill_required);
}

static bool check_matched(int type, u8 jid)
{
	const u8 *it;

	switch (type) {
	case HMU_PIVOT:
		it = gpPivotClassList;
		break;

	case HMU_REPOSITION:
		it = gpPivotClassList;
		break;

	case HMU_SWAP:
		it = gpPivotClassList;
		break;

	case HMU_SHOVE:
		it = gpPivotClassList;
		break;

	case HMU_SMITE:
		it = gpPivotClassList;
		break;

	case HMU_SWARP:
		it = gpPivotClassList;
		break;

	default:
		return false;
	}

	while (*it)
		if (*(it++) == jid)
			return true;

	return false;
}

static void SetupHerosMovementState(void)
{
	int i;
	u8 jid = UNIT_CLASS_ID(gActiveUnit);

	sHerosMovementState.type = HMU_TYPE_INVALID;
	for (i = 0; i < HMU_TYPE_COUNT; i++) {
		if (check_matched(i, jid)) {
			sHerosMovementState.type = i;
			break;
		}
	}
}

u8 HerosMovement_UM_Usability(const struct MenuItemDef *def, int number)
{
	u16 sid;
	const struct MenuItemDef *sdef;

	if (HerosMovementSkillRequired())
		return MENU_NOTSHOWN;

	SetupHerosMovementState();

	if (sHerosMovementState.type == HMU_TYPE_INVALID)
		return MENU_NOTSHOWN;

	sid = HerosMovementSkills[sHerosMovementState.type];
	if (!COMMON_SKILL_VALID(sid))
		return MENU_NOTSHOWN;

	sdef = GetSkillMenuInfo(sid);
	if (!sdef || !sdef->isAvailable)
		return MENU_NOTSHOWN;

	return sdef->isAvailable(sdef, number);
}

int HerosMovement_UM_OnDarw(struct MenuProc *menu, struct MenuItemProc *item)
{
	u16 sid = HerosMovementSkills[sHerosMovementState.type];
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
	u16 sid = HerosMovementSkills[sHerosMovementState.type];
	const struct MenuItemDef *def = GetSkillMenuInfo(sid);

	if (def && def->onSelected)
		return def->onSelected(menu, item);

	return 0;
}

int HerosMovement_UM_Hover(struct MenuProc *menu, struct MenuItemProc *item)
{
	u16 sid = HerosMovementSkills[sHerosMovementState.type];
	const struct MenuItemDef *def = GetSkillMenuInfo(sid);

	if (def && def->onSwitchIn)
		return def->onSwitchIn(menu, item);

	return 0;
}

int HerosMovement_UM_Unhover(struct MenuProc *menu, struct MenuItemProc *item)
{
	u16 sid = HerosMovementSkills[sHerosMovementState.type];
	const struct MenuItemDef *def = GetSkillMenuInfo(sid);

	if (def && def->onSwitchOut)
		return def->onSwitchOut(menu, item);

	return 0;
}

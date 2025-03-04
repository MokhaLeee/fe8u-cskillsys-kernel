#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

#if defined(SID_Blacksmith) && (COMMON_SKILL_VALID(SID_Blacksmith))
u8 Blacksmith_Usability(const struct MenuItemDef *def, int number)
{
	int partyGold = GetPartyGoldAmount();
	int slot = GetUnitEquippedWeaponSlot(gActiveUnit);
	u16 weapon = GetItemFromSlot(gActiveUnit, slot);

	if (gActiveUnit->state & US_CANTOING)
		return MENU_NOTSHOWN;

	if (!weapon)
		return MENU_NOTSHOWN;

	switch (slot) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		break;

	default:
		return MENU_NOTSHOWN;
	}

	if (partyGold < 1000)
		return MENU_NOTSHOWN;

	if (GetItemAttributes(weapon) & IA_UNBREAKABLE)
		return MENU_NOTSHOWN;

	if (GetItemUses(weapon) >= GetItemMaxUses(weapon))
		return MENU_NOTSHOWN;

	if (GetItemRequiredExp(weapon) >= WPN_EXP_B)
		return MENU_NOTSHOWN;

	switch (GetItemType(weapon)) {
	case ITYPE_ANIMA:
	case ITYPE_LIGHT:
	case ITYPE_DARK:
	case ITYPE_STAFF:
		return MENU_NOTSHOWN;
	}

	return MENU_ENABLED;

}

u8 Blacksmith_OnSelected(struct MenuProc *menu, struct MenuItemProc *item)
{
	if (item->availability == MENU_DISABLED) {
		MenuFrozenHelpBox(menu, MSG_MenuSkill_CommonFail);
		return MENU_ACT_SND6B;
	}

	gActionData.unk08 = SID_Blacksmith;
	gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

static void callback_anim(ProcPtr proc)
{
	PlaySoundEffect(0x269);
	Proc_StartBlocking(ProcScr_DanceringAnim, proc);

	BG_SetPosition(
		BG_0,
		-SCREEN_TILE_IX(gActiveUnit->xPos - 1),
		-SCREEN_TILE_IX(gActiveUnit->yPos - 2));
}

static void callback_exec(ProcPtr proc)
{
	int weaponSlot = GetUnitEquippedWeaponSlot(gActiveUnit);
	int partyGold = GetPartyGoldAmount();
	u16 weapon = gActiveUnit->items[weaponSlot];

	SetPartyGoldAmount(partyGold - 1000);
	gActiveUnit->items[weaponSlot] = MakeNewItem(weapon);

	NewPopup_VerySimple(MSG_ThousandGoldSpent, 0x5A, proc);
}

bool Action_Blacksmith(ProcPtr parent)
{
	NewMuSkillAnimOnActiveUnitWithDeamon(parent, gActionData.unk08, callback_anim, callback_exec);
	return true;
}
#endif

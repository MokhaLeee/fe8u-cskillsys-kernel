#include <common-chax.h>
#include <battle-system.h>
#include <skill-system.h>
#include <kernel-lib.h>
#include <bwl.h>
#include <list-verify.h>
#include <gaiden-magic.h>

struct GaidenMagicList *const gGaidenMagicList = &sGaidenMagicListObj;

NOINLINE static bool IsBMag(int iid)
{
	u32 attr = GetItemAttributes(iid);

	if (!(attr & IA_WEAPON))
		return false;

	if (gpKernelDesigerConfig->gaiden_magic_must_be_magic)
		if (!(attr & IA_MAGIC))
			return false;

	return true;
}

NOINLINE static bool IsWMag(int iid)
{
	if (GetItemType(iid) != ITYPE_STAFF)
		return false;

	return true;
}

void ResetGaidenMagicList(struct GaidenMagicList *list)
{
	memset(list, 0, sizeof(*list));
}

void UpdateGaidenMagicList(struct Unit *unit, struct GaidenMagicList *list)
{
	int i;
	const struct GaidenPinfoConfigEnt *conf1 = gpGaidenPinfoConfigList[UNIT_CHAR_ID(unit)].ent;
	const struct GaidenChaxConfigEnt  *conf2 = gpGaidenChaxConfigs;

	u8 *tmpbuf = gGenericBuffer;

	WARN_GENERIC_BUF_USED;

	CpuFastFill16(0, tmpbuf, 0x100);

	if (conf1) {
		for (; conf1->iid != ITEM_NONE; conf1++) {
			if ((conf1->level & 0x80) && !(UNIT_CATTRIBUTES(unit) & CA_PROMOTED))
				continue;

			if (unit->level < (conf1->level & 0x7F))
				continue;

			tmpbuf[conf1->iid] = 1;
		}
	}

	if (gpKernelDesigerConfig->gaiden_magic_ext_conf_en) {
		for (; conf2->iid != ITEM_NONE; conf2++) {
			if (conf2->faction != UNIT_FACTION(unit))
				continue;

			if (conf2->pid != 0 && UNIT_CHAR_ID(unit) != conf2->pid)
				continue;

			if (conf2->jid != 0 && UNIT_CLASS_ID(unit) != conf2->jid)
				continue;

			if (unit->level < conf2->level)
				continue;

			if (COMMON_SKILL_VALID(conf2->skill) && !SkillTester(unit, conf2->skill))
				continue;

			if (conf2->evflag != 0 && !CheckFlag(conf2->evflag))
				continue;

			tmpbuf[conf2->iid] = 1;
		}
	}

	ResetGaidenMagicList(list);

	for (i = 0xFF; i > 0; i--) {
		if (list->bmag_cnt >= GAIDEN_MAGIC_LIST_LEN)
			break;

		if (tmpbuf[i] && IsBMag(i))
			list->bmags[list->bmag_cnt++] = i;
	}

	for (i = 0xFF; i > 0; i--) {
		if (list->wmag_cnt >= GAIDEN_MAGIC_LIST_LEN)
			break;

		if (tmpbuf[i] && IsWMag(i))
			list->wmags[list->wmag_cnt++] = i;
	}

	WARN_GENERIC_BUF_RELEASED;
	WriteUnitList(unit, &list->header);
}

struct GaidenMagicList *GetGaidenMagicList(struct Unit *unit)
{
	struct GaidenMagicList *list = gGaidenMagicList;

	if (!JudgeUnitList(unit, &list->header)) {
		UpdateGaidenMagicList(unit, list);
		WriteUnitList(unit, &list->header);
	}
	return list;
}

bool CanUnitUseGaidenMagic(struct Unit *unit, int item)
{
	if (gpKernelDesigerConfig->gaiden_magic_requires_wrank) {
		if (GetItemAttributes(item) & IA_WEAPON)
			return CanUnitUseWeapon(unit, item);
		else if (GetItemType(item) == ITYPE_STAFF)
			return CanUnitUseStaff(unit, item);

		return false;
	}

	/* We are free! */
	return true;
}

bool CanUnitUseGaidenMagicNow(struct Unit *unit, int item)
{
	if ((GetItemAttributes(item) & (IA_MAGIC | IA_STAFF)) && IsUnitMagicSealed(unit))
		return false;

	if (unit->curHP <= GetGaidenWeaponHpCost(unit, item))
		return false;

	return CanUnitUseGaidenMagic(unit, item);
}

int GetGaidenMagicAutoEquipSlot(struct Unit *unit)
{
	int i;
	struct GaidenMagicList *list = GetGaidenMagicList(unit);

	for (i = 0; i < list->bmag_cnt; i++) {
		int item = list->bmags[i];

		if (!(GetItemAttributes(item) & IA_WEAPON))
			continue;

		if (CanUnitUseGaidenMagicNow(unit, item))
			return CHAX_BUISLOT_GAIDEN_BMAG1 + i;
	}

	for (i = 0; i < list->wmag_cnt; i++) {
		int item = list->wmags[i];

		if (!(GetItemAttributes(item) & IA_WEAPON))
			continue;

		if (CanUnitUseGaidenMagicNow(unit, item))
			return CHAX_BUISLOT_GAIDEN_WMAG1 + i;
	}

	return -1;
}

int GetGaidenMagicAutoEquipStaff(struct Unit *unit)
{
	int i;
	struct GaidenMagicList *list = GetGaidenMagicList(unit);

	for (i = 0; i < list->wmag_cnt; i++) {
		int item = list->wmags[i];

		if (!(GetItemAttributes(item) & IA_STAFF))
			continue;

		if (CanUnitUseGaidenMagicNow(unit, item))
			return item;
	}

	for (i = 0; i < list->bmag_cnt; i++) {
		int item = list->bmags[i];

		if (!(GetItemAttributes(item) & IA_STAFF))
			continue;

		if (CanUnitUseGaidenMagicNow(unit, item))
			return item;
	}

	return ITEM_NONE;
}

int GetGaidenMagicItem(struct Unit *unit, int slot)
{
	int list_index;
	struct GaidenMagicList *list = GetGaidenMagicList(unit);

	switch (slot) {
	case CHAX_BUISLOT_GAIDEN_BMAG1 ... CHAX_BUISLOT_GAIDEN_BMAG7:
		list_index = slot - CHAX_BUISLOT_GAIDEN_BMAG1;

		if (list->bmag_cnt > list_index)
			return list->bmags[list_index];

		break;

	case CHAX_BUISLOT_GAIDEN_WMAG1 ... CHAX_BUISLOT_GAIDEN_WMAG7:
		list_index = slot - CHAX_BUISLOT_GAIDEN_WMAG1;

		if (list->wmag_cnt > list_index)
			return list->wmags[list_index];

		break;

	default:
		break;
	}

	return ITEM_NONE;
}

void TryChangeGaidenMagicAction(void)
{
	int item;
	int slot = gActionData.itemSlotIndex;

	switch (slot) {
	case CHAX_BUISLOT_GAIDEN_BMAG1 ... CHAX_BUISLOT_GAIDEN_BMAG7:
	case CHAX_BUISLOT_GAIDEN_WMAG1 ... CHAX_BUISLOT_GAIDEN_WMAG7:
		break;

	default:
		return;
	}

	item = GetItemFromSlot(gActiveUnit, slot);

	if (GetItemAttributes(item) & IA_WEAPON)
		gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat;
	else if (GetItemType(item) == ITYPE_STAFF)
		gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_GaidenMagicStaff;
}

void PutGaidenMagicCostNumber(u16 *tm, int color, int number)
{
	if (number <= 0 || number == 0xFF)
		return;

	while (number != 0) {
		PutSpecialChar(tm, color, k_umod(number, 10) + TEXT_SPECIAL_BIGNUM_0);

		number = k_udiv(number, 10);
		tm--;
	}

	PutSpecialChar(tm, color, TEXT_SPECIAL_DASH);
}

void DrawGaidenMagItemMenuLine(struct Text *text, int item, s8 isUsable, u16 *mapOut)
{
	Text_SetParams(text, 0, (isUsable ? TEXT_COLOR_SYSTEM_WHITE : TEXT_COLOR_SYSTEM_GRAY));
	Text_DrawString(text, GetItemName(item));
	PutText(text, mapOut + 2);
	DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

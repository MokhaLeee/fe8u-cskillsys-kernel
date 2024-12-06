#include <common-chax.h>
#include <battle-system.h>
#include <skill-system.h>
#include <kernel-lib.h>
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
	u32 attr = GetItemAttributes(iid);

	if (attr & IA_STAFF)
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
	const struct GaidenPinfoConfigEnt *conf1 = gGaidenPinfoConfigList[UNIT_CHAR_ID(unit)];
	const struct GaidenChaxConfigEnt  *conf2 = gpGaidenChaxConfigs;

	u8 *tmpbuf = gGenericBuffer;

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

	ResetGaidenMagicList(list);

	for (i = 0xFF; i > 0; i--) {
		if (tmpbuf[i] == 0)
			continue;

		if (IsBMag(i))
			if (list->bmag_cnt < GAIDEN_MAGIC_LIST_LEN)
				list->bmags[list->bmag_cnt++] = i;

		if (IsWMag(i))
			if (list->wmag_cnt < GAIDEN_MAGIC_LIST_LEN)
				list->wmags[list->wmag_cnt++] = i;
	}

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
		u32 attr = GetItemAttributes(item);

		if (attr & IA_WEAPON)
			return CanUnitUseWeapon(unit, item);
		else if (attr & IA_STAFF)
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

	return CanUnitUseGaidenMagic(unit, item);
}

int GetGaidenMagicAutoEquipSlot(struct Unit *unit)
{
	int i;
	struct GaidenMagicList *list = GetGaidenMagicList(unit);

	for (i = 0; i < list->bmag_cnt; i++)
		if (CanUnitUseGaidenMagicNow(unit, list->bmags[i]))
			return CHAX_BUISLOT_GAIDEN_BMAG1 + i;

	for (i = 0; i < list->wmag_cnt; i++)
		if (CanUnitUseGaidenMagicNow(unit, list->wmags[i]))
			return CHAX_BUISLOT_GAIDEN_WMAG1 + i;

	return -1;
}

int GetGaidenMagicItem(struct Unit *unit, int slot)
{
	int list_index;
	struct GaidenMagicList *list = GetGaidenMagicList(unit);

	switch (slot) {
	case CHAX_BUISLOT_GAIDEN_BMAG1:
	case CHAX_BUISLOT_GAIDEN_BMAG2:
	case CHAX_BUISLOT_GAIDEN_BMAG3:
	case CHAX_BUISLOT_GAIDEN_BMAG4:
	case CHAX_BUISLOT_GAIDEN_BMAG5:
	case CHAX_BUISLOT_GAIDEN_BMAG6:
	case CHAX_BUISLOT_GAIDEN_BMAG7:
		list_index = slot - CHAX_BUISLOT_GAIDEN_BMAG1;

		if (list->bmag_cnt > list_index)
			return list->bmags[list_index];

		break;

	case CHAX_BUISLOT_GAIDEN_WMAG1:
	case CHAX_BUISLOT_GAIDEN_WMAG2:
	case CHAX_BUISLOT_GAIDEN_WMAG3:
	case CHAX_BUISLOT_GAIDEN_WMAG4:
	case CHAX_BUISLOT_GAIDEN_WMAG5:
	case CHAX_BUISLOT_GAIDEN_WMAG6:
	case CHAX_BUISLOT_GAIDEN_WMAG7:
		list_index = slot - CHAX_BUISLOT_GAIDEN_WMAG1;

		if (list->wmag_cnt > list_index)
			return list->wmags[list_index];

		break;

	default:
		break;
	}

	return ITEM_NONE;
}

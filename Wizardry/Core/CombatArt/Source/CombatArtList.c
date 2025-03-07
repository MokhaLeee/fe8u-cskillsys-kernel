#include "common-chax.h"
#include "skill-system.h"
#include "combat-art.h"
#include "kernel-lib.h"
#include "constants/combat-arts.h"

extern struct CombatArtList sCombatArtList;

void CalcCombatArtListExt(struct Unit *unit, int item)
{
	int i;
	u8 cid;
	int wtype = GetItemType(item);
	u8 pid = UNIT_CHAR_ID(unit);
	u8 jid = UNIT_CLASS_ID(unit);
	u8 *tmp_list = gGenericBuffer;
	struct SkillList *slist = GetUnitSkillList(unit);

	CpuFill16(0, tmp_list, 0x100);

	/* Skill table */
	if (slist) {
		for (i = 0; i < slist->amt; i++) {
			cid = gpCombatArtSkillTable[slist->sid[i]];

			if (COMBART_VALID(cid))
				tmp_list[cid] = true;
		}
	}

	/* Weapon table */
	cid = gpCombatArtWeaponTable[ITEM_INDEX(item)];
	if (COMBART_VALID(cid))
		tmp_list[cid] = true;

	/* ROM table */
	for (i = WPN_LEVEL_E; i <= WPN_LEVEL_S; i++) {
		if (unit->ranks[ITYPE_SWORD] >= WRankToWExp(i)) {
			cid = gpCombatArtDefaultTable->cid_sword[i];
			if (COMBART_VALID(cid))
				tmp_list[cid] = true;

			cid = gpCombatArtRomPTable[pid].cid_sword[i];
			if (COMBART_VALID(cid))
				tmp_list[cid] = true;

			cid = gpCombatArtRomJTable[jid].cid_sword[i];
			if (COMBART_VALID(cid))
				tmp_list[cid] = true;
		}

		if (unit->ranks[ITYPE_LANCE] >= WRankToWExp(i)) {
			cid = gpCombatArtDefaultTable->cid_lance[i];
			if (COMBART_VALID(cid))
				tmp_list[cid] = true;

			cid = gpCombatArtRomPTable[pid].cid_lance[i];
			if (COMBART_VALID(cid))
				tmp_list[cid] = true;

			cid = gpCombatArtRomJTable[jid].cid_lance[i];
			if (COMBART_VALID(cid))
				tmp_list[cid] = true;
		}

		if (unit->ranks[ITYPE_AXE] >= WRankToWExp(i)) {
			cid = gpCombatArtDefaultTable->cid_axe[i];
			if (COMBART_VALID(cid))
				tmp_list[cid] = true;

			cid = gpCombatArtRomPTable[pid].cid_axe[i];
			if (COMBART_VALID(cid))
				tmp_list[cid] = true;

			cid = gpCombatArtRomJTable[jid].cid_axe[i];
			if (COMBART_VALID(cid))
				tmp_list[cid] = true;
		}

		if (unit->ranks[ITYPE_BOW] >= WRankToWExp(i)) {
			cid = gpCombatArtDefaultTable->cid_bow[i];
			if (COMBART_VALID(cid))
				tmp_list[cid] = true;

			cid = gpCombatArtRomPTable[pid].cid_bow[i];
			if (COMBART_VALID(cid))
				tmp_list[cid] = true;

			cid = gpCombatArtRomJTable[jid].cid_bow[i];
			if (COMBART_VALID(cid))
				tmp_list[cid] = true;
		}
	}

	sCombatArtList.amt = 0;
	for (i = 1; i < 0xFF; i++) {
		if (tmp_list[i] == 0)
			continue;

		if (wtype != CA_WTYPE_ANY && GetCombatArtInfo(i)->wtype != CA_WTYPE_ANY && GetCombatArtInfo(i)->wtype != wtype)
			continue;

		if (ITEM_USES(item) < GetCombatArtInfo(i)->cost)
            continue;

		sCombatArtList.cid[sCombatArtList.amt++] = i;
		if (sCombatArtList.amt >= COMBART_LIST_MAX_AMT)
			break;
	}
}

struct CombatArtList *GetCombatArtList(struct Unit *unit, u16 item)
{
	u8 wtype = GetItemType(item);

	if (sCombatArtList.wtype != wtype || sCombatArtList.item != item || !JudgeUnitList(unit, &sCombatArtList.ref)) {
		CalcCombatArtListExt(unit, item);
		WriteUnitList(unit, &sCombatArtList.ref);
		sCombatArtList.wtype = wtype;
		sCombatArtList.item = item;
	}
	return &sCombatArtList;
}

void ResetCombatArtList(void)
{
	CpuFill16(0, &sCombatArtList, sizeof(sCombatArtList));
}

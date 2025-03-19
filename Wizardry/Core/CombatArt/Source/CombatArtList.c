#include "common-chax.h"
#include "skill-system.h"
#include "combat-art.h"
#include "kernel-lib.h"
#include "constants/combat-arts.h"

extern struct CombatArtList sCombatArtList;

#define LOCAL_TRACE 0

STATIC_DECLAR void CalcCombatArtListExt(struct Unit *unit, int item)
{
	#define APPEND_TMPLIST(_combat_art_index) \
	do { \
		u8 __tmp_cid = _combat_art_index; \
		if (COMBART_VALID(__tmp_cid)) \
			tmp_list[__tmp_cid] = true; \
	} while (0)

	int i;
	int wtype = GetItemType(item);
	u8 pid = UNIT_CHAR_ID(unit);
	u8 jid = UNIT_CLASS_ID(unit);
	u8 iid = ITEM_INDEX(item);
	u8 *tmp_list = gGenericBuffer;
	struct SkillList *slist;

	LTRACEF("uid=0x%02X, pid=0x%02X, item=0x%04X", unit->index & 0xFF, UNIT_CHAR_ID(unit), item);

	/* Skill table */
	slist = GetUnitSkillList(unit);

	/**
	 * It may also touch tmp list in skill list generator
	 */
	WARN_GENERIC_BUF_USED;
	CpuFill16(0, tmp_list, 0x100);

	if (slist)
		for (i = 0; i < slist->amt; i++)
			APPEND_TMPLIST(gpCombatArtSkillTable[slist->sid[i]]);

	/* Skill weapon table */
	APPEND_TMPLIST(gpCombatArtSkillTable[gpConstSkillTable_Weapon[iid * 2 + 0]]);
	APPEND_TMPLIST(gpCombatArtSkillTable[gpConstSkillTable_Weapon[iid * 2 + 1]]);

	/* Weapon table */
	APPEND_TMPLIST(gpCombatArtWeaponTable[iid]);

	/* ROM table */
	for (i = WPN_LEVEL_E; i <= WPN_LEVEL_S; i++) {
		if (unit->ranks[ITYPE_SWORD] >= WRankToWExp(i)) {
			APPEND_TMPLIST(gpCombatArtDefaultTable->cid_sword[i]);
			APPEND_TMPLIST(gpCombatArtRomPTable[pid].cid_sword[i]);
			APPEND_TMPLIST(gpCombatArtRomJTable[jid].cid_sword[i]);
		}

		if (unit->ranks[ITYPE_LANCE] >= WRankToWExp(i)) {
			APPEND_TMPLIST(gpCombatArtDefaultTable->cid_lance[i]);
			APPEND_TMPLIST(gpCombatArtRomPTable[pid].cid_lance[i]);
			APPEND_TMPLIST(gpCombatArtRomJTable[jid].cid_lance[i]);
		}

		if (unit->ranks[ITYPE_AXE] >= WRankToWExp(i)) {
			APPEND_TMPLIST(gpCombatArtDefaultTable->cid_axe[i]);
			APPEND_TMPLIST(gpCombatArtRomPTable[pid].cid_axe[i]);
			APPEND_TMPLIST(gpCombatArtRomJTable[jid].cid_axe[i]);
		}

		if (unit->ranks[ITYPE_BOW] >= WRankToWExp(i)) {
			APPEND_TMPLIST(gpCombatArtDefaultTable->cid_bow[i]);
			APPEND_TMPLIST(gpCombatArtRomPTable[pid].cid_bow[i]);
			APPEND_TMPLIST(gpCombatArtRomJTable[jid].cid_bow[i]);
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

	WARN_GENERIC_BUF_RELEASED;

	#undef APPEND_TMPLIST
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

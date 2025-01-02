#include "common-chax.h"
#include "combat-art.h"

extern struct CombatArtList sCombatArtList;

static int WRankToWExp(int wrank)
{
	static const u8 wrank_to_exp_ref[] = {
		[WPN_LEVEL_0] = WPN_EXP_0,
		[WPN_LEVEL_E] = WPN_EXP_E,
		[WPN_LEVEL_D] = WPN_EXP_D,
		[WPN_LEVEL_C] = WPN_EXP_C,
		[WPN_LEVEL_B] = WPN_EXP_B,
		[WPN_LEVEL_A] = WPN_EXP_A,
		[WPN_LEVEL_S] = WPN_EXP_S,
	};

	switch (wrank) {
	case WPN_LEVEL_0 ... WPN_LEVEL_S:
		return wrank_to_exp_ref[wrank];

	default:
		return WPN_EXP_S;
	}
}

static void CalcCombatArtListExt_Fix(struct Unit * unit, u8 wtype)
{
    int i;
    u8 cid;
    u16 item_index = ITEM_INDEX(GetUnitEquippedWeapon(unit));
    u8 pid = UNIT_CHAR_ID(unit);
    u8 jid = UNIT_CLASS_ID(unit);
    u8 * tmp_list = gGenericBuffer;

    CpuFill16(0, tmp_list, 0x100);

    /* Skill table */
    for (i = 1; i < MAX_SKILL_NUM; i++)
    {
        cid = gpCombatArtSkillTable[i];
        if (COMBART_VALID(cid))
            if (SkillTester(unit, i))
                tmp_list[cid] = true;
    }

    /* Weapon table */
    cid = gpCombatArtWeaponTable[item_index];
    if (COMBART_VALID(cid))
        tmp_list[cid] = true;

    /* ROM table */
    for (i = WPN_LEVEL_E; i <= WPN_LEVEL_S; i++)
    {
        if (unit->ranks[ITYPE_SWORD] >= WRankToWExp(i))
        {
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

        if (unit->ranks[ITYPE_LANCE] >= WRankToWExp(i))
        {
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

        if (unit->ranks[ITYPE_AXE] >= WRankToWExp(i))
        {
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

        if (unit->ranks[ITYPE_BOW] >= WRankToWExp(i))
        {
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
    for (i = 1; i < 0xFF; i++)
    {
        if (tmp_list[i] == 0)
            continue;

        if (wtype != CA_WTYPE_ANY && GetCombatArtInfo(i)->wtype != CA_WTYPE_ANY && GetCombatArtInfo(i)->wtype != wtype)
            continue;

        sCombatArtList.cid[sCombatArtList.amt++] = i;
        if (sCombatArtList.amt >= COMBART_LIST_MAX_AMT)
            break;
    }
}

struct CombatArtList * GetCombatArtList_Fix(struct Unit * unit, u8 wtype)
{
    if (sCombatArtList.wtype != wtype || !JudgeUnitList(unit, &sCombatArtList.ref))
    {
        CalcCombatArtListExt_Fix(unit, wtype);
        WriteUnitList(unit, &sCombatArtList.ref);
        sCombatArtList.wtype = wtype;
    }
    return &sCombatArtList;
}

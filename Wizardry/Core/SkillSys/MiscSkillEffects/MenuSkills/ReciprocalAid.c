#include "common-chax.h"
#include "kernel-lib.h"
#include "map-anims.h"
#include "skill-system.h"
#include "event-rework.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "unit-expa.h"
#include "action-expa.h"


#if defined(SID_ReciprocalAid) && (COMMON_SKILL_VALID(SID_ReciprocalAid))
void TryAddToReciprocalAidTargetList(struct Unit *unit);
void MakeReciprocalAidTargetList(struct Unit *unit);
extern void ForEachAdjacentUnit(int x, int y, void(*)(struct Unit*));
extern u16 HarmAndHealEvent; 

u8 ReciprocalAid_Usability(const struct MenuItemDef *def, int number)
{
	if (gActiveUnit->state & US_CANTOING)
		return MENU_NOTSHOWN;

	if (!HasSelectTarget(gActiveUnit, MakeReciprocalAidTargetList))
		return MENU_DISABLED;

	return MENU_ENABLED;
}

static u8 ReciprocalAid_OnSelectTarget(ProcPtr proc, struct SelectTarget *target)
{
	gActionData.targetIndex = target->uid;

	gActionData.xOther = target->x;
	gActionData.yOther = target->y;

	HideMoveRangeGraphics();

	BG_Fill(gBG2TilemapBuffer, 0);
	BG_EnableSyncByMask(BG2_SYNC_BIT);

	gActionData.unk08 = SID_ReciprocalAid;
	gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

	return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}

u8 ReciprocalAid_OnSelected(struct MenuProc *menu, struct MenuItemProc *item)
{
	if (item->availability == MENU_DISABLED)
	{
		MenuFrozenHelpBox(menu, MSG_MenuSkill_ReciprocalAid_FRtext);
		return MENU_ACT_SND6B;
	}

	ClearBg0Bg1();

	MakeReciprocalAidTargetList(gActiveUnit);
	BmMapFill(gBmMapMovement, -1);

	StartSubtitleHelp(
		NewTargetSelection_Specialized(&gSelectInfo_PutTrap, ReciprocalAid_OnSelectTarget),
		GetStringFromIndex(MSG_MenuSkill_Common_Target));

	PlaySoundEffect(0x6A);
	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

void MakeReciprocalAidTargetList(struct Unit* unit) {
    int x = unit->xPos;
    int y = unit->yPos;

    gSubjectUnit = unit;

    BmMapFill(gBmMapRange, 0);

    ForEachAdjacentUnit(x, y, TryAddToReciprocalAidTargetList);

    return;
}

void TryAddToReciprocalAidTargetList(struct Unit* unit) {

    if (!AreUnitsAllied(gSubjectUnit->index, unit->index)) {
        return;
    }

    if (unit->state & US_RESCUED) {
        return;
    }
	// if both units are full hp, exit 
	if ((gSubjectUnit->maxHP == gSubjectUnit->curHP) && (unit->maxHP == unit->curHP)) {
		return; 
	}
	if ((unit->maxHP == unit->curHP) && (unit->maxHP < gSubjectUnit->curHP)) { 
		return; 
	} 

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
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
	gActiveUnit->state |= US_HAS_MOVED;
	gActiveUnit->state &= ~US_CANTOING;
	int actorHP = gActiveUnit->curHP;
	// int actorMax = gActiveUnit->maxHP;
	struct Unit *targetUnit = GetUnit(gActionData.targetIndex);
	int targetHP = targetUnit->curHP;

	int amountToHeal = 1; // default case
	// int targetMax = targetUnit->maxHP;

	// target is lower hp than you, so heal them
	if (targetHP < actorHP)
	{
		amountToHeal = actorHP - targetHP;
		// gActiveUnit->curHP -= amountToHeal; // if not showing anim
		gEventSlots[1] = gActiveUnit->pCharacterData->number; // unit to harm
		gEventSlots[2] = targetUnit->pCharacterData->number;
		gEventSlots[6] = amountToHeal;
	}
	else
	{ // if you're lower or equal hp vs target, heal yourself
		amountToHeal = targetHP - actorHP;
		// targetUnit->curHP -= amountToHeal;
		gEventSlots[1] = targetUnit->pCharacterData->number; // unit to harm
		gEventSlots[2] = gActiveUnit->pCharacterData->number;
		gEventSlots[6] = amountToHeal;
	}

	// gActiveUnit->state = gActiveUnit->state & ~US_HIDDEN;
	// SMS_UpdateFromGameData();
	// MU_EndAll();

	if (amountToHeal)
	{
		CallEvent(&HarmAndHealEvent, 1);
	}
	//return 0; // parent proc yields
}

bool Action_ReciprocalAid(ProcPtr parent)
{
	NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec);
	return true;
}
#endif

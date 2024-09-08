#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

extern u16 HarmAndHealEvent; 

u8 Sacrifice_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (!HasSelectTarget(gActiveUnit, MakeTargetListForAdjacentHeal))
		return MENU_DISABLED;

    return MENU_ENABLED;
}

static u8 Sacrifice_OnSelectTarget(ProcPtr proc, struct SelectTarget * target)
{
    gActionData.targetIndex = target->uid;

    gActionData.xOther = target->x;
    gActionData.yOther = target->y;

    HideMoveRangeGraphics();

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    gActionData.unk08 = SID_Sacrifice;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}

u8 Sacrifice_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
 if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_MenuSkill_Sacrifice_FRtext);
        return MENU_ACT_SND6B;
    }

    ClearBg0Bg1();

    MakeTargetListForAdjacentHeal(gActiveUnit);
    BmMapFill(gBmMapMovement, -1);

    StartSubtitleHelp(
        NewTargetSelection_Specialized(&gSelectInfo_PutTrap, Sacrifice_OnSelectTarget),
        GetStringFromIndex(MSG_MenuSkill_Common_Target));

    PlaySoundEffect(0x6A);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
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
	struct Unit* targetUnit = GetUnit(gActionData.targetIndex);
	int targetHP = targetUnit->curHP; 
	int amountToHeal = actorHP - 1; 
	int targetMax = targetUnit->maxHP;
	if ((amountToHeal + targetHP) > targetMax) { 
		amountToHeal = targetMax - targetHP; 
	} 
	
	// if target has bad status, recover it 
	int stID = targetUnit->statusIndex; 
	if (stID) { 
		if ((stID == UNIT_STATUS_POISON) || (stID == UNIT_STATUS_SLEEP) || (stID == UNIT_STATUS_SILENCED) || (stID == UNIT_STATUS_BERSERK) || (stID == UNIT_STATUS_SICK) || (stID == UNIT_STATUS_PETRIFY)) {
			targetUnit->statusIndex = 0; 
			targetUnit->statusDuration = 0; 
		}			
	}
	//gActiveUnit->curHP -= amountToHeal; // if not showing anim
	gEventSlots[1] = gActiveUnit->pCharacterData->number; 
	gEventSlots[2] = targetUnit->pCharacterData->number; 
	gEventSlots[6] = amountToHeal;
	//gActiveUnit->state = gActiveUnit->state & ~US_HIDDEN; 
	//SMS_UpdateFromGameData();
	//MU_EndAll();
	CallEvent(&HarmAndHealEvent, 1); 
	// return 0; // parent proc yields 
}

bool Action_Sacrifice(ProcPtr parent)
{
	NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec);
	return true;
}
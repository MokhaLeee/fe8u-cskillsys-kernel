#include "common-chax.h"
#include "playst-expa.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "map-anims.h"
#include "constants/skills.h"
#include "constants/texts.h"

#if defined(SID_DivineReprieve) && (COMMON_SKILL_VALID(SID_DivineReprieve))

u8 DivineReprieve_Usability(const struct MenuItemDef *def, int number)
{
	if (gActiveUnit->state & US_CANTOING)
		return MENU_NOTSHOWN;

	if (PlayStExpa_CheckBit(PLAYSTEXPA_BIT_DivineReprieve_Used))
		return MENU_DISABLED;

	return MENU_ENABLED;
}

u8 DivineReprieve_OnSelected(struct MenuProc *menu, struct MenuItemProc *item)
{
	if (item->availability == MENU_DISABLED) {
		MenuFrozenHelpBox(menu, MSG_MenuSkill_DivineReprieve_FRtext);
		return MENU_ACT_SND6B;
	}

	gActionData.unk08 = SID_DivineReprieve;
	gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

static void callback_anim(ProcPtr proc)
{
	PlaySoundEffect(0x269);
	StartNinianPrayfx(proc, gActiveUnit->xPos, gActiveUnit->yPos);
}

static void callback_exec(ProcPtr proc)
{
	PlayStExpa_SetBit(PLAYSTEXPA_BIT_DivineReprieve_Used);
	PlayStExpa_SetBit(PLAYSTEXPA_BIT_DivineReprieve_InForce);
}

bool Action_DivineReprieve(ProcPtr parent)
{
	NewMuSkillAnimOnActiveUnitWithDeamon(parent, gActionData.unk08, callback_anim, callback_exec);
	return true;
}

#endif /* SID_DivineReprieve */

/**
 * External hooks
 */
void PlayerPhase_HandleAutoEndRework(ProcPtr proc)
{
#if defined(SID_DivineReprieve) && (COMMON_SKILL_VALID(SID_DivineReprieve))
	if (PlayStExpa_CheckBit(PLAYSTEXPA_BIT_DivineReprieve_InForce))
		Proc_Goto(proc, 3);
#endif

	// vanilla
	PlayerPhase_HandleAutoEnd(proc);
}

void AiPhaseInitRework(ProcPtr proc)
{
#if defined(SID_DivineReprieve) && (COMMON_SKILL_VALID(SID_DivineReprieve))
	if (PlayStExpa_CheckBit(PLAYSTEXPA_BIT_DivineReprieve_InForce)) {
		PlayStExpa_ClearBit(PLAYSTEXPA_BIT_DivineReprieve_InForce);
		return;
	}
#endif

	// vanilla
	AiPhaseInit(proc);
}

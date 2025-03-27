#include "common-chax.h"
#include "stat-screen.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "combat-art.h"
#include "help-box.h"

LYN_REPLACE_CHECK(DisplayPage2);
void DisplayPage2(void)
{
	switch (gpKernelDesigerConfig->skil_page_style) {
	case CONFIG_PAGE4_MOKHA_PLAN_A:
	default:
		DrawSkillPage_MokhaPlanA();
		break;

	case CONFIG_PAGE4_MOKHA_PLAN_B:
		DrawSkillPage_MokhaPlanB();
		break;
	}
}

void StartSkillScreenHelp(int pageid, struct Proc *proc)
{
	switch (gpKernelDesigerConfig->skil_page_style) {
	case CONFIG_PAGE4_MOKHA_PLAN_A:
	default:
		gStatScreen.help = RTextSkillPage_MokhaPlanA;
		break;

	case CONFIG_PAGE4_MOKHA_PLAN_B:
		gStatScreen.help = RTextSkillPage_MokhaPlanB;
		break;
	}
}

/* HelpBox API */
void HbPopuplate_SkillPageCommon(struct HelpBoxProc *proc)
{
	struct SkillList *list = GetUnitSkillList(&gBattleActor.unit /* gStatScreen.unit */);

	proc->mid = GetSkillDescMsg(list->sid[proc->info->mid]);
}

void HbRedirect_SkillPageCommon(struct HelpBoxProc *proc)
{
	if (proc->info->mid < GetUnitSkillList(&gBattleActor.unit /* gStatScreen.unit */)->amt)
		return;

	switch (proc->moveKey) {
	case DPAD_DOWN:
		TryRelocateHbDown(proc);
		break;

	case DPAD_UP:
		TryRelocateHbUp(proc);
		break;

	case DPAD_LEFT:
		TryRelocateHbLeft(proc);
		break;

	case DPAD_RIGHT:
	default:
		TryRelocateHbRight(proc);
		break;
	}
}

void HbPopuplate_ArtPageCommon(struct HelpBoxProc *proc)
{
	struct CombatArtList *list = AutoGetCombatArtList(gStatScreen.unit);
	int cid = list->cid[proc->info->mid];

	proc->item = cid;
	proc->mid = GetCombatArtDesc(cid);
	sHelpBoxType = NEW_HB_COMBAT_ART_BKSEL;
}

void HbRedirect_ArtPageCommon(struct HelpBoxProc *proc)
{
	if (proc->info->mid < AutoGetCombatArtList(gStatScreen.unit)->amt)
		return;

	switch (proc->moveKey) {
	case DPAD_DOWN:
		TryRelocateHbDown(proc);
		break;

	case DPAD_UP:
		TryRelocateHbUp(proc);
		break;

	case DPAD_LEFT:
		TryRelocateHbLeft(proc);
		break;

	case DPAD_RIGHT:
	default:
		TryRelocateHbRight(proc);
		break;
	}
}

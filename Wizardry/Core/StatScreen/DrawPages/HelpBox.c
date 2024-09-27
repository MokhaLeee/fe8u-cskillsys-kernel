#include "common-chax.h"
#include "stat-screen.h"
#include "kernel-lib.h"
#include "skill-system.h"

LYN_REPLACE_CHECK(StartStatScreenHelp);
void StartStatScreenHelp(int pageid, struct Proc *proc)
{
	LoadHelpBoxGfx(NULL, -1); // default

	if (!gStatScreen.help) {
		switch (pageid) {
		case STATSCREEN_PAGE_0:
			StartUnitScreenHelp(pageid, proc);
			break;

		case STATSCREEN_PAGE_1:
			gStatScreen.help = &gHelpInfo_Ss1Item0;
			break;

		case STATSCREEN_PAGE_2:
			StartSkillScreenHelp(pageid, proc);
			break;

		case 3:
			gStatScreen.help = RTextPageSupport;
			break;
		} // switch (pageid)
	}
	StartMovingHelpBox(gStatScreen.help, proc);
}

LYN_REPLACE_CHECK(DisplayPage);
void DisplayPage(int pageid)
{
	typedef void (*func_type)(void);
	extern const func_type gStatScreenDrawPages[];

	CpuFastFill(0, gUiTmScratchA, sizeof(gUiTmScratchA));
	CpuFastFill(0, gUiTmScratchC, sizeof(gUiTmScratchC));

	gStatScreenDrawPages[pageid]();
}

void HbPopuplate_Page3Skill(struct HelpBoxProc *proc)
{
	struct SkillList *list = GetUnitSkillList(gStatScreen.unit);

	proc->mid = GetSkillDescMsg(list->sid[proc->info->mid]);
}

void HbRedirect_Page3Skill(struct HelpBoxProc *proc)
{
	if (proc->info->mid < GetUnitSkillList(gStatScreen.unit)->amt)
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
	} // switch
}

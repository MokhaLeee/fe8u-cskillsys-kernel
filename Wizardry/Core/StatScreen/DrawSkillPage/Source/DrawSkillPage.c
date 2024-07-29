#include "common-chax.h"
#include "stat-screen.h"
#include "kernel-lib.h"
#include "skill-system.h"

/* LynJump */
void DisplayPage2(void)
{
    switch (gpKernelDesigerConfig->page4_style) {
    case CONFIG_PAGE4_MOKHA_PLAN_A:
    default:
        DrawSkillPage_MokhaPlanA();
        break;
    }
}

void StartSkillScreenHelp(int pageid, struct Proc * proc)
{
    switch (gpKernelDesigerConfig->page4_style) {
    case CONFIG_PAGE4_MOKHA_PLAN_A:
    default:
        gStatScreen.help = RTextSkillPage_MokhaPlanA;
        break;
    }
}

/* HelpBox API */
void HbPopuplate_SkillPageCommon(struct HelpBoxProc * proc)
{
    struct SkillList * list = GetUnitSkillList(gStatScreen.unit);
    proc->mid = GetSkillDescMsg(list->sid[proc->info->mid]);
}

void HbRedirect_SkillPageCommon(struct HelpBoxProc * proc)
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

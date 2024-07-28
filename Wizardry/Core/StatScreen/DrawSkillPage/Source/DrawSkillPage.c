#include "common-chax.h"
#include "stat-screen.h"
#include "kernel-lib.h"

/* LynJump */
void DisplayPage2(void)
{
    switch (gpKernelDesigerConfig->page4_style) {
    case CONFIG_PAGE4_MOKHA_PLAN:
    default:
        DrawSkillPage_MokhaPlan();
        break;
    }
}

void StartSkillScreenHelp(int pageid, struct Proc * proc)
{
    switch (gpKernelDesigerConfig->page4_style) {
    case CONFIG_PAGE4_MOKHA_PLAN:
    default:
        gStatScreen.help = RTextSkillPage_MokhaPlan;
        break;
    }
}

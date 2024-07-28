#include "common-chax.h"
#include "kernel-lib.h"
#include "stat-screen.h"
#include "constants/texts.h"

void DrawPage4Rework(void)
{
    switch (gpKernelDesigerConfig->page4_style) {
    case CONFIG_PAGE4_MOKHA_PLAN:
    default:
        DrawPage4_MokhaPlan();
        break;
    }
}

void StartSkillScreenHelp(int pageid, struct Proc * proc)
{
    switch (gpKernelDesigerConfig->page4_style) {
    case CONFIG_PAGE4_MOKHA_PLAN:
    default:
        gStatScreen.help = RTextPage4_MokhaPlan;
        break;
    }
}

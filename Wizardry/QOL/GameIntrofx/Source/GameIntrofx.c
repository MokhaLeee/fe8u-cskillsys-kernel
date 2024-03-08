#include "global.h"
#include "proc.h"
#include "gamecontrol.h"

#include "common-chax.h"

void AutoSkilOpAnim(struct Proc * proc)
{
    SetNextGameActionId(GAME_ACTION_USR_SKIPPED);
    Proc_Goto(proc->proc_parent, 99);
}

#include "global.h"
#include "proc.h"
#include "prepscreen.h"
#include "uichapterstatus.h"

#include "common-chax.h"
#include "prep-skill.h"

void sub_8095C2C(struct ProcAtMenu * proc);

void PrepScreenMenu_OnEquip(struct ProcAtMenu * proc)
{
    proc->state = 6;
    Proc_Goto(proc, 0xA);
}

/* LynJump! */
void AtMenu_StartSubmenu(struct ProcAtMenu * proc)
{
    sub_8095C2C(proc);

    switch (proc->state) {
    case 5:
        sub_808E79C(proc);
        break;

    case 2: /* Items */
        StartPrepItemScreen(proc);
        break;

    case 1: /* Pick Units */
        Proc_StartBlocking(ProcScr_PrepUnitScreen, proc);
        break;

    case 4: /* Support */
        StartFortuneSubMenu(2, proc);
        break;

    case 3: /* Save */
        StartPrepSaveScreen(proc);
        break;

    case 6: /* Equip */
        StartPrepEquipScreen(proc);
        break;

    default:
        break;
    }

    Proc_Break(proc);
}

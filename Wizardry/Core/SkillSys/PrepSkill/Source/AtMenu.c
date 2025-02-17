#include "common-chax.h"
#include "prep-skill.h"
#include "kernel-lib.h"

void PrepScreenMenu_OnEquip(struct ProcAtMenu *proc)
{
	if (gpKernelDesigerConfig->equip_skill_en == false) {
		/* Check map */
		Proc_Goto(proc, 0x5);
		return;
	}

	proc->state = 6;
	Proc_Goto(proc, 0xA);
}

LYN_REPLACE_CHECK(AtMenu_StartSubmenu);
void AtMenu_StartSubmenu(struct ProcAtMenu *proc)
{
	sub_8095C2C(proc);

	switch (proc->state) {
	case 5:
		StartChapterStatusScreen_FromPrep(proc);
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

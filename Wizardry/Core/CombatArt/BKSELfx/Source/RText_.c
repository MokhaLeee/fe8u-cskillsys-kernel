#include "common-chax.h"
#include "combat-art.h"
#include "help-box.h"

void HbPopuplate_CombatArtBKSEL(struct HelpBoxProc *proc)
{
	int cid = GetCombatArtInForce(gActiveUnit);

	proc->item = cid;
	proc->mid = GetCombatArtDesc(cid);
	sHelpBoxType = NEW_HB_COMBAT_ART_BKSEL;
}

void HbPopuplate_NotCombatArtBKSEL(struct HelpBoxProc *proc)
{
	sHelpBoxType = 0;
}

void HbRedirect_CombatArtBKSEL(struct HelpBoxProc *proc)
{
	int cid = GetCombatArtInForce(gActiveUnit);

	if (COMBART_VALID(cid))
		return;

	switch (proc->moveKey) {
	case DPAD_DOWN:
		TryRelocateHbDown(proc);
		break;

	case DPAD_UP:
		TryRelocateHbUp(proc);
		break;
	}
}

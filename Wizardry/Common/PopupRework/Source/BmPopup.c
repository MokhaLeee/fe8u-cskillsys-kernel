#include <common-chax.h>
#include <icon-rework.h>
#include <popup-reowrk.h>

LYN_REPLACE_CHECK(ParsePopupInstAndGetLen);
int ParsePopupInstAndGetLen(struct PopupProc *_proc)
{
	struct ProcPopupR *proc = (struct ProcPopupR *)_proc;

	proc->pop.xGfxSize = 0;

	for (proc->cur_inst = proc->pop.pDefinition; proc->cur_inst->opcode != POPUP_OP_END; proc->cur_inst++) {
		if (gPopupComponents[proc->cur_inst->opcode].get_len == NULL)
			continue;

		proc->pop.xGfxSize += gPopupComponents[proc->cur_inst->opcode].get_len(proc);
	}

	return proc->pop.xGfxSize;
}

LYN_REPLACE_CHECK(GeneratePopupText);
void GeneratePopupText(const struct PopupInstruction *inst, struct Text th)
{
	for (; inst->opcode != POPUP_OP_END; inst++) {
		if (gPopupComponents[inst->opcode].display == NULL)
			continue;

		gPopupComponents[inst->opcode].display(&th, inst);
	}
	BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT);
}

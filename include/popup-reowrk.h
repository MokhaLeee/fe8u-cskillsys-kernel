#pragma once

#include "common-chax.h"

enum chax_popup_opcode_index {
	CHAX_POPUP_OP_START = POPUP_OP_SOUND,

	// new

	CHAX_POPUP_OP_ALLOC_MAX = 0x30
};

struct ProcPopupR {
	struct PopupProc pop;

	const struct PopupInstruction *cur_inst;
};

struct PopupComponent {
	int  (*get_len)(struct ProcPopupR *proc);
	void (*display)(struct Text *text, const struct PopupInstruction *inst);
};

extern struct PopupComponent const gPopupComponents[CHAX_POPUP_OP_ALLOC_MAX];

/**
 * Vanilla .bss
 *
 * Add definition to usr-defined.s
 */
extern IWRAM_DATA struct PopupInstruction gPopupInst[4];
extern IWRAM_DATA struct Unit *gpPopupUnit;
extern IWRAM_DATA u16 gPopupItem;
extern IWRAM_DATA u32 gPopupNumber;

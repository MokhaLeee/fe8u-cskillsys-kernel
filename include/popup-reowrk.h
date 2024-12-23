#pragma once

#include "common-chax.h"

enum chax_popup_opcode_index {
	CHAX_POPUP_OP_START = POPUP_OP_SOUND,

	// new

	CHAX_POPUP_OP_ALLOC_MAX = 0x30
};

struct PopupComponent {
	int  (*get_len)(struct PopupProc *proc, const struct PopupInstruction *inst);
	void (*display)(struct Text *text, const struct PopupInstruction *inst);
};

extern struct PopupComponent const gPopupComponents[CHAX_POPUP_OP_ALLOC_MAX];

struct PopupConfig {
	bool (*setup)(void);
	const struct PopupInstruction *inst;
	int duration;
};

extern struct PopupConfig const gBattlePopupConfigs[];
extern struct PopupConfig const gPromotPopupConfigs[];

struct ProcPopupHanlder {
	PROC_HEADER;

	const struct PopupConfig *config;
	struct Anim *anim;
};

/**
 * Vanilla .bss
 *
 * Add definition to usr-defined.s
 */
extern IWRAM_DATA struct PopupInstruction gPopupInst[4];
extern IWRAM_DATA struct Unit *gpPopupUnit;
extern IWRAM_DATA u16 gPopupItem;
extern IWRAM_DATA u32 gPopupNumber;

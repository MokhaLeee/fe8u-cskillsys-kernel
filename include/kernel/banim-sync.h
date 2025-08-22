#pragma once

#include "common-chax.h"

/**
 * Usage:
 *
 * Register BanimSyncHandler, put handler function to
 * hook list: BanimSyncHandlerGetter
 */

struct BanimSyncHandler {
	/* 00 */ u8 pid, jid;
	/* 02 */ u16 weapon;
	/* 04 */ const struct BattleAnimDef *animdef;
	/* 08 at end */
};
extern struct BanimSyncHandler gBanimSyncHandler;

typedef struct BanimSyncHandler * (*BanimSyncHandlerGetter_t)(struct Anim *anim);
extern const BanimSyncHandlerGetter_t BanimSyncHandlerGetter[];

int BanimSync(int pos, u8 pid, u8 jid, const struct BattleAnimDef *animdef, u16 weapon, int type);
void BanimSyncFrame(int pos, int type);
struct BanimSyncHandler *FindBanimSyncHandler(struct Anim *anim);
int TrySwitchBanim(struct Anim *anim);

struct BanimSyncInfo {
	PROC_HEADER;

	u8 timer;
	bool in_default[2];

	s16 banim_idx[2];
	s16 spellassoc_idx[2];
	s16 unique_pal[2];
};

#define gpBanimSyncInfo ((struct BanimSyncInfo *)gpProcEkrBattleDeamon)

/**
 * Custom banim switcher
 */
struct CustomAnimnSwitcher {
	u8 pid, jid;
	u8 wtype;
	u8 item;

	u8 tar_pid;
	u8 tar_jid;
	u8 act_when_killing;
	u8 act_when_crit;

	u16 banim_index;

	u16 sid;
	u16 act_flag;
	u16 _pad_;
};

extern struct CustomAnimnSwitcher const gCustomAnimnSwitcher[];
extern struct CustomAnimnSwitcher const *const gpCustomAnimnSwitcher;

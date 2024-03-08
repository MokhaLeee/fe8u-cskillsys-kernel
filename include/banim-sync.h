#pragma once

#include "global.h"

/**
 * Usage:
 *
 * Register BanimSyncHandler, put handler function to
 * hook list: BanimSyncHandlerGetter
 */

struct BanimSyncHandler {
    /* 00 */ u8 pid, jid;
    /* 02 */ u16 weapon;
    /* 04 */ const struct BattleAnimDef * animdef;
    /* 08 at end */
};
extern struct BanimSyncHandler gBanimSyncHandler;

typedef struct BanimSyncHandler * (* BanimSyncHandlerGetter_t)(struct Anim * anim);
extern const BanimSyncHandlerGetter_t BanimSyncHandlerGetter[];

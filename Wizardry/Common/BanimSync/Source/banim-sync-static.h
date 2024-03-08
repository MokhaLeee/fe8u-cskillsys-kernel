#pragma once

#include "global.h"
#include "banim-sync.h"

int BanimSync(int pos, u8 pid, u8 jid, const struct BattleAnimDef * animdef, u16 weapon, int type);
void BanimSyncFrame(int pos, int type);

struct BanimSyncHandler * FindBanimSyncHandler(struct Anim * anim);
int TrySwitchBanim(struct Anim * anim);

struct BanimSyncInfo {
    PROC_HEADER;

    u8 timer;
    bool in_default[2];

    s16 banim_idx[2];
    s16 spellassoc_idx[2];
    s16 unique_pal[2];
};

#define gpBanimSyncInfo ((struct BanimSyncInfo *)gpProcEkrBattleDeamon)

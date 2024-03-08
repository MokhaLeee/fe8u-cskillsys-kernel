#include "common-chax.h"
#include "banim-sync.h"
#include "banim-sync-static.h"

STATIC_DECLAR int TrySwitchBanimInfo(int pos, u8 pid, u8 jid, const struct BattleAnimDef * animdef, u16 weapon);

struct BanimSyncHandler * FindBanimSyncHandler_test(struct Anim * anim)
{
    gBanimSyncHandler.pid = CHARACTER_EPHRAIM;
    gBanimSyncHandler.jid = CLASS_EPHRAIM_LORD;
    gBanimSyncHandler.weapon = ITEM_LANCE_BRAVE;
    gBanimSyncHandler.animdef = GetClassData(CLASS_EPHRAIM_LORD)->pBattleAnimDef;
    return &gBanimSyncHandler;
}

struct BanimSyncHandler * FindBanimSyncHandler(struct Anim * anim)
{
    struct BanimSyncHandler * handler;
    int i = 0;
    for (i = 0; BanimSyncHandlerGetter[i] != NULL; i++)
    {
        handler = BanimSyncHandlerGetter[i](anim);
        if (handler)
            return handler;
    }
    return NULL;
}

int TrySwitchBanim(struct Anim * anim)
{
    int pos = GetAnimPosition(anim);
    int ret;
    struct BanimSyncHandler * handler;

    handler = FindBanimSyncHandler(anim);
    if (handler)
    {
        ret = TrySwitchBanimInfo(pos,
                                    handler->pid,
                                    handler->jid,
                                    handler->animdef,
                                    handler->weapon);
        /* Already find new force-updated anim */
        if (ret == 1)
        {
            gpBanimSyncInfo->in_default[pos] = false;
            return 1;
        }
    }

    /* No new anim, judge whether we need to switch back to dafault anim */
    if (gpBanimSyncInfo->in_default[pos] == false)
    {
        gpBanimSyncInfo->in_default[pos] = true;

        gBanimIdx[pos]          = gpBanimSyncInfo->banim_idx[pos];
        gBanimUniquePal[pos]    = gpBanimSyncInfo->unique_pal[pos];
        gEkrSpellAnimIndex[pos] = gpBanimSyncInfo->spellassoc_idx[pos];
        return 1;
    }
    return 0;
}

extern const u8 * p_gAnimCharaPalConfig;
extern const u8 * p_gAnimCharaPalIt;

STATIC_DECLAR int TrySwitchBanimInfo(int pos,
                                     u8 pid,
                                     u8 jid,
                                     const struct BattleAnimDef * animdef,
                                     u16 weapon)
{
    int i;
    u32 unused;

    /* Find banim index */
    gBanimIdx[pos] = GetBattleAnimationId(NULL, animdef, weapon, &unused);
    if (gBanimIdx[pos] == -1) /* We did not find a proper anim */
        return 0;

    /* Find unique palette */
    gBanimUniquePal[pos] = -1;
    for (i = 0; i < 7; i++)
    {
        if (p_gAnimCharaPalConfig[(pid - 1) * 7 + i] == jid)
        {
            gBanimUniquePal[pos] = p_gAnimCharaPalIt[(pid - 1) * 7 + i] - 1;
            break;
        }
    }

    /* Find spell assoc */
    gEkrSpellAnimIndex[pos] = GetSpellAnimId(jid, ITEM_INDEX(weapon));
    if (gEkrSpellAnimIndex[pos] < 0)
        gEkrSpellAnimIndex[pos] = 0;

    Printf("pid %#x, jid %#x", pid, jid);
    Printf("bid %#x, unqpal %#x, spell %#x", gBanimIdx[pos], gBanimUniquePal[pos], gEkrSpellAnimIndex[pos]);

    return 1;
}

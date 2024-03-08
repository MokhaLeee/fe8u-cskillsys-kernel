#include "common-chax.h"
#include "banim-sync.h"
#include "banim-sync-static.h"

STATIC_DECLAR void EkrBattleDeamonRework_PreInit(struct BanimSyncInfo * proc);
STATIC_DECLAR void EkrBattleDeamonRework_Init(struct BanimSyncInfo * proc);
STATIC_DECLAR void EkrBattleDeamonRework_Main(struct BanimSyncInfo * proc);

void EkrBattleDeamonRework(struct BanimSyncInfo * proc)
{
    proc->timer = 0;
    proc->proc_idleCb = (ProcFunc)EkrBattleDeamonRework_PreInit;
}

STATIC_DECLAR void EkrBattleDeamonRework_PreInit(struct BanimSyncInfo * proc)
{
    if (++proc->timer == 5)
        proc->proc_idleCb = (ProcFunc)EkrBattleDeamonRework_Init;
}

STATIC_DECLAR void EkrBattleDeamonRework_Init(struct BanimSyncInfo * proc)
{
    /* Init routine */
    proc->in_default[POS_L] = true;
    proc->in_default[POS_R] = true;

    proc->banim_idx[POS_L] = gBanimIdx[POS_L];
    proc->banim_idx[POS_R] = gBanimIdx[POS_R];

    proc->spellassoc_idx[POS_L] = gEkrSpellAnimIndex[POS_L];
    proc->spellassoc_idx[POS_R] = gEkrSpellAnimIndex[POS_R];

    proc->unique_pal[POS_L] = gBanimUniquePal[POS_L];
    proc->unique_pal[POS_R] = gBanimUniquePal[POS_R];

    proc->proc_idleCb = (ProcFunc)EkrBattleDeamonRework_Main;
}

STATIC_DECLAR void EkrBattleDeamonRework_Main(struct BanimSyncInfo * proc)
{
    return;
}

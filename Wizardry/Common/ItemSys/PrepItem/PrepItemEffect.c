#include "global.h"
#include "bmunit.h"
#include "bmitem.h"
#include "prepscreen.h"
#include "soundwrapper.h"
#include "constants/items.h"

#include "common-chax.h"

void PrepItemEffect_StatBooster(struct ProcPrepItemUse * proc, u16 item);
void PrepItemEffect_Promotion(struct ProcPrepItemUse * proc, u16 item);
void PrepItemEffect_JunaFruit(struct ProcPrepItemUse * proc, u16 item);

typedef void (* PrepItemEffectFunc_t)(struct ProcPrepItemUse * proc, u16 item);
PrepItemEffectFunc_t const gPrepItemEffectFunc[0x100] = {
    [ITEM_HEAVENSEAL]  = PrepItemEffect_Promotion,
    [ITEM_HEROCREST]   = PrepItemEffect_Promotion,
    [ITEM_KNIGHTCREST] = PrepItemEffect_Promotion,
    [ITEM_ORIONSBOLT]  = PrepItemEffect_Promotion,
    [ITEM_ELYSIANWHIP] = PrepItemEffect_Promotion,
    [ITEM_GUIDINGRING] = PrepItemEffect_Promotion,
    [ITEM_MASTERSEAL]  = PrepItemEffect_Promotion,
    [ITEM_OCEANSEAL]   = PrepItemEffect_Promotion,
    [ITEM_LUNARBRACE]  = PrepItemEffect_Promotion,
    [ITEM_SOLARBRACE]  = PrepItemEffect_Promotion,
    [ITEM_UNK_C1]      = PrepItemEffect_Promotion,

    [ITEM_JUNAFRUIT] = PrepItemEffect_JunaFruit,

    [ITEM_BOOSTER_HP]  = PrepItemEffect_StatBooster,
    [ITEM_BOOSTER_POW] = PrepItemEffect_StatBooster,
    [ITEM_BOOSTER_SKL] = PrepItemEffect_StatBooster,
    [ITEM_BOOSTER_SPD] = PrepItemEffect_StatBooster,
    [ITEM_BOOSTER_LCK] = PrepItemEffect_StatBooster,
    [ITEM_BOOSTER_DEF] = PrepItemEffect_StatBooster,
    [ITEM_BOOSTER_RES] = PrepItemEffect_StatBooster,
    [ITEM_BOOSTER_MOV] = PrepItemEffect_StatBooster,
    [ITEM_BOOSTER_CON] = PrepItemEffect_StatBooster,
};

/* LynJump */
void PrepItemUse_HandleItemEffect(struct ProcPrepItemUse * proc)
{
    u16 item = proc->unit->items[proc->slot];
    PrepItemEffectFunc_t it = gPrepItemEffectFunc[ITEM_INDEX(item)];

    if (it)
        it(proc, item);
}

void PrepItemEffect_StatBooster(struct ProcPrepItemUse * proc, u16 item)
{
    Proc_StartBlocking(ProcScr_PrepItemUseBooster, proc);
}

void PrepItemEffect_Promotion(struct ProcPrepItemUse * proc, u16 item)
{
    PlaySoundEffect(0x6A);
    Proc_Goto(proc, PROC_LABEL_PREPITEMUSE_EXEC_PROMO);
}

void PrepItemEffect_JunaFruit(struct ProcPrepItemUse * proc, u16 item)
{
    Proc_Goto(proc, PROC_LABEL_PREPITEMUSE_EXEC_JUNA);
}

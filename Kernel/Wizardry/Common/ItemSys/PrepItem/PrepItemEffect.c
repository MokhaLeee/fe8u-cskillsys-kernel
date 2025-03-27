#include "common-chax.h"
#include "item-sys.h"

LYN_REPLACE_CHECK(PrepItemUse_HandleItemEffect);
void PrepItemUse_HandleItemEffect(struct ProcPrepItemUse *proc)
{
	u16 item = proc->unit->items[proc->slot];
	PrepItemEffectFunc_t it = gpPrepItemEffectFunc[ITEM_INDEX(item)];

	if (it)
		it(proc, item);
}

void PrepItemEffect_StatBooster(struct ProcPrepItemUse *proc, u16 item)
{
	Proc_StartBlocking(ProcScr_PrepItemUseBooster, proc);
}

void PrepItemEffect_Promotion(struct ProcPrepItemUse *proc, u16 item)
{
	PlaySoundEffect(0x6A);
	Proc_Goto(proc, PROC_LABEL_PREPITEMUSE_EXEC_PROMO);
}

void PrepItemEffect_JunaFruit(struct ProcPrepItemUse *proc, u16 item)
{
	Proc_Goto(proc, PROC_LABEL_PREPITEMUSE_EXEC_JUNA);
}

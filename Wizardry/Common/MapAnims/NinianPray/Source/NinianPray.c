#include "common-chax.h"
#include "map-anims.h"

void StartNinianPrayfx(ProcPtr proc, int x, int y)
{
	struct ProcEventSpriteAnim *procfx;

	if (proc)
		procfx = Proc_StartBlocking(ProcScr_EventSpriteAnim, proc);
	else
		procfx = Proc_Start(ProcScr_EventSpriteAnim, PROC_TREE_3);

	procfx->x = x;
	procfx->y = y;
	procfx->priv = &EventSpriteAnimConf_NinianPray;
}

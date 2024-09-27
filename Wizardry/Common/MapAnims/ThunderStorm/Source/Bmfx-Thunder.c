#include "common-chax.h"
#include "map-anims.h"

#define LOCAL_TRACE 0

struct ProcEventThunderfx {
	PROC_HEADER;

	/* 3C */ int x, y;
};

STATIC_DECLAR void EventThunderfx_Init(struct ProcEventThunderfx *proc)
{
	gLCDControlBuffer.bg0cnt.priority = 0;
	gLCDControlBuffer.bg1cnt.priority = 1;
	gLCDControlBuffer.bg2cnt.priority = 0;
	gLCDControlBuffer.bg3cnt.priority = 3;

	SetBlendAlpha(0x10, 0x10);
	SetBlendTargetA(0, 0, 1, 0, 0);
	SetBlendTargetB(0, 0, 0, 1, 1);

	PlaySoundEffect(0x11A);

	StartBmBgfx(
		BmBgfxConf_EventThunder,
		BG_2,
		proc->x,
		proc->y,
		0,
		0x2000,
		0xF,
		NULL, proc
	);
}

STATIC_DECLAR void EventThunderfx_End(struct ProcEventThunderfx *proc)
{
	SetBlendConfig(BLEND_EFFECT_NONE, 0, 0x10, 0);
	InitBmBgLayers();
}

STATIC_DECLAR const struct ProcCmd ProcScr_EventThunderfx[] = {
	PROC_NAME("MapAnimThunderfx"),
	PROC_YIELD,
	PROC_CALL(EventThunderfx_Init),
	PROC_YIELD,
	PROC_WHILE(CheckBmBgfxDone),
	PROC_CALL(EventThunderfx_End),
	PROC_YIELD,
	PROC_END,
};

void CallMapAnim_ThunderStorm(ProcPtr parent, int x, int y)
{
	struct ProcEventThunderfx *proc;

	if (parent)
		proc = Proc_StartBlocking(ProcScr_EventThunderfx, parent);
	else
		proc = Proc_Start(ProcScr_EventThunderfx, PROC_TREE_3);

	LTRACEF("x %d, y %d", x, y);

	proc->x = SCREEN_TILE_IX(x) - 0x18;
	proc->y = SCREEN_TILE_IY(y) - 0x78;
}

bool MapAnimThunderStormExists(void)
{
	return !!Proc_Find(ProcScr_EventThunderfx);
}

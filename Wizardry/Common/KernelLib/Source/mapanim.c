#include "common-chax.h"
#include "kernel-lib.h"

void MapAnim_CommonInit(void)
{
	EndAllMus();
	RenderBmMap();
	RefreshEntityBmMaps();
	RefreshUnitSprites();
}

void MapAnim_CommonEnd(void)
{
	// ResetMuAnims();
	InitBmBgLayers();
	LoadUiFrameGraphics();
	LoadObjUIGfx();
}

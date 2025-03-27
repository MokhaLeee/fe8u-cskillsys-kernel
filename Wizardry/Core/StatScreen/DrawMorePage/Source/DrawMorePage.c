#include "common-chax.h"

extern u16 const *const *const gpSprites_PageNameRework;
extern u16 const *const gpPageNameChrOffsetLutRe;
extern u16 const *const gpPageNamePaletteRe;

LYN_REPLACE_CHECK(DisplayPageNameSprite);
void DisplayPageNameSprite(int pageid)
{
	int colorid;

	PutSprite(4,
		111 + gStatScreen.xDispOff, 1 + gStatScreen.yDispOff,
		sSprite_PageNameBack, TILEREF(0x293, 4) + 0xC00);

	PutSprite(4,
		114 + gStatScreen.xDispOff, 0 + gStatScreen.yDispOff,
		gpSprites_PageNameRework[pageid],
		TILEREF(0x240 + gpPageNameChrOffsetLutRe[pageid], 3) + 0xC00);

	colorid = (GetGameClock()/4) % 16;

	CpuCopy16(
		gpPageNamePaletteRe + colorid,
		PAL_OBJ(3) + 0xE,
		sizeof(u16));

	EnablePaletteSync();
}

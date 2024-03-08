#include "global.h"
#include "hardware.h"
#include "bmlib.h"
#include "ctc.h"
#include "statscreen.h"

static const u16 Sprite_Page0NameRework[] =
{
    3,
    0x4104, 0x9008, TILEREF(0, 0),
    0x4104, 0x9028, TILEREF(4, 0),
    0x4104, 0x9048, TILEREF(8, 0),
};

static const u16 Sprite_Page1NameRework[] =
{
    2,
    0x4104, 0x901E, TILEREF(0, 0),
    0x4104, 0x903E, TILEREF(4, 0),
};

static const u16 Sprite_Page2NameRework[] =
{
    5,
    0x4108, 0x9004, TILEREF(6,  0),
    0x4108, 0x9024, TILEREF(10, 0),
    0x4108, 0x9044, TILEREF(14, 0),
    0x4100, 0x9000, TILEREF(0,  0),
    0x0100, 0x5020, TILEREF(4,  0),
};

static const u16 Sprite_Page3NameRework[] =
{
    3,
    0x4104, 0x9008, TILEREF(0, 0),
    0x4104, 0x9028, TILEREF(4, 0),
    0x4104, 0x9048, TILEREF(8, 0),
};

static const u16 Sprite_Page4NameRework[] =
{
    3,
    0x4104, 0x9008, TILEREF(14, 0),
    0x4104, 0x9028, TILEREF(18, 0),
    0x4104, 0x9048, TILEREF(22, 0),
};

u16 const * const Sprites_PageNameRework[] =
{
    Sprite_Page0NameRework,
    Sprite_Page1NameRework,
    Sprite_Page2NameRework,
    Sprite_Page3NameRework,
    Sprite_Page4NameRework,
};

const u16 gPageNameChrOffsetLutRe[] = {
    0x00, 0x40, 0x0E, 0x80, 0x80
};

extern u16 const * const * const gpSprites_PageNameRework;
extern u16 const * const gpPageNameChrOffsetLutRe;
extern u16 const * const gpPageNamePaletteRe;

/* LynJump */
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

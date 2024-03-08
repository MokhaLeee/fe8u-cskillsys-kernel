#include "global.h"
#include "bmlib.h"
#include "hardware.h"
#include "statscreen.h"
#include "bmlib.h"
#include "ctc.h"
#include "bmunit.h"
#include "mu.h"
#include "sysutil.h"
#include "uiutils.h"
#include "efxbattle.h"
#include "constants/video-global.h"

#include "common-chax.h"
#include "bwl.h"
#include "statscreenfx.h"

extern const u8 Tsa_StatScreenBg1Left[];

static void LoadStatScreenUiFrame(void)
{
    if (UNIT_FACTION(gStatScreen.unit) != FACTION_RED)
    {
        Decompress(Img_StatScreenUI, BG_CHAR_ADDR(0));
        ApplyPalette(Pal_StatScreenUI, BGPAL_WINDOW_FRAME);
        ApplyPalette(sUiFramePaletteLookup[0], STATSCREEN_BGPAL_3);
        ApplyPalette(sStatBarPaletteLookup[0], STATSCREEN_BGPAL_6);
    }
    else
    {
        Decompress(Img_StatScreenUIRed, BG_CHAR_ADDR(0));
        ApplyPalette(Pal_StatScreenUIRed, BGPAL_WINDOW_FRAME);
        ApplyPalette(sUiFramePaletteLookup[1], STATSCREEN_BGPAL_3);
        ApplyPalette(sStatBarPaletteLookup[1], STATSCREEN_BGPAL_6);
    }
}

static void LoadStatScreenMuAreaBackGround(void)
{
    Decompress(Img_StatScreenMuAreaBg, gGenericBuffer);
    Copy2dChr(gGenericBuffer, OBJ_VRAM0 + STATSCREEN_MUAREA_CHR_BASE, 5, 4);
    ApplyPalette(Pal_StatScreenMuAreaBg, 0x10 + STATSCREEN_MUAREA_PAL_BASE);
}

/* LynJump */
void PageNumCtrl_DisplayMuPlatform(struct StatScreenPageNameProc * proc)
{
    SetBlendTargetA(0, 0, 1, 0, 0);
    SetBlendTargetB(0, 0, 0, 1, 0);
    SetBlendAlpha(6, 8);

    PutSprite(12,
        gStatScreen.xDispOff,
        gStatScreen.yDispOff,
        Sprite_StatScreenMuAreaBackground,
        OAM2_CHR(STATSCREEN_MUAREA_CHR_BASE / 0x20) +
        OAM2_PAL(STATSCREEN_MUAREA_PAL_BASE) +
        OAM2_LAYER(3));

    PutSprite(11,
        gStatScreen.xDispOff + 64,
        gStatScreen.yDispOff + 131,
        gObject_32x16, TILEREF(0x28F, STATSCREEN_OBJPAL_4) + OAM2_LAYER(3));
}

static void StatScreenDisplayBG1(int page)
{
    if (UNIT_FACTION(gStatScreen.unit) == FACTION_BLUE)
        CallARM_FillTileRect(gUiTmScratchB, TsaLut_StatScreenBgPlayer[page], TILEREF(0, 1));
    else
        CallARM_FillTileRect(gUiTmScratchB, TsaLut_StatScreenBgNonePlayer[page], TILEREF(0, 1));
}

static void StatScreenNewCallBack(ProcPtr proc)
{
    EndMuralBackground();
}

static void StatScreen_InitDisplayScrollingBG(void)
{
    StartMuralBackground(NULL, (void *)VRAM + 0xB000, -1);
    Decompress(Img_StatScreenScrollBG, (void *)0x0600B000);
    ApplyPalettes(Pal_StatScreenScrollBG, 0xE, 2);
}


void StatScreen_InitDisplayRework(ProcPtr proc)
{
    StatScreen_InitDisplay(proc);

    /* Remove hallo */
    CpuFastFill16(0, (void *)0x06004400, 0x1400);

    /* Remove BG3 */
    TileMap_FillRect(gBG3TilemapBuffer + TILEMAP_INDEX(0, 0), 32, 30, 0);

    /* Set left BG */
    LoadStatScreenMuAreaBackGround();
    LoadStatScreenUiFrame();
    CallARM_FillTileRect(gBG1TilemapBuffer, Tsa_StatScreenBg1Left, TILEREF(0, 1));

    /* Set right BG */
    StatScreenDisplayBG1(gStatScreen.page);
    TileMap_CopyRect(
        gUiTmScratchB,
        gBG1TilemapBuffer + 0 + TILEMAP_INDEX(12, 2),
        18, 18);
    BG_EnableSyncByMask(BG1_SYNC_BIT);

    /* scroll BG */
    StatScreen_InitDisplayScrollingBG();

    Proc_SetEndCb(proc, StatScreenNewCallBack);
}

void PageSlide_OnLoopRework(struct StatScreenEffectProc* proc)
{
    int off;

    int len, dstOff, srcOff;

    // clear bg0, bg2 page area
    TileMap_FillRect(gBG0TilemapBuffer + TILEMAP_INDEX(12, 2), 18, 18, 0);
    TileMap_FillRect(gBG2TilemapBuffer + TILEMAP_INDEX(12, 2), 18, 18, 0);
#if CHAX
    TileMap_FillRect(gBG1TilemapBuffer + TILEMAP_INDEX(12, 2), 18, 18, 0);
#endif

    off = sPageSlideOffsetLut[proc->timer];

    if (off == INT8_MAX)
    {
        // INT8_MAX offset means switch to displaying next page
#if CHAX
        StatScreenDisplayBG1(proc->newItem);
#endif
        DisplayPage(proc->newItem);

        proc->timer++;
        off = sPageSlideOffsetLut[proc->timer];
    }

    // compute len, dstOff and srcOff
    // len is the length of the display bit of the page
    // dstOff is the x offset to which to copy the page to
    // srcOff is the x offset from which to copy the page from

    if (proc->key & DPAD_LEFT)
        off = -off;

    len = 18 - abs(off);

    if (off < 0)
    {
        dstOff = 0;
        srcOff = -off;
    }
    else
    {
        dstOff = off;
        srcOff = 0;
    }

    TileMap_CopyRect(
        gUiTmScratchA + srcOff,
        gBG0TilemapBuffer + dstOff + TILEMAP_INDEX(12, 2),
        len, 18);

    TileMap_CopyRect(
        gUiTmScratchC + srcOff,
        gBG2TilemapBuffer + dstOff + TILEMAP_INDEX(12, 2),
        len, 18);

#if CHAX
    TileMap_CopyRect(
        gUiTmScratchB + srcOff,
        gBG1TilemapBuffer + dstOff + TILEMAP_INDEX(12, 2),
        len, 18);
#endif

    BG_EnableSyncByMask(BG0_SYNC_BIT + BG1_SYNC_BIT + BG2_SYNC_BIT);

    proc->timer++;
    off = sPageSlideOffsetLut[proc->timer];

    if (off == INT8_MIN)
        Proc_Break(proc);
}

/* LynJump */
void UnitSlide_SetNewUnit(struct StatScreenEffectProc* proc)
{
    gStatScreen.unit = GetUnit(proc->newItem);

    StatScreen_Display(Proc_Find(gProcScr_StatScreen));
    Proc_Break(proc);

#if 0
    CallARM_FillTileRect(gBG1TilemapBuffer, Tsa_StatScreenBg1Left, TILEREF(0, 1));
    StatScreenDisplayBG1(gStatScreen.page);
    TileMap_CopyRect(
        gUiTmScratchB,
        gBG1TilemapBuffer + 0 + TILEMAP_INDEX(12, 2),
        18, 18);

    BG_EnableSyncByMask(BG1_SYNC_BIT);
#endif

    /**
     * I have no idea, but the BG1 is disabled
     */
    SetDispEnable(1, 1, 1, 1, 1);
}

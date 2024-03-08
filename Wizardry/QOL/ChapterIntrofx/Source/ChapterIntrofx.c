#include "global.h"
#include "bmfx.h"
#include "hardware.h"
#include "proc.h"
#include "bm.h"
#include "bmio.h"
#include "bmlib.h"
#include "soundwrapper.h"

#include "common-chax.h"
#include "ChapterIntrofx.h"

STATIC_DECLAR void ChapterIntroFE6_Bg3Scroll_Loop(ProcPtr proc);

struct ProcCmd const ProcScr_ChapterIntro_Bg3Scroll[] =
{
    PROC_REPEAT(ChapterIntroFE6_Bg3Scroll_Loop),
    PROC_END,
};

STATIC_DECLAR void ChapterIntroFE6_Bg3Scroll_Loop(ProcPtr proc)
{
    int offset = (GetGameClock()/2) % 0x100;

    BG_SetPosition(3, offset, offset);
}

STATIC_DECLAR void ChapterIntroFE6_KeyListen_Init(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_KeyListen_Loop(struct Proc * proc);

struct ProcCmd const ProcScr_ChapterIntro_KeyListen[] =
{
    PROC_CALL(ChapterIntroFE6_KeyListen_Init),
    PROC_REPEAT(ChapterIntroFE6_KeyListen_Loop),

    PROC_END,
};

STATIC_DECLAR void ChapterIntroFE6_KeyListen_Init(struct Proc * proc)
{
    struct Proc * parent = proc->proc_parent;

    parent->unk50 = 0;
    proc->unk50 = 0;
}

STATIC_DECLAR void ChapterIntroFE6_KeyListen_Loop(struct Proc * proc)
{
    if (gKeyStatusPtr->newKeys & (A_BUTTON | B_BUTTON | START_BUTTON))
        proc->unk50 = 1;

    if (proc->unk50 != 0)
    {
        struct Proc * parent = proc->proc_parent;

        if (parent->unk50)
        {
            Proc_Goto(parent, parent->unk50);
            Proc_End(proc);
        }
    }
}

STATIC_DECLAR void ChapterIntroFE6_Init(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_BeginFadeIn(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_LoopFadeIn(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_BeginMotifFadeIn(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_LoopMotifFadeIn(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_BeginHOpenText(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_LoopHOpenText(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_BeginVOpenText(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_LoopVOpenText(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_Begin_0801E1A0(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_Loop_0801E1F8(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_Begin_0801E220(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_Loop_0801E244(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_0801E2B0(struct Proc * proc);
// STATIC_DECLAR void ChapterIntro_InitMapDisplay(struct Proc * proc);
// STATIC_DECLAR void ChapterIntro_BeginFadeToMap(struct Proc * proc);
// STATIC_DECLAR void ChapterIntro_LoopFadeToMap(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_BeginCloseText(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_LoopCloseText(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_BeginFastCloseText(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_LoopFastCloseText(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_BeginFadeOut(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_LoopFadeOut(struct Proc * proc);
// STATIC_DECLAR void ChapterIntro_BeginFastFadeToMap(struct Proc * proc);
// STATIC_DECLAR void ChapterIntro_LoopFastFadeToMap(struct Proc * proc);
STATIC_DECLAR void ChapterIntroFE6_SetSkipTarget(short arg, struct Proc * proc);

struct ProcCmd const ProcScr_ChapterIntroRe[] =
{
    PROC_CALL(BMapDispSuspend),

    PROC_CALL(ChapterIntroFE6_Init),

    PROC_START_CHILD(ProcScr_ChapterIntro_Bg3Scroll),
    PROC_START_CHILD(ProcScr_ChapterIntro_KeyListen),

    PROC_CALL_ARG(ChapterIntroFE6_SetSkipTarget, 1),

    PROC_CALL(ChapterIntroFE6_BeginFadeIn),
    PROC_REPEAT(ChapterIntroFE6_LoopFadeIn),

    PROC_SLEEP(40),

    PROC_CALL(ChapterIntroFE6_BeginMotifFadeIn),
    PROC_REPEAT(ChapterIntroFE6_LoopMotifFadeIn),

    PROC_SLEEP(80),

    PROC_CALL_ARG(ChapterIntroFE6_SetSkipTarget, 0),

    PROC_CALL(ChapterIntroFE6_BeginHOpenText),
    PROC_REPEAT(ChapterIntroFE6_LoopHOpenText),

    PROC_CALL(ChapterIntroFE6_BeginVOpenText),
    PROC_REPEAT(ChapterIntroFE6_LoopVOpenText),

    PROC_CALL_ARG(ChapterIntroFE6_SetSkipTarget, 1),

    PROC_CALL(ChapterIntroFE6_Begin_0801E1A0),
    PROC_REPEAT(ChapterIntroFE6_Loop_0801E1F8),

    PROC_SLEEP(120),

    PROC_CALL(ChapterIntroFE6_Begin_0801E220),
    PROC_REPEAT(ChapterIntroFE6_Loop_0801E244),

    PROC_END_EACH(ProcScr_ChapterIntro_Bg3Scroll),

    PROC_CALL_ARG(ChapterIntroFE6_SetSkipTarget, 0),

    PROC_CALL(ChapterIntroFE6_0801E2B0),

    PROC_CALL(BMapDispResume),

    PROC_CALL(ChapterIntro_InitMapDisplay),
    PROC_CALL(ChapterIntro_BeginFadeToMap),
    PROC_REPEAT(ChapterIntro_LoopFadeToMap),

    PROC_CALL_ARG(ChapterIntroFE6_SetSkipTarget, 2),

    PROC_SLEEP(90),

    PROC_CALL_ARG(ChapterIntroFE6_SetSkipTarget, 0),

    PROC_CALL(ChapterIntroFE6_BeginCloseText),
    PROC_REPEAT(ChapterIntroFE6_LoopCloseText),

    PROC_SLEEP(30),

    PROC_GOTO(99),

PROC_LABEL(1),
    PROC_CALL(ChapterIntroFE6_BeginFadeOut),
    PROC_REPEAT(ChapterIntroFE6_LoopFadeOut),

    PROC_CALL(ChapterIntroFE6_0801E2B0),

    PROC_CALL(BMapDispResume),

    PROC_CALL(ChapterIntro_InitMapDisplay),

    PROC_CALL(ChapterIntro_BeginFastFadeToMap),
    PROC_REPEAT(ChapterIntro_LoopFastFadeToMap),

    PROC_GOTO(99),

PROC_LABEL(2),
    PROC_CALL(ChapterIntroFE6_BeginFastCloseText),
    PROC_REPEAT(ChapterIntroFE6_LoopFastCloseText),

PROC_LABEL(99),
    PROC_CALL(ChapterIntro_80210C8),
    PROC_END,
};

void PutChapterIntroMotifFE6(void)
{
    int ix, iy;
    int tile = 0;
    u16 * tm_buf = (u16 *)gGenericBuffer;

    BG_Fill(gBG2TilemapBuffer, 0);

    Decompress(Tm_ChapterIntroMotifFE6, tm_buf);

    for (iy = 0; iy < 18; ++iy)
        for (ix = 0; ix < 24; ++ix)
            gBG2TilemapBuffer[TILEMAP_INDEX(3 + ix, 1 + iy)] = TILEREF(1 + tm_buf[tile++], BGPAL_CHAPTERINTRO_MOTIF);
}

STATIC_DECLAR void ChapterIntroFE6_Init(struct Proc * proc)
{
    InitBmBgLayers();

    BG_SetPosition(0, 0, 0);
    BG_SetPosition(1, 0, 0);
    BG_SetPosition(2, 0, 0);

    BG_Fill(gBG0TilemapBuffer, 0);
    BG_Fill(gBG1TilemapBuffer, 0);
    BG_Fill(gBG2TilemapBuffer, 0);
    BG_Fill(gBG3TilemapBuffer, 0);

    SetBackgroundTileDataOffset(2, CHR_SIZE * BGCHR_CHAPTERINTRO_MOTIF);

    SetWinEnable(1, 0, 0);
    SetWin0Layers(1, 1, 1, 1, 1);
    SetWOutLayers(0, 0, 1, 1, 1);

    gLCDControlBuffer.wincnt.win0_enableBlend = 1;
    gLCDControlBuffer.wincnt.wout_enableBlend = 1;

    SetWin0Box(0, 0, 0, 0);

    // sub_80895B4(8, BGPAL_CHAPTERINTRO_0);
    ApplyPalette(Pal_Unk_0830D5E4, BGPAL_CHAPTERINTRO_0);
    sub_80895B4(0, BGPAL_CHAPTERINTRO_1);

    sub_80896A8(BGCHR_CHAPTERINTRO_80);

    sub_8089624(0x100, sub_808979C(&gPlaySt));

    sub_8089744(gBG1TilemapBuffer + TILEMAP_INDEX(0, 8), BGPAL_CHAPTERINTRO_0);
    sub_80896D8(gBG0TilemapBuffer + TILEMAP_INDEX(3, 9), BGPAL_CHAPTERINTRO_1);

    MaybeResetSomePal();
    sub_80017B4(BGPAL_CHAPTERINTRO_0, 2, 0x40, -1);
    CALLARM_ColorFadeTick();

    EnablePaletteSync();

    Decompress(Img_ChapterIntroFogFE6, (u8 *) VRAM + CHR_SIZE * BGCHR_CHAPTERINTRO_FOG);
    ApplyPalette(Pal_ChapterIntroFogFE6, BGPAL_CHAPTERINTRO_FOG);

    Decompress(Img_ChapterIntroMotifFE6, (u8 *) VRAM + CHR_SIZE * (BGCHR_CHAPTERINTRO_MOTIF + 1));
    ApplyPalette(Pal_ChapterIntroMotifFE6, BGPAL_CHAPTERINTRO_MOTIF);

    RegisterBlankTile(BGCHR_CHAPTERINTRO_MOTIF);

    gPaletteBuffer[0] = 0;

    PutChapterIntroMotifFE6();
    PutScreenFogEffect();

    BG_EnableSyncByMask(BG0_SYNC_BIT + BG1_SYNC_BIT + BG2_SYNC_BIT + BG3_SYNC_BIT);
}

STATIC_DECLAR void ChapterIntroFE6_SetSkipTarget(short arg, struct Proc * proc)
{
    proc->unk50 = arg;
}

STATIC_DECLAR void ChapterIntroFE6_BeginFadeIn(struct Proc * proc)
{
    SetDispEnable(0, 0, 0, 1, 1);
    SetBlendTargetA(0, 0, 0, 1, 1);

    proc->unk4C = 12;

    Sound_FadeOutBGM(2);
}

STATIC_DECLAR void ChapterIntroFE6_LoopFadeIn(struct Proc * proc)
{
    SetBlendDarken(proc->unk4C);

    if (proc->unk50 == 3 || (GetGameClock() % 4) == 0)
    {
        proc->unk4C--;

        if (proc->unk4C < 0)
            Proc_Break(proc);
    }
}

STATIC_DECLAR void ChapterIntroFE6_BeginMotifFadeIn(struct Proc * proc)
{
    SetDispEnable(0, 0, 1, 1, 1);

    proc->unk4C = 0x10;

    SetBlendTargetA(0, 0, 1, 0, 0);
    SetBlendTargetB(0, 0, 0, 1, 0);

    PlaySoundEffect(0x90);
}

STATIC_DECLAR void ChapterIntroFE6_LoopMotifFadeIn(struct Proc * proc)
{
    SetBlendAlpha(0x10 - proc->unk4C, 0x10);

    if (proc->unk50 == 3 || (GetGameClock() % 4) == 0)
    {
        proc->unk4C--;

        if (proc->unk4C < 0)
            Proc_Break(proc);
    }
}

STATIC_DECLAR void ChapterIntroFE6_BeginHOpenText(struct Proc * proc)
{
    proc->unk4C = 0;

    SetDispEnable(1, 1, 1, 1, 1);
    PlaySoundEffect(0x91);
}

STATIC_DECLAR void ChapterIntroFE6_LoopHOpenText(struct Proc * proc)
{
    int val = Interpolate(INTERPOLATE_RSQUARE, 0, DISPLAY_WIDTH/2, proc->unk4C, 0x10);
    SetWin0Box(DISPLAY_WIDTH/2 - val, 78, DISPLAY_WIDTH/2 + val, 81);

    proc->unk4C++;

    if (proc->unk4C > 0x10)
        Proc_Break(proc);
}

STATIC_DECLAR void ChapterIntroFE6_BeginVOpenText(struct Proc * proc)
{
    proc->unk4C = 1;
}

STATIC_DECLAR void ChapterIntroFE6_LoopVOpenText(struct Proc * proc)
{
    int val = Interpolate(INTERPOLATE_LINEAR, 0, 0x10, proc->unk4C, 3);
    SetWin0Box(0, DISPLAY_HEIGHT/2 - val, DISPLAY_WIDTH, DISPLAY_HEIGHT/2 + val);

    proc->unk4C++;

    if (proc->unk4C > 3)
        Proc_Break(proc);
}

/* Display chapter title */
STATIC_DECLAR void ChapterIntroFE6_Begin_0801E1A0(struct Proc * proc)
{
    proc->unk4C = 0x20;

    SetBlendAlpha(0x10, 0x10);

    SetBlendTargetA(0, 1, 1, 0, 0);
    SetBlendTargetB(0, 0, 0, 1, 1);
}

STATIC_DECLAR void ChapterIntroFE6_Loop_0801E1F8(struct Proc * proc)
{
    CALLARM_ColorFadeTick();
    EnablePaletteSync();

    proc->unk4C--;

    if (proc->unk4C < 0)
        Proc_Break(proc);
}

STATIC_DECLAR void ChapterIntroFE6_Begin_0801E220(struct Proc * proc)
{
    proc->unk4C = 13;

    MaybeResetSomePal();
    MaybeSmoothChangeSomePal(gPaletteBuffer + 0x10 * BGPAL_CHAPTERINTRO_FOG, BGPAL_CHAPTERINTRO_FOG, 2, -1);
}

STATIC_DECLAR void ChapterIntroFE6_Loop_0801E244(struct Proc * proc)
{
    if ((GetGameClock() % 4) == 0)
    {
        CALLARM_ColorFadeTick();
        EnablePaletteSync();

        proc->unk4C--;

        if (proc->unk4C < 0)
        {
            SetDispEnable(1, 1, 0, 0, 0);
            SetBackgroundTileDataOffset(2, 0);

            gPaletteBuffer[0] = 0;
            EnablePaletteSync();

            Proc_Break(proc);
        }
    }
}

STATIC_DECLAR void ChapterIntroFE6_0801E2B0(struct Proc * proc)
{
    gBmSt.camera.y = 40 * 16;
}

STATIC_DECLAR void ChapterIntroFE6_BeginCloseText(struct Proc * proc)
{
    proc->unk4C = 0x10;
}

STATIC_DECLAR void ChapterIntroFE6_LoopCloseText(struct Proc * proc)
{
    SetWin0Box(0, DISPLAY_HEIGHT/2 - proc->unk4C, DISPLAY_WIDTH, DISPLAY_HEIGHT/2 + proc->unk4C);

    proc->unk4C--;

    if (proc->unk4C < 0)
        Proc_Break(proc);
}

STATIC_DECLAR void ChapterIntroFE6_BeginFastCloseText(struct Proc * proc)
{
    proc->unk4C = 8;
}

STATIC_DECLAR void ChapterIntroFE6_LoopFastCloseText(struct Proc * proc)
{
    SetWin0Box(0, DISPLAY_HEIGHT/2 - proc->unk4C, DISPLAY_WIDTH, DISPLAY_HEIGHT/2 + proc->unk4C);

    proc->unk4C -= 2;

    if (proc->unk4C < 0)
        Proc_Break(proc);
}

STATIC_DECLAR void ChapterIntroFE6_BeginFadeOut(struct Proc * proc)
{
    MaybeResetSomePal();
    MaybeSmoothChangeSomePal(PAL_BG(0), 0, 6, -2);

    proc->unk4C = 15;

    Sound_FadeOutSE(1);
}

STATIC_DECLAR void ChapterIntroFE6_LoopFadeOut(struct Proc * proc)
{
    CALLARM_ColorFadeTick();
    EnablePaletteSync();

    proc->unk4C--;

    if (proc->unk4C < 0)
    {
        SetDispEnable(0, 0, 0, 0, 0);
        SetBackgroundTileDataOffset(2, 0);
        Proc_Break(proc);
    }
}

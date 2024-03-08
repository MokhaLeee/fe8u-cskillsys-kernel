#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"
#include "ctc.h"
#include "hardware.h"
#include "bmlib.h"
#include "bmitem.h"
#include "fontgrp.h"
#include "sysutil.h"
#include "bksel.h"
#include "bmmind.h"

#include "common-chax.h"
#include "combat-art.h"
#include "battle-system.h"
#include "constants/texts.h"

extern s8 sCombatArtBKSELfxTimer;
extern const u16 Img_BkselObjArrow[];
extern const u16 Pal_BkselObjArrow[];
extern u16 Tsa_BkselDetailedScreen[];
extern u16 Tsa_BkselStandardScreen[];

enum CombatArtBKSELfxConfig {
    // Real VRAM Offset to uncompress: OBJ_VRAM0 + OBJ_MOKHA_VRAMOFF
    OBJ_ARROW_VRAMOFF = 0x0320, 

    // Real Palette index: OBJ_ARROW_PAL + 0x10
    OBJ_ARROW_PAL = 0x3,

    // Obj config
    OBJ_ARROW_L_XPOS = -1,
    OBJ_ARROW_R_XPOS = 0x4B,
    OBJ_ARROW_YPOS = 0x16A,
};

STATIC_DECLAR void BKSELfxUpdateAoeSprits(struct BattleForecastProc * proc)
{
    const u8 Y_SYNC_OFFs[] = {0, 1, 2, 3, 2, 1};
    const u8 Y_SYNC_TIMEs[] = {0, 0, 1, 2, 2};
    const s8 X_TILEs[] = {-16, 0, 0, 0, 16};
    const s8 Y_TILEs[] = {0, 16, 0, -16, 0};

    int i;
    u8 cid = GetCombatArtByTargetSelIndex();

    if (!COMBART_VALID(cid) || !gpCombatArtInfos[cid].aoe_debuff)
        return;

    for (i = 0; i < 5; i++)
    {
        int ix = gBattleTarget.unit.xPos * 16 - gBmSt.camera.x + X_TILEs[i];
        int iy = gBattleTarget.unit.yPos * 16 - gBmSt.camera.y + Y_TILEs[i];

        if(ix > 0xF0 || iy > 0xA0)
            continue;

        PutSprite(
            0,
            0x1FF & (ix + 0x204),
            0xFF  & (iy + 0x103 + Y_SYNC_OFFs[sCombatArtBKSELfxTimer + Y_SYNC_TIMEs[i]]),
            gObject_8x8,
            OAM2_PAL(OBJ_ARROW_PAL) + OAM2_LAYER(0b00) + OAM2_CHR(OBJ_ARROW_VRAMOFF / 0x20)
        );
    }
}

STATIC_DECLAR void BKSELfxUpdateSprits(struct BattleForecastProc * proc)
{
    const int X = proc->side      == -1 ? 0 : 0x9F;
    const int Y = proc->frameKind ==  1 ? 0 : 0x20;
    const int VOFF = (OBJ_ARROW_VRAMOFF / 0x20) + 1 + sCombatArtBKSELfxTimer;

    if (!(GetGameClock() & 0x7))
    {
        sCombatArtBKSELfxTimer++;
        if (sCombatArtBKSELfxTimer >= 6)
            sCombatArtBKSELfxTimer = 0;
    }

    if (!CombatArtSelectTargetExist())
        return;

    /* Put arrow */
    PutSprite(
        0,
        OBJ_ARROW_L_XPOS + X,
        OBJ_ARROW_YPOS + Y,
        gObject_8x16,
        OAM2_PAL(OBJ_ARROW_PAL) + OAM2_LAYER(0b00) + OAM2_CHR(VOFF)
    );

    PutSprite(
        0,
        OBJ_ARROW_R_XPOS + X,
        OBJ_ARROW_YPOS + Y,
        gObject_8x16_HFlipped,
        OAM2_PAL(OBJ_ARROW_PAL) + OAM2_LAYER(0b00) + OAM2_CHR(VOFF)
    );

    BKSELfxUpdateAoeSprits(proc);
}

/* External jump */
void BattleForecast_InitRework(struct BattleForecastProc * proc)
{
    /* Vanilla */
    BattleForecast_Init(proc);
    StartGreenText(proc);

    if (CanUnitPlayCombatArt(gActiveUnit, GetItemFormSlot(gActiveUnit, gActionData.itemSlotIndex)))
    {
        sCombatArtBKSELfxTimer = 0;

        Decompress(Img_BkselObjArrow, gGenericBuffer);
        Copy2dChr(gGenericBuffer, OBJ_VRAM0 + OBJ_ARROW_VRAMOFF, 7, 2);

        ApplyPalette(Pal_BkselObjArrow, OBJ_ARROW_PAL + 0x10);

        StartParallelWorker(BKSELfxUpdateSprits, proc);
    }
}

STATIC_DECLAR void DrawBattleForecastContentsStandardRework(struct BattleForecastProc * proc)
{
    DrawBattleForecastContentsStandard(proc);

    if (CanUnitPlayCombatArt(gActiveUnit, GetItemFormSlot(gActiveUnit, gActionData.itemSlotIndex)))
        CallARM_FillTileRect(gUiTmScratchB, Tsa_BkselStandardScreen, 0x1000);
}

STATIC_DECLAR void DrawBattleForecastContentsExtendedRework(struct BattleForecastProc * proc)
{
    DrawBattleForecastContentsExtended(proc);

    if (CanUnitPlayCombatArt(gActiveUnit, GetItemFormSlot(gActiveUnit, gActionData.itemSlotIndex)))
        CallARM_FillTileRect(gUiTmScratchB, Tsa_BkselDetailedScreen, 0x1000);
}

STATIC_DECLAR void DrawBattleForecastContentsVanilla(struct BattleForecastProc * proc)
{
    proc->unk_2C = 0;
    proc->needContentUpdate = 0;

    switch (proc->frameKind) {
    case 1:
        InitBattleForecastBattleStats(proc);
        DrawBattleForecastContentsStandardRework(proc);
        break;

    case 2:
        InitBattleForecastBattleStats(proc);
        DrawBattleForecastContentsExtendedRework(proc);
        break;
    }
}

/* LynJump */
void DrawBattleForecastContents(struct BattleForecastProc * proc)
{
    u8 cid;
    char * str;
    struct Text * text = &proc->itemNameText;
    const int Y = proc->frameKind ==  1 ? 0x0D : 0x11;

    /* Vanilla */
    DrawBattleForecastContentsVanilla(proc);

    ClearText(text);

    if (!CanUnitPlayCombatArt(gActiveUnit, GetItemFormSlot(gActiveUnit, gActionData.itemSlotIndex)))
    {
        /* If no arts, draw item name */
        str = GetItemName(gBattleTarget.weaponBefore);

        PutDrawText(
            text,
            TILEMAP_LOCATED(gUiTmScratchA, 0x1, Y),
            TEXT_COLOR_SYSTEM_WHITE,
            GetStringTextCenteredPos(0x40, str),
            0, str
        );
        return;
    }

    cid = GetCombatArtByTargetSelIndex();

    if (!COMBART_VALID(cid))
    {
        str = GetStringFromIndex(MSG_COMBATART_BKSEL_NO_ARTS);

        PutDrawText(
            text,
            TILEMAP_LOCATED(gUiTmScratchA, 0x1, Y),
            TEXT_COLOR_SYSTEM_WHITE,
            GetStringTextCenteredPos(0x40, str),
            0, str
        );
    }
    else
    {
        str = GetStringFromIndex(GetCombatArtName(cid));

        PutDrawText(
            text,
            TILEMAP_LOCATED(gUiTmScratchA, 0x1, Y),
            TEXT_COLOR_SYSTEM_GREEN,
            GetStringTextCenteredPos(0x40, str),
            0, str
        );
    }
}


#include "C_Code.h" // headers
#include "bmshop.h"

#define PUREFUNC __attribute__((pure))
int Mod(int a, int b) PUREFUNC;
// vanilla UI images
extern const u8 gUiFrameImage[];
extern u8 gGfx_PlayerInterfaceFontTiles[];
extern u8 gGfx_PlayerInterfaceNumbers[];
extern u8 CONST_DATA gUnknown_08A1B730[];
extern u8 gUnknown_08A1B1FC[];
extern u16 Img_ShopGoldBox[];
extern u16 CONST_DATA Img_PrepItemUseScreen[];
extern u8 Img_PrepPopupWindow[];

// battle
extern u16 gUnknown_08802558[]; // palette in func EfxPrepareScreenFx
// TSA ?
extern u8 gUnknown_0880210C[]; // ekrGaugeMain
extern u8 gUnknown_088021C0[];
extern u8 gUnknown_08802274[];
extern u8 gUnknown_08802348[];
extern u8 gUnknown_08802348_Fenreir[];
extern u8 gUnknown_08802428[]; // ekrGaugeMain
extern u8 gUnknown_08802508[]; // EfxPrepareScreenFx
// Img
extern u16 Img_08801C14[];
extern u16 Img_EfxLeftNameBox[];
extern u16 Img_EfxLeftItemBox[];
extern u16 Img_EfxRightNameBox[];
extern u16 Img_EfxRightItemBox[];

extern const u8 gUiFrameImage_Stephano[];
extern const u8 gGfx_PlayerInterfaceFontTiles_Stephano[];
extern const u8 gGfx_PlayerInterfaceNumbers_Stephano[];
extern const u8 gPrepExtra1_Stephano[]; // gUnknown_08A1B730
extern const u8 gPrepUseGfx_Stephano[]; // Img_PrepItemUseScreen

extern const u8 gUiFrameImage_Gamma[];
extern const u8 gUiFrameImage_Pikmin[];
extern const u8 gPrepExtra1_Gamma[];     // gUnknown_08A1B730
extern const u8 gPrepExtra1_Pikmin[];    // gUnknown_08A1B730
extern const u8 gPrepUseGfx_Gamma[];     // Img_PrepItemUseScreen
extern const u8 gPrepUseGfx_Pikmin[];    // Img_PrepItemUseScreen
extern const u8 gPrepItemGfx_Gamma[];    // gUnknown_08A1B1FC
extern const u8 Gamma_PrepPopupWindow[]; // Img_PrepPopupWindow

// battle
extern u16 gUnknown_08802558_Sokaballa[]; // palette in func EfxPrepareScreenFx
// TSA
extern u8 gUnknown_0880210C_Sokaballa[]; // ekrGaugeMain
extern u8 gUnknown_088021C0_Alt[];
extern u8 gUnknown_088021C0_Fenreir[];
extern u8 gUnknown_08802274_Alt[];
extern u8 gUnknown_08802274_Fenreir[];
extern u8 gUnknown_08802348_Alt[];
extern u8 gUnknown_08802428_Alt[]; // ekrGaugeMain
extern u8 gUnknown_08802508_Alt[]; // EfxPrepareScreenFx
// img
extern u16 Img_08801C14_Sokaballa[];
extern u16 Img_EfxLeftNameBox_Sokaballa[];
extern u16 Img_EfxLeftItemBox_Sokaballa[];
extern u16 Img_EfxRightNameBox_Sokaballa[];
extern u16 Img_EfxRightItemBox_Sokaballa[];

extern u8 gUnknown_0880210C_Gamma[]; // ekrGaugeMain
extern u8 gUnknown_088021C0_Gamma[];
extern u8 gUnknown_08802274_Gamma[];
extern u8 gUnknown_08802348_Gamma[];
extern u8 gUnknown_08802428_Gamma[];
extern u8 gUnknown_0880210C_Fenreir[];  // ekrGaugeMain
extern u16 gUnknown_08802558_Gamma[];   // palette in func EfxPrepareScreenFx
extern u16 gUnknown_08802558_Fenreir[]; // palette in func EfxPrepareScreenFx
extern u16 Img_08801C14_Gamma[];
extern u16 Img_08801C14_Fenreir[];
extern u16 Img_EfxLeftNameBox_Gamma[];
extern u16 Img_EfxLeftNameBox_Fenreir[];
extern u16 Img_EfxLeftItemBox_Gamma[];
extern u16 Img_EfxLeftItemBox_Fenreir[];
extern u16 Img_EfxRightNameBox_Gamma[];
extern u16 Img_EfxRightNameBox_Fenreir[];
extern u16 Img_EfxRightItemBox_Gamma[];
extern u16 Img_EfxRightItemBox_Fenreir[];
extern u16 Img_ShopGoldBox_Gamma[];

extern int StephanoStyleFlag;
extern int GammaStyleFlag;
extern int PikminStyleFlag;
int GetUI_id(void)
{
    if (CheckFlag(StephanoStyleFlag))
    {
        return 1;
    }
    if (CheckFlag(GammaStyleFlag))
    {
        return 2;
    }
    if (CheckFlag(PikminStyleFlag))
    {
        return 3;
    }

    return 0;
}
int GetUIPalID(void)
{
    return GetUI_id();
}

extern const void * sUiFrameImage;
static const void * sUiFrame[] = {

    gUiFrameImage, // vanilla
    gUiFrameImage_Stephano,
    gUiFrameImage_Gamma,
    gUiFrameImage_Pikmin,
};

const u8 * GetUiFrame(void)
{
    int id = GetUI_id();
    if (!id)
    {
        return sUiFrameImage;
    }
    return sUiFrame[id];
}

extern const void * poinGfx_PlayerInterfaceFontTiles; // poin 8a167c8
static const void * sPIFontTiles[] = {

    gGfx_PlayerInterfaceFontTiles, // vanilla
    gGfx_PlayerInterfaceFontTiles_Stephano,
    gGfx_PlayerInterfaceFontTiles_Stephano,
    gGfx_PlayerInterfaceFontTiles_Stephano,
};

const u8 * GetPIFontTiles(void)
{
    int id = GetUI_id();
    if (!id)
    {
        return poinGfx_PlayerInterfaceFontTiles;
    }
    return sPIFontTiles[id];
}

extern const void * poinGfx_PlayerInterfaceNumbers; // poin 8a16d6c
static const void * sPINumbers[] = {

    gGfx_PlayerInterfaceNumbers, // vanilla
    gGfx_PlayerInterfaceNumbers_Stephano,
    gGfx_PlayerInterfaceNumbers_Stephano,
    gGfx_PlayerInterfaceNumbers_Stephano,
};

const u8 * GetPINumbers(void)
{
    int id = GetUI_id();
    if (!id)
    {
        return poinGfx_PlayerInterfaceNumbers;
    }
    return sPINumbers[id];
}

void PlayerPhaseInterfaceNumbersHook(void)
{
    Decompress(GetPIFontTiles(), (void *)(VRAM + 0x2000));
    Decompress(GetPINumbers(), (void *)(VRAM + 0x15C00));
}

// AtMenu_Reintialize 08095c84
// gUnknown_08A1B698? gUnknown_08A1B698, gUnknown_08A1B658

// gUnknown_08A1B7C8
// A1B7C8

extern const void * poinUnknown_08A1B658; // Link arena something UI tsa?
extern const u8 gUnknown_08A1B698_Gamma[];
extern const void * poinUnknown_08A1B698;

static const void * sUnknown_08A1B698[] = {

    gUnknown_08A1B698, // vanilla
    gUnknown_08A1B698,
    gUnknown_08A1B698_Gamma,
    gUnknown_08A1B698,
};

const u8 * GetUnknown_08A1B698(void)
{
    int id = GetUI_id();
    if (!id)
    {
        return poinUnknown_08A1B698;
    }
    return sUnknown_08A1B698[id];
}

void PrepUiTSAHook(void)
{
    if (CheckInLinkArena())
    {
        Decompress(GetUnknown_08A1B698(), gGenericBuffer);
        CallARM_FillTileRect(TILEMAP_LOCATED(gBG1TilemapBuffer, 1, 5), gGenericBuffer, 0x1000);
    }
    else
    {
        Decompress(gUnknown_08A1B658, gGenericBuffer);
        CallARM_FillTileRect(TILEMAP_LOCATED(gBG1TilemapBuffer, 0x10, 2), gGenericBuffer, 0x1000);
        Decompress(GetUnknown_08A1B698(), gGenericBuffer);
        CallARM_FillTileRect(TILEMAP_LOCATED(gBG1TilemapBuffer, 1, 6), gGenericBuffer, 0x1000);
    }
}

extern const void * poinUnknown_08A1B730;
static const void * sPrepExtra1[] = {

    gUnknown_08A1B730, // vanilla
    gPrepExtra1_Stephano,
    gPrepExtra1_Gamma,
    gPrepExtra1_Pikmin,
};

const u8 * GetPrepExtra1(void)
{
    int id = GetUI_id();
    if (!id)
    {
        return poinUnknown_08A1B730;
    }
    return sPrepExtra1[id];
}

extern const u8 gUnknown_08A1B7C8_Gamma[];
extern const void * poinUnknown_08A1B7C8;
static const void * sUnknown_08A1B7C8[] = {

    gUnknown_08A1B7C8, // vanilla
    gUnknown_08A1B7C8,
    gUnknown_08A1B7C8_Gamma,
    gUnknown_08A1B7C8,
};

const u8 * GetUnknown_08A1B7C8(void)
{
    int id = GetUI_id();
    if (!id)
    {
        return poinUnknown_08A1B7C8;
    }
    return sUnknown_08A1B7C8[id];
}

void PrepInitGfxHook(void)
{
    sub_80950E8(0x6000, 0xF);

    Decompress(GetPrepExtra1(), (void *)0x06000440);
    Decompress(GetUnknown_08A1B7C8(), gGenericBuffer);
    asm("mov r11, r11");
    CallARM_FillTileRect(gBG1TilemapBuffer, gGenericBuffer, 0x1000);
}

extern const void * poinImg_PrepItemUseScreen;
static const void * sPrepUseItem[] = {

    Img_PrepItemUseScreen, // vanilla
    gPrepUseGfx_Stephano,
    gPrepUseGfx_Gamma,
    gPrepUseGfx_Pikmin,
};

const u8 * GetPrepUseItem(void)
{
    int id = GetUI_id();
    if (!id)
    {
        return poinImg_PrepItemUseScreen;
    }
    return sPrepUseItem[id];
}

void Decompress_Img_PrepItemUseScreen(void)
{
    Decompress(GetPrepUseItem(), (void *)BG_VRAM + 0x440);
}

extern const void * poinUnknown_08A1B1FC;
static const void * sPrepItem[] = {

    gUnknown_08A1B1FC, // vanilla
    gPrepItemGfx_Gamma,
    gPrepItemGfx_Gamma,
    gPrepItemGfx_Gamma,
};

const u8 * GetPrepItem(void)
{
    int id = GetUI_id();
    if (!id)
    {
        return poinUnknown_08A1B1FC;
    }
    return sPrepItem[id];
}

void DrawFundsSprite_Init(struct DrawFundsSpriteProc * proc)
{
    Decompress(GetPrepItem(), (void *)0x06013000);
    ApplyPalette(gUnknown_08A1B638, proc->pal + 0x10);

    return;
}

extern const void * poinImg_PrepPopupWindow;
static const void * sPrepPopupWindow[] = {

    Img_PrepPopupWindow, // vanilla
    Gamma_PrepPopupWindow,
    Gamma_PrepPopupWindow,
    Gamma_PrepPopupWindow,
};

const u8 * GetPrepPopupWindow(void)
{
    int id = GetUI_id();
    if (!id)
    {
        return poinImg_PrepPopupWindow;
    }
    return sPrepPopupWindow[id];
}

void PutImg_PrepPopupWindow(int vram, int pal)
{
    Decompress(GetPrepPopupWindow(), (void *)(0x06010000 + vram));
    ApplyPalette(gUiFramePaletteD, pal + 0x10);
    return;
}

enum bmshop_bgchr
{
    OBJCHR_SHOP_SPINARROW = 0x4800 / 0x20,
    OBJPAL_SHOP_SPINARROW = 3,

    OBJCHR_SHOP_GOLDBOX = 0x4C00 / 0x20,
    OBJPAL_SHOP_GOLDBOX = 4,

    BGPAL_SHOP_4 = 4,
    BGPAL_SHOP_MAINBG = 14,
};
// void DecompressImg_ShopGoldBox(void)
// {
// Decompress(Img_ShopGoldBox, OBJ_CHR_ADDR(OBJCHR_SHOP_GOLDBOX));
// }

void UnpackUiFrameImage(void * dest)
{
    const u8 * UiFrame = GetUiFrame();
    if (dest == NULL)
        dest = BG_CHAR_ADDR(0);

    Decompress(UiFrame, dest);
}

void UnpackUiFrameBuffered(int id)
{
    const u8 * UiFrame = GetUiFrame();
    int bufSize;
    s8 * bufAddr;

    if (id < 0)
        id = gPlaySt.config.windowColor;

    bufSize = GetDataSize(UiFrame);
    bufAddr = gFadeComponentStep - bufSize;

    Decompress(UiFrame, bufAddr);
    RegisterDataMove(bufAddr, BG_CHAR_ADDR(0), bufSize);

    UnpackUiFramePalette(-1);
}

// battle below

struct ProcEkrDispUP2
{
    PROC_HEADER;

    /* 29 */ u8 sync;
    /* 2A */ u8 asnyc;

    /* 2B */ u8 _pad_2B[0x32 - 0x2B];

    /* 32 */ u16 x; /* unused actually */

    /* 34 */ u8 _pad_32[0x3A - 0x34];

    /* 3A */ u16 y;

    /* 3C */ u8 _pad_3C[0x4C - 0x3C];

    /* 4C */ u32 unk4C;
    /* 50 */ u32 unk50;
};
// decomp says [1], not [16] !
#define gBG0TilemapBuffer2D ((u16(*)[16])gBG0TilemapBuffer)
extern const void * poinUnknown_0880210C;
static const void * sBUiFrame_880210C[] = {

    gUnknown_0880210C, // vanilla
    gUnknown_0880210C_Sokaballa, gUnknown_0880210C_Gamma,
    gUnknown_0880210C_Fenreir, // top left box for name TSA
};
const u8 * GetBUiFrame_880210C(void)
{
    int id = GetUI_id();
    if (!id)
    {
        return poinUnknown_0880210C;
    }
    return sBUiFrame_880210C[id];
}
extern const void * poinUnknown_088021C0;
static const void * sBUiFrame_88021C0[] = {

    gUnknown_088021C0, // vanilla
    gUnknown_088021C0_Alt, gUnknown_088021C0_Gamma,
    gUnknown_088021C0_Fenreir, // top right box for name TSA
};
const u8 * GetBUiFrame_88021C0(void)
{
    int id = GetUI_id();
    if (!id)
    {
        return poinUnknown_088021C0;
    }
    return sBUiFrame_88021C0[id];
}

void ekrDispUPMain(struct ProcEkrDispUP * var)
{
    struct ProcEkrDispUP2 * proc = (struct ProcEkrDispUP2 *)var;
    int val0, iy, height, map_idx, ix1;
    int ix2 = 15; // for matching

    if (proc->asnyc == true)
        return;

    if (proc->sync != false)
        return;

    val0 = (proc->y << 0x10) >> 0x13;
    iy = val0 << 5;
    if (iy < 0)
        iy = 0;

    height = val0 + 7;
    if (height > 6)
        height = 6;

    map_idx = 30 * (6 - height);

    if (gEkrDistanceType >= 0)
    {
        if (gEkrDistanceType <= 2)
            ix1 = 0;
        else
            goto label;
    }
    else
    {
        ix1 = 0; // for matching, can be any value
    label:
        ix1 = 15;
    }

    FillBGRect(gBG0TilemapBuffer, 30, 7, 0, 128);

    if (height > 0)
    {
        if (proc->unk4C == 0)
        {
            EfxTmCpyBG(&GetBUiFrame_880210C()[map_idx], &gBG0TilemapBuffer2D[iy][ix1], 15, height, -1, -1);
            sub_8070D04(&gBG0TilemapBuffer2D[iy][ix1], 15, height, 2, 128);
        }

        if (proc->unk50 == 0)
        {
            EfxTmCpyBG(&GetBUiFrame_88021C0()[map_idx], &gBG0TilemapBuffer2D[iy][ix2], ix2, height, -1, -1);
            sub_8070D04(&gBG0TilemapBuffer2D[iy][ix2], 15, height, 3, 128);
        }
    }

    BG_EnableSyncByMask(BG0_SYNC_BIT);
}

static const void * sUnknown_08802274[] = {

    gUnknown_08802274, // vanilla
    gUnknown_08802274, gUnknown_08802274_Gamma,
    gUnknown_08802274_Fenreir, // bottom left side TSA
};
extern const void * poinUnknown_08802274;
const u8 * ekrGaugeMain_GetImg()
{
    int id = GetUI_id();
    if (!id)
    {
        return poinUnknown_08802274;
    }
    return sUnknown_08802274[id];
}

extern const void * poinUnknown_08802348;
static const void * sUnknown_08802348[] = {

    gUnknown_08802348, // vanilla
    gUnknown_08802348, gUnknown_08802348_Gamma,
    gUnknown_08802348_Fenreir, // bottom right side TSA
};

const u8 * ekrGaugeMain2_GetImg()
{
    int id = GetUI_id();
    if (!id)
    {
        return poinUnknown_08802348;
    }
    return sUnknown_08802348[id];
}

extern const void * poinUnknown_08802428;
static const void * sUnknown_08802428[] = {

    gUnknown_08802428, // vanilla
    // gUnknown_08802428_Alt,
    gUnknown_08802428, gUnknown_08802428_Gamma,
    gUnknown_08802348_Fenreir, // bottom right side TSA alternate
};

const u8 * ekrGaugeMain3_GetImg()
{
    int id = GetUI_id();
    if (!id)
    {
        return poinUnknown_08802428;
    }
    return sUnknown_08802428[id];
}

extern const void * poinUnknown_08802508;
static const void * sUnknown_08802508[] = {

    gUnknown_08802508, // vanilla
    gUnknown_08802508_Alt,
    gUnknown_08802508,
    gUnknown_08802508,
};

const u8 * GetUnknown_08802508()
{
    int id = GetUI_id();
    if (!id)
    {
        return poinUnknown_08802508;
    }
    return sUnknown_08802508[id];
}

extern const void * poinImg_08801C14;
static const void * sImg_08801C14[] = {

    Img_08801C14, // vanilla
    Img_08801C14_Sokaballa,
    Img_08801C14_Gamma,
    Img_08801C14_Fenreir,
};

const u8 * GetImg_08801C14()
{
    int id = GetUI_id();
    if (!id)
    {
        return poinImg_08801C14;
    }
    return sImg_08801C14[id];
}

extern const void * poinUnknown_08802558;
static const void * sBPal_8802558[] = {

    gUnknown_08802558, // vanilla
    gUnknown_08802558_Sokaballa,
    gUnknown_08802558_Gamma,
    gUnknown_08802558_Fenreir,
};
const u16 * GetBPal_8802558(void)
{
    int id = GetUI_id();
    if (!id)
    {
        return poinUnknown_08802558;
    }
    return sBPal_8802558[id];
}

// extern const u16 gUnknown_08802598[];
// static const void * sBPal_8802598[] = {

// gUnknown_08802598, // vanilla
// gUnknown_08802598,
// gUnknown_08802598,
// gUnknown_08802598,
// };
// const u8 * GetBPal_8802598(void)
// {
// int id = GetUI_id();
// return sBPal_8802598[id];
// }

extern s16 gBanimFactionPal[2];
// void EfxPrepareScreenFxHook()
// {
// CpuFastCopy(&PAL_BUF_COLOR(GetBPal_8802558(), gBanimFactionPal[POS_L], 0), PAL_BG(0x2), 0x20);
// CpuFastCopy(&PAL_BUF_COLOR(GetBPal_8802558(), gBanimFactionPal[POS_R], 0), PAL_BG(0x3), 0x20);
// }

extern struct Font gBanimFont;
extern s16 gBanimValid[2];
extern struct Text gBanimText[20];

extern const void * poinImg_EfxLeftNameBox;
static const void * sImg_EfxLeftNameBox[] = {

    Img_EfxLeftNameBox, // vanilla
    Img_EfxLeftNameBox_Sokaballa,
    Img_EfxLeftNameBox_Gamma,
    Img_EfxLeftNameBox_Fenreir,
};

const u8 * GetImg_EfxLeftNameBox()
{
    int id = GetUI_id();
    if (!id)
    {
        return poinImg_EfxLeftNameBox;
    }
    return sImg_EfxLeftNameBox[id];
}
extern const void * poinImg_EfxLeftItemBox;
static const void * sImg_EfxLeftItemBox[] = {

    Img_EfxLeftItemBox, // vanilla
    Img_EfxLeftItemBox_Sokaballa,
    Img_EfxLeftItemBox_Gamma,
    Img_EfxLeftItemBox_Fenreir,
};

const u8 * GetImg_EfxLeftItemBox()
{
    int id = GetUI_id();
    if (!id)
    {
        return poinImg_EfxLeftItemBox;
    }
    return sImg_EfxLeftItemBox[id];
}

extern const void * poinImg_EfxRightNameBox;
static const void * sImg_EfxRightNameBox[] = {

    Img_EfxRightNameBox, // vanilla
    Img_EfxRightNameBox_Sokaballa,
    Img_EfxRightNameBox_Gamma,
    Img_EfxRightNameBox_Fenreir,
};

const u8 * GetImg_EfxRightNameBox()
{
    int id = GetUI_id();
    if (!id)
    {
        return poinImg_EfxRightNameBox;
    }
    return sImg_EfxRightNameBox[id];
}

extern const void * poinImg_EfxRightItemBox;
static const void * sImg_EfxRightItemBox[] = {

    Img_EfxRightItemBox, // vanilla
    Img_EfxRightItemBox_Sokaballa,
    Img_EfxRightItemBox_Gamma,
    Img_EfxRightItemBox_Fenreir,
};

const u8 * GetImg_EfxRightItemBox()
{
    int id = GetUI_id();
    if (!id)
    {
        return poinImg_EfxRightItemBox;
    }
    return sImg_EfxRightItemBox[id];
}

void EfxPrepareScreenFx(void)
{
    const char * str;

    ApplyPalette(Pal_Text, 2);
    ApplyPalette(Pal_Text, 3);
    InitTextFont(&gBanimFont, (void *)0x6001880, 0xC4, 2);
    SetTextDrawNoClear();
    LZ77UnCompVram(GetImg_08801C14(), (void *)0x6001000);

    /* left unit name */
    if (gBanimValid[EKR_POS_L] == false)
        str = gNopStr;
    else
        str = GetStringFromIndex(gpEkrBattleUnitLeft->unit.pCharacterData->nameTextId);

    InitText(&gBanimText[0], 7);
    Text_SetCursor(&gBanimText[0], GetStringTextCenteredPos(0x38, str));
    LZ77UnCompVram(GetImg_EfxLeftNameBox(), (void *)0x6001880);
    Text_DrawString(&gBanimText[0], str);

    /* left unit item */
    if (gBanimValid[EKR_POS_L] == false)
        str = gNopStr;
    else
        str = GetItemName(gpEkrBattleUnitLeft->weaponBefore);

    InitText(&gBanimText[2], 8);
    Text_SetCursor(&gBanimText[2], GetStringTextCenteredPos(0x40, str));
    LZ77UnCompVram(GetImg_EfxLeftItemBox(), (void *)0x6001A40);
    Text_DrawString(&gBanimText[2], str);

    /* right unit name */
    if (gBanimValid[EKR_POS_R] == false)
        str = gNopStr;
    else
        str = GetStringFromIndex(gpEkrBattleUnitRight->unit.pCharacterData->nameTextId);

    InitText(&gBanimText[3], 7);
    Text_SetCursor(&gBanimText[3], GetStringTextCenteredPos(0x38, str));
    LZ77UnCompVram(GetImg_EfxRightNameBox(), (void *)0x6001C40);
    Text_DrawString(&gBanimText[3], str);

    /* right unit item */
    if (gBanimValid[EKR_POS_R] == false)
        str = gNopStr;
    else
        str = GetItemName(gpEkrBattleUnitRight->weaponBefore);

    InitText(&gBanimText[1], 8);
    Text_SetCursor(&gBanimText[1], GetStringTextCenteredPos(0x3E, str));
    LZ77UnCompVram(GetImg_EfxRightItemBox(), (void *)0x6001E00);
    Text_DrawString(&gBanimText[1], str);

    BG_Fill(gBG0TilemapBuffer, 0x80);
    EfxTmCpyBG(GetUnknown_08802508(), gBG0TilemapBuffer + 0x1E, 2, 20, -1, -1);
    sub_8070D04(gBG0TilemapBuffer + 0x1F, 1, 20, 2, 128);
    sub_8070D04(gBG0TilemapBuffer + 0x1E, 1, 20, 3, 128);
    BG_EnableSyncByMask(BG0_SYNC_BIT);

    CpuFastCopy(&PAL_BUF_COLOR(GetBPal_8802558(), gBanimFactionPal[POS_L], 0), PAL_BG(0x2), 0x20);
    CpuFastCopy(&PAL_BUF_COLOR(GetBPal_8802558(), gBanimFactionPal[POS_R], 0), PAL_BG(0x3), 0x20);
    EnablePaletteSync();

    gEkrBg0QuakeVec.x = 0;
    gEkrBg0QuakeVec.y = 0;
    BG_SetPosition(BG_0, 0, 0);
}

// extern const u16 gPal_PlayerInterface_Blue[];

// UI palette
extern const u16 gUiFramePaletteA[];
extern const u16 MenuTilesPalette_Gamma[];
extern const u16 MenuTilesPalette_Pikmin[];

static const u16 * const sUiPalLookupVanilla[] = {
    &gUiFramePaletteA[0x0], // gUiFramePaletteA has all 4 in a row anyway
    &gUiFramePaletteA[0x10], &gUiFramePaletteA[0x20], &gUiFramePaletteA[0x30], &gUiFramePaletteA[0x40],
    &gUiFramePaletteA[0x50], &gUiFramePaletteA[0x60], &gUiFramePaletteA[0x70],

};

static const u16 * const sUiPalLookupGamma[] = {
    &MenuTilesPalette_Gamma[0x0],  &MenuTilesPalette_Gamma[0x10], &MenuTilesPalette_Gamma[0x20],
    &MenuTilesPalette_Gamma[0x30], &MenuTilesPalette_Gamma[0x40], &MenuTilesPalette_Gamma[0x50],
    &MenuTilesPalette_Gamma[0x60], &MenuTilesPalette_Gamma[0x70],

};
static const u16 * const sUiPalLookupPikmin[] = {
    &MenuTilesPalette_Pikmin[0x0],  &MenuTilesPalette_Pikmin[0x10], &MenuTilesPalette_Pikmin[0x20],
    &MenuTilesPalette_Pikmin[0x30], &MenuTilesPalette_Pikmin[0x40], &MenuTilesPalette_Pikmin[0x50],
    &MenuTilesPalette_Pikmin[0x60], &MenuTilesPalette_Pikmin[0x70],

};

static const u16 * const sFactionPalLookupVanilla[] = {
    &gUiFramePaletteA[0x00],
    &gUiFramePaletteA[0x20],
    &gUiFramePaletteA[0x10],
    &gUiFramePaletteA[0x30],
};

static const u16 * const sFactionPalLookupGamma[] = {
    &MenuTilesPalette_Gamma[0x10],
    &MenuTilesPalette_Gamma[0x20],
    &MenuTilesPalette_Gamma[0x0],
    &MenuTilesPalette_Gamma[0x30],
};
static const u16 * const sFactionPalLookupPikmin[] = {
    &MenuTilesPalette_Pikmin[0x10],
    &MenuTilesPalette_Pikmin[0x20],
    &MenuTilesPalette_Pikmin[0x0],
    &MenuTilesPalette_Pikmin[0x30],
};

static const u16 * const * const sUiPalLookup[] = {
    sUiPalLookupVanilla,
    sUiPalLookupVanilla,
    sUiPalLookupGamma,
    sUiPalLookupPikmin,
};
static const u16 * const * const sFactionPalLookup[] = {
    sFactionPalLookupVanilla,
    sFactionPalLookupVanilla,
    sFactionPalLookupGamma,
    sFactionPalLookupPikmin,
};

#define BGPAL_WINDOW_FRAME 1
// sub_804D80C - link arena?
// gGfx_SupportMenu
// gPal_SupportMenu

// StartLinkArenaMainMenu - hook this and have it restore UI palette to normal, or similar

// 0x80481e0

void UnpackUiFramePalette(int palId) // used in link arena
{
    int id = GetUIPalID();
    if (palId < 0)
        palId = BGPAL_WINDOW_FRAME;

    // ApplyPalette(sUiFramePaletteLookup[gPlaySt.config.windowColor], palId);
    ApplyPalette(sUiPalLookup[id][gPlaySt.config.windowColor], palId);
    // ApplyPalette(&MenuTilesPalette_Gamma[gPlaySt.config.windowColor * 0x10], palId);
}
extern void ApplyUnitSpritePalettes(void);
// 4C374
void RestoreLinkArenaUI(void)
{
    // asm("mov r11, r11");
    gLCDControlBuffer.bg0cnt.priority = 0;
    gLCDControlBuffer.bg1cnt.priority = 1;
    gLCDControlBuffer.bg2cnt.priority = 2;
    gLCDControlBuffer.bg3cnt.priority = 3;

    ReadGameSaveCoreGfx();

    Decompress(sUiFrame[0], BG_CHAR_ADDR(0));
    ApplyPalette(sUiPalLookup[0][gPlaySt.config.windowColor], BGPAL_WINDOW_FRAME);
    // InitTextFont(&Font_0203DB64, (void *)0x06001800, 0xc0, 0);
}

// obj pal 2.. not this
void PrepItemScreen_SetupGfx_Hook(void)
{

    // ApplyPalette(gUiFramePaletteD, 2);
    // int id = GetUIPalID();
    // const u16 * pal = sUiPalLookup[id][3];
    // ApplyPalette(pal, 2);
    LoadObjUIGfx();
}

void PrepUiPalHook_2(void) // item use screen
{
    int id = GetUIPalID();
    const u16 * pal = sUiPalLookup[id][3]; // normally gUiFramePaletteD
    ApplyPalette(pal, 0x2);
}

void PrepUiPalHook_3(void) // main prep palette
{
    RestartMuralBackground();
    int id = GetUIPalID();
    const u16 * pal = sUiPalLookup[id][1]; // normally gUiFramePaletteB
    ApplyPalette(pal, 0x2);
    pal = sUiPalLookup[id][2]; // normally gUiFramePaletteC
    ApplyPalette(pal, 0x3);
    pal = sUiPalLookup[id][3]; // normally gUiFramePaletteD
    ApplyPalette(pal, 0x4);
}

void DrawUnitInfoBg_Init(void) // in prep - [id][0] was causing palette problems
{
    // int id = GetUIPalID();
    const u16 * pal = sUiPalLookup[0][0]; // normally gUiFramePaletteA
    ApplyPalette(pal, 0x12);
    return;
}

void StatusScreenUIPal_Hook(void)
{
    ClearBg0Bg1();

    int id = GetUIPalID();

    ApplyPalette(sFactionPalLookup[id][0], 2);
    ApplyPalette(sFactionPalLookup[id][2], 3);
    ApplyPalette(sFactionPalLookup[id][1], 4);

    // const u16 * pal = sFactionPalLookup[id][0];
    // ApplyPalettes(pal, 2, 3);
}

void GuideUIPal_Hook(void)
{
    int id = GetUIPalID();
    const u16 * pal = sUiPalLookup[id][gPlaySt.config.windowColor + 4];
    ApplyPalette(pal, 2);
}

extern u16 Pal_SysBrownBox[];
const u8 Pal_SysBrownBox_Pikmin[] = {
    0x0E, 0x52, 0xFF, 0x7F, 0x9E, 0x73, 0xDB, 0x5A, 0x2E, 0x7F, 0x7F, 0x13, 0x51, 0x05, 0x1F, 0x1E,
    0x78, 0x00, 0x2E, 0x7F, 0xE5, 0x4D, 0xCF, 0x4B, 0xE3, 0x0A, 0x30, 0x4E, 0x1F, 0x7C, 0x00, 0x00,
    0x0E, 0x52, 0xDE, 0x7B, 0x5B, 0x63, 0xB7, 0x4A, 0x14, 0x32, 0x4F, 0x19, 0xEA, 0x10, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
void Pal_SysBrownBox_Hook()
{
    if (GetUIPalID() == 3)
    {
        ApplyPalettes(Pal_SysBrownBox_Pikmin, 0x19, 2);
    }
    else
    {
        ApplyPalettes(Pal_SysBrownBox, 0x19, 2);
    }
}

void GetMinimugFactionPalette(int faction, int palId)
{
    int id = GetUIPalID();
    const u16 * pal = sFactionPalLookup[id][faction >> 6]; // 0x40 -> 0x10
    ApplyPalette(pal, palId);
}

const u16 * GetFactionBattleForecastFramePalette(int faction)
{
    int id = GetUIPalID();
    const u16 * pal = sFactionPalLookup[id][faction >> 6]; // 0x40 -> 0x10
    return pal;
}

void sub_80BE5B4(int faction, int palId) // world map
{
    int id = GetUIPalID();
    if (faction < 0)
    {
        faction = 0xC0; // does some grey palette on world map
    }
    const u16 * pal = sFactionPalLookup[id][faction >> 6]; // 0x40 -> 0x10

    ApplyPalette(pal, palId);
    // u16 * src;

    // switch (faction)
    // {
    // case FACTION_BLUE:
    // src = gUnknown_08A98E2C;
    // break;

    // case FACTION_RED:
    // src = gUnknown_08A98E4C;
    // break;

    // case FACTION_GREEN:
    // src = gUnknown_08A98E6C;
    // break;

    // default:
    // src = gUnknown_08A98E8C;
    // break;
    // }

    // ApplyPalette(src, palId);

    return;
}

// struct ProcShop
// {
//     /* 00 */ PROC_HEADER;

//     /* 2C */ struct Unit * unit;
//     /* 30 */ u16 shopItems[20];

//     /* 58 */ u16 unk_58;

//     /* 5A */ u8 shopItemCount;
//     /* 5B */ u8 unitItemCount;
//     /* 5C */ u8 head_loc;
//     /* 5D */ u8 hand_loc;
//     /* 5E */ u8 head_idx;
//     /* 5F */ u8 hand_idx; // maybe top visible item in menu?
//     /* 60 */ u8 buy_or_sel;
//     /* 61 */ u8 shopType;
//     /* 62 */ u8 helpTextActive;

//     /* 64 */ s16 goldbox_x;
//     /* 66 */ s16 goldbox_y;
//     /* 68 */ s16 goldbox_oam2;
// };

extern void InitGoldBoxText(u16 *);
extern const void * poinImg_ShopGoldBox;
static const void * sUiGoldBox[] = {

    Img_ShopGoldBox, // vanilla
    Img_ShopGoldBox_Gamma,
    Img_ShopGoldBox_Gamma,
    Img_ShopGoldBox_Gamma,
};

const u8 * GetUiGoldBox(void)
{
    int id = GetUI_id();
    return sUiGoldBox[id];
}

void StartUiGoldBox(ProcPtr parent)
{
    struct ProcShop * proc;

    Decompress(GetUiGoldBox(), OBJ_CHR_ADDR(OBJCHR_SHOP_GOLDBOX));

    proc = Proc_Start(gProcScr_GoldBox, parent);
    proc->goldbox_x = 0xAC;
    proc->goldbox_y = 0x2D;
    proc->goldbox_oam2 = OBJ_PALETTE(OBJPAL_SHOP_GOLDBOX) + OBJ_CHAR(OBJCHR_SHOP_GOLDBOX);

    int id = GetUIPalID();
    const u16 * pal = sUiPalLookup[id][0];
    ApplyPalette(pal, 0x10 + OBJPAL_SHOP_GOLDBOX);

    // ApplyPalette(gUiFramePaletteA, 0x10 + OBJPAL_SHOP_GOLDBOX);
    InitGoldBoxText(TILEMAP_LOCATED(gBG0TilemapBuffer, 28, 6));
    DisplayGoldBoxText(TILEMAP_LOCATED(gBG0TilemapBuffer, 27, 6));
}

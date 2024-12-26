// #include "global.h"

// #include "eventinfo.h"
// #include "bmshop.h"
// #include "fontgrp.h"
// #include "ctc.h"
// #include "hardware.h"
// #include "uiutils.h"
// #include "bmlib.h"
// #include "m4a.h"
// #include "soundwrapper.h"
// #include "bmio.h"
// #include "bm.h"

#include "common-chax.h"
#include "bmguide.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

// #include "proc.h"

struct GuideEnt gGuideTable[] = {
    {
        .title = 0x1,
        .itemName = 0x5D5,
        .details = 0x60D,
        .displayFlag = 0xB4,
        .readFlag = 0xF0,
    },
    {
        .title = 0x1,
        .itemName = 0x5D6,
        .details = 0x60E,
        .displayFlag = 0xB5,
        .readFlag = 0xF1,
    },
    {
        .title = 0x1,
        .chapterTitle = 0x1,
        .itemName = 0x5D7,
        .details = 0x60F,
        .displayFlag = 0xB6,
        .readFlag = 0xF2,
    },
    {
        .title = 0x2,
        .itemName = 0x5D8,
        .details = 0x610,
        .displayFlag = 0xB7,
        .readFlag = 0xF3,
    },
    {
        .title = 0x2,
        .chapterTitle = 0x2,
        .itemName = 0x5D9,
        .details = 0x611,
        .displayFlag = 0xB8,
        .readFlag = 0xF4,
    },
    {
        .title = 0x3,
        .itemName = 0x5DA,
        .details = 0x612,
        .displayFlag = 0xB9,
        .readFlag = 0xF5,
    },
    {
        .title = 0x3,
        .chapterTitle = 0x1,
        .itemName = 0x5DB,
        .details = 0x613,
        .displayFlag = 0xBA,
        .readFlag = 0xF6,
    },
    {
        .title = 0x3,
        .chapterTitle = 0x4,
        .itemName = 0x5DC,
        .details = 0x614,
        .displayFlag = 0xBB,
        .readFlag = 0xF7,
    },
    {
        .title = 0x3,
        .chapterTitle = 0x2,
        .itemName = 0x5DD,
        .details = 0x615,
        .displayFlag = 0xBC,
        .readFlag = 0xF8,
    },
    {
        .title = 0x3,
        .chapterTitle = 0x5,
        .itemName = 0x5DE,
        .details = 0x616,
        .displayFlag = 0xBD,
        .readFlag = 0xF9,
    },
    {
        .title = 0x3,
        .chapterTitle = 0x4,
        .itemName = 0x5DF,
        .details = 0x617,
        .displayFlag = 0xBE,
        .readFlag = 0xFA,
    },
    {
        .title = 0x3,
        .chapterTitle = 0x4,
        .itemName = 0x5E0,
        .details = 0x618,
        .displayFlag = 0xBF,
        .readFlag = 0xFB,
    },
    {
        .title = 0x3,
        .chapterTitle = 0x2,
        .itemName = 0x5E1,
        .details = 0x619,
        .displayFlag = 0xC0,
        .readFlag = 0xFC,
    },
    {
        .title = 0x3,
        .chapterTitle = 0x6,
        .itemName = 0x5E2,
        .details = 0x61A,
        .displayFlag = 0xC1,
        .readFlag = 0xFD,
    },
    {
        .title = 0x3,
        .itemName = 0x5E3,
        .details = 0x61B,
        .displayFlag = 0xC2,
        .readFlag = 0xFE,
    },
    {
        .title = 0x3,
        .itemName = 0x5E4,
        .details = 0x61C,
        .displayFlag = 0xC3,
        .readFlag = 0xFF,
    },
    {
        .title = 0x4,
        .chapterTitle = 0x2,
        .itemName = 0x5E5,
        .details = 0x61D,
        .displayFlag = 0xC4,
        .readFlag = 0x100,
    },
    {
        .title = 0x4,
        .chapterTitle = 0x2,
        .itemName = 0x5E6,
        .details = 0x61E,
        .displayFlag = 0xC5,
        .readFlag = 0x101,
    },
    {
        .title = 0x5,
        .chapterTitle = 0x3,
        .itemName = 0x5E7,
        .details = 0x61F,
        .displayFlag = 0xC6,
        .readFlag = 0x102,
    },
    {
        .title = 0x5,
        .chapterTitle = 0x1,
        .itemName = 0x5E8,
        .details = 0x620,
        .displayFlag = 0xC7,
        .readFlag = 0x103,
    },
    {
        .title = 0x5,
        .chapterTitle = 0x1,
        .itemName = 0x5E9,
        .details = 0x621,
        .displayFlag = 0xC8,
        .readFlag = 0x104,
    },
    {
        .title = 0x5,
        .itemName = 0x5EA,
        .details = 0x622,
        .displayFlag = 0xC9,
        .readFlag = 0x105,
    },
    {
        .title = 0x5,
        .chapterTitle = 0x2,
        .itemName = 0x5EB,
        .details = 0x623,
        .displayFlag = 0xCA,
        .readFlag = 0x106,
    },
    {
        .title = 0x5,
        .chapterTitle = 0x2,
        .itemName = 0x5EC,
        .details = 0x624,
        .displayFlag = 0xCB,
        .readFlag = 0x107,
    },
    {
        .title = 0x5,
        .chapterTitle = 0x5,
        .itemName = 0x5ED,
        .details = 0x625,
        .displayFlag = 0xCC,
        .readFlag = 0x108,
    },
    {
        .title = 0x6,
        .chapterTitle = 0x4,
        .itemName = 0x5EE,
        .details = 0x626,
        .displayFlag = 0xCD,
        .readFlag = 0x109,
    },
    {
        .title = 0x6,
        .chapterTitle = 0x1,
        .itemName = 0x5EF,
        .details = 0x627,
        .displayFlag = 0xCE,
        .readFlag = 0x10A,
    },
    {
        .title = 0x6,
        .chapterTitle = 0x1,
        .itemName = 0x5F0,
        .details = 0x628,
        .displayFlag = 0xCF,
        .readFlag = 0x10B,
    },
    {
        .title = 0x6,
        .chapterTitle = 0x2,
        .itemName = 0x5F1,
        .details = 0x629,
        .displayFlag = 0xD0,
        .readFlag = 0x10C,
    },
    {
        .title = 0x6,
        .chapterTitle = 0x3,
        .itemName = 0x5F2,
        .details = 0x62A,
        .displayFlag = 0xD1,
        .readFlag = 0x10D,
    },
    {
        .title = 0x6,
        .chapterTitle = 0x4,
        .itemName = 0x5F3,
        .details = 0x62B,
        .displayFlag = 0xD2,
        .readFlag = 0x10E,
    },
    {
        .title = 0x6,
        .chapterTitle = 0x3,
        .itemName = 0x5F4,
        .details = 0x62C,
        .displayFlag = 0xD3,
        .readFlag = 0x10F,
    },
    {
        .title = 0x6,
        .chapterTitle = 0x6,
        .itemName = 0x5F5,
        .details = 0x62D,
        .displayFlag = 0xD4,
        .readFlag = 0x110,
    },
    {
        .title = 0x6,
        .chapterTitle = 0x7,
        .itemName = 0x5F6,
        .details = 0x62E,
        .displayFlag = 0xD5,
        .readFlag = 0x111,
    },
    {
        .title = 0x7,
        .chapterTitle = 0x1,
        .itemName = 0x5F7,
        .details = 0x62F,
        .displayFlag = 0xD6,
        .readFlag = 0x112,
    },
    {
        .title = 0x7,
        .chapterTitle = 0x1,
        .itemName = 0x5F8,
        .details = 0x630,
        .displayFlag = 0xD7,
        .readFlag = 0x113,
    },
    {
        .title = 0x7,
        .chapterTitle = 0x3,
        .itemName = 0x5F9,
        .details = 0x631,
        .displayFlag = 0xD8,
        .readFlag = 0x114,
    },
    {
        .title = 0x7,
        .chapterTitle = 0x3,
        .itemName = 0x5FA,
        .details = 0x632,
        .displayFlag = 0xD9,
        .readFlag = 0x115,
    },
    {
        .title = 0x7,
        .chapterTitle = 0x2,
        .itemName = 0x5FB,
        .details = 0x633,
        .displayFlag = 0xDA,
        .readFlag = 0x116,
    },
    {
        .title = 0x7,
        .chapterTitle = 0x5,
        .itemName = 0x5FC,
        .details = 0x634,
        .displayFlag = 0xDB,
        .readFlag = 0x117,
    },
    {
        .title = 0x8,
        .itemName = 0x5FD,
        .details = 0x635,
        .displayFlag = 0xDC,
        .readFlag = 0x118,
    },
    {
        .title = 0x8,
        .chapterTitle = 0x1,
        .itemName = 0x5FE,
        .details = 0x636,
        .displayFlag = 0xDD,
        .readFlag = 0x119,
    },
    {
        .title = 0x8,
        .chapterTitle = 0x2,
        .itemName = 0x5FF,
        .details = 0x637,
        .displayFlag = 0xDE,
        .readFlag = 0x11A,
    },
    {
        .title = 0x8,
        .chapterTitle = 0x8,
        .itemName = 0x600,
        .details = 0x638,
        .displayFlag = 0xDF,
        .readFlag = 0x11B,
    },
    {
        .title = 0x9,
        .itemName = 0x601,
        .details = 0x639,
        .displayFlag = 0xE0,
        .readFlag = 0x11C,
    },
    {
        .title = 0x9,
        .itemName = 0x602,
        .details = 0x63A,
        .displayFlag = 0xE1,
        .readFlag = 0x11D,
    },
    {
        .title = 0xA,
        .chapterTitle = 0x1,
        .itemName = 0x603,
        .details = 0x63B,
        .displayFlag = 0xE2,
        .readFlag = 0x11E,
    },
    {
        .title = 0xA,
        .chapterTitle = 0x2,
        .itemName = 0x604,
        .details = 0x63C,
        .displayFlag = 0xE3,
        .readFlag = 0x11F,
    },
    {
        .title = 0xA,
        .chapterTitle = 0x2,
        .itemName = 0x605,
        .details = 0x63D,
        .displayFlag = 0xE4,
        .readFlag = 0x120,
    },
    {
        .title = 0xA,
        .chapterTitle = 0x8,
        .itemName = 0x606,
        .details = 0x63E,
        .displayFlag = 0xE5,
        .readFlag = 0x121,
    },
    {
        .title = 0xB,
        .chapterTitle = 0x4,
        .itemName = 0x607,
        .details = 0x63F,
        .displayFlag = 0xE6,
        .readFlag = 0x122,
    },
    {
        .title = 0xB,
        .itemName = 0x608, /* Leveling up */
        .details = 0x640,
        .displayFlag = 0xE7,
        .readFlag = 0x123,
    },
    {
        .title = 0xB,
        .chapterTitle = 0x2,
        .itemName = 0x609, /* Changing Classes */
        .details = 0x641,
        .displayFlag = 0xE8,
        .readFlag = 0x124,
    },
    {
        .title = 0xB,
        .chapterTitle = 0x3,
        .itemName = 0x60A,
        .details = 0x642,
        .displayFlag = 0xE9,
        .readFlag = 0x125,
    },
    {
        .title = 0xB,
        .chapterTitle = 0x5,
        .itemName = 0x60B, /* Arena */
        .details = 0x643,
        .displayFlag = 0xEA,
        .readFlag = 0x126,
    },
    {
        .title = 0xB,
        .chapterTitle = 0x8,
        .itemName = 0x60C,     /* Retreat */
        .details = 0x644,
        .displayFlag = 0xEB,
        .readFlag = 0x127,
    },
    {
        .title = 0xC,
    },
};

struct GuideSt * const gGuideSt_CSS = (void *)gGenericBuffer;

// clang-format off

u16 const gSprite_GuideBannerText_CSS[] =
{
    4,
    OAM0_SHAPE_32x8, OAM1_SIZE_32x8, OAM2_CHR(0xC0),
    OAM0_SHAPE_32x8, OAM1_SIZE_32x8 + OAM1_X(32), OAM2_CHR(0xC4),
    OAM0_SHAPE_32x8 + OAM0_Y(8), OAM1_SIZE_32x8, OAM2_CHR(0xC8),
    OAM0_SHAPE_32x8 + OAM0_Y(8), OAM1_SIZE_32x8 + OAM1_X(32), OAM2_CHR(0xCC),
};

u16 const gSprite_SelectButtonSort_CSS[] =
{
    2,
    OAM0_SHAPE_32x16, OAM1_SIZE_32x16, OAM2_CHR(0x92),
    OAM0_SHAPE_32x16, OAM1_SIZE_32x16 + OAM1_X(32), OAM2_CHR(0x98),
};

u16 const gSprite_BButtonBack_CSS[] =
{
    2,
    OAM0_SHAPE_16x16, OAM1_SIZE_16x16 + OAM1_X(16), OAM2_CHR(0x96),
    OAM0_SHAPE_32x16, OAM1_SIZE_32x16 + OAM1_X(32), OAM2_CHR(0x9C),
};

int const gTextIds_GuideCategoriesChapter_CSS[] =
{
    0x05C0, // TODO: msgid "Prologue"
    0x05C1, // TODO: msgid "Chapter 1"
    0x05C2, // TODO: msgid "Chapter 2"
    0x05C3, // TODO: msgid "Chapter 3"
    0x05C4, // TODO: msgid "Chapter 4"
    0x05C5, // TODO: msgid "Chapter 5"
    0x05C6, // TODO: msgid "Chapter 6"
    0x05C7, // TODO: msgid "Chapter 7"
    0x05C8, // TODO: msgid "Chapter 8"
};

u16 const gTextIds_GuideCategoriesTopic_CSS[] =
{
    0x0000,
    0x05C9, // TODO: msgid "Basic Rules"
    0x05CA, // TODO: msgid "Movement"
    0x05CB, // TODO: msgid "Combat"
    0x05CC, // TODO: msgid "Rescue"
    0x05CD, // TODO: msgid "Items"
    0x05CE, // TODO: msgid "Terrain"
    0x05CF, // TODO: msgid "Allies"
    0x05D0, // TODO: msgid "Victory"
    0x05D1, // TODO: msgid "Save"
    0x05D2, // TODO: msgid "World Map"
    0x05D3, // TODO: msgid "Other"
    GUIDE_TITLE_SKILL,
};

// clang-format on

extern u8 Tsa_08B176CC[]; // tsa
extern u8 Img_08B177C0[]; // gfx
extern u8 Img_08B17864[]; // gfx
extern u16 Pal_08B17B44[]; // pal

// TODO: Implicit declarations
void UpdateMenuScrollBarConfig(u8, u16, u16, u8);
bool IsGuideLocked(void);
ProcPtr StartMenuScrollBarExt(ProcPtr, int, int, int, int);
void sub_8097668(void);
void LockMenuScrollBar(void);
void EndMenuScrollBar(void);
void GuideSpriteDraw_Init(void);
void GuideSpriteDraw_Loop(void);
void PutGuideBottomBarText(void);
void sub_80CE1C0(int strIndex, int textIndex, int y);
void sub_80CE248(void);
void sub_80CE28C(void);
void GuideMenuRefresh_SyncBg1(void);
void GuideMenuRefresh_SyncBg0Bg1(void);
void GuideEntry_RedrawUp(struct GuideProc * pr);
void GuideEntry_RedrawDown(struct GuideProc * proc);
void sub_80CE414(void);
void GuideEntry_DrawInitial(void);
const char * GetStringNextLine(const char * str);
void MoveGuideDetailText(int idx, int moveDirection);
void sub_80CE750(ProcPtr proc, int b);
void sub_80CE858(ProcPtr proc, int b);
void GuideDetailsRedraw_Init(struct GuideProc * proc);
void GuideDetailsRedraw_Loop(struct GuideProc * proc);
void sub_80CEAE8(void);
void sub_80CEBA4(void);
void sub_80CEC68(u16 off);
void Guide_Init(ProcPtr proc);
void Guide_SetBlend(void);
int GetGuideAction(ProcPtr proc);
void Guide_MainLoop(struct GuideProc * proc);
void Guide_OnEnd(void);
void BmGuideTextSetAllGreen(void);
bool BmGuideTextShowGreenOrNormal(void);
s8 WorldMap_CallGuide(ProcPtr proc);
u8 MapMenu_GuideCommand(struct MenuProc* menu, struct MenuItemProc* menuItem);


//! FE8U = 0x080CDF4C
LYN_REPLACE_CHECK(IsGuideLocked);
bool IsGuideLocked(void)
{
    return FALSE;

    struct GuideEnt * it = gGuideTable;

    while (1)
    {
        if (it->title == 12)
        {
            return TRUE;
        }

        if (CheckFlag(it->displayFlag))
        {
            return FALSE;
        }

        it++;
    }
}

//! FE8U = 0x080CDF78
LYN_REPLACE_CHECK(GuideSpriteDraw_Init);
void GuideSpriteDraw_Init(void)
{
    UnpackUiVArrowGfx(0xe0, 3);
    return;
}

//! FE8U = 0x080CDF88
LYN_REPLACE_CHECK(GuideSpriteDraw_Loop);
void GuideSpriteDraw_Loop(void)
{
    int y1;
    int y2;

    GetGameClock();

    PutSprite(3, 16, 8, gSprite_GuideBannerText_CSS, OAM2_PAL(2));

    if (gGuideSt_CSS->state == GUIDE_STATE_0)
    {
        PutSprite(3, 176, 3, gSprite_SelectButtonSort_CSS, OAM2_PAL(2));
    }

    PutSprite(3, 176, 15, gSprite_BButtonBack_CSS, OAM2_PAL(2));

    y1 = (gGuideSt_CSS->categoryIdx - gGuideSt_CSS->unk_2a) * 2 + 5;
    y2 = (gGuideSt_CSS->unk_2b - gGuideSt_CSS->unk_2c) * 2 + 5;

    switch (gGuideSt_CSS->state)
    {
        case GUIDE_STATE_0:
            DisplayUiHand(12, y1 * 8);

            if ((gGuideSt_CSS->sortMode != GUIDE_SORT_MODE_TOPIC ? gGuideSt_CSS->unk_3c : gGuideSt_CSS->unk_3d) > 6)
            {
                if (gGuideSt_CSS->unk_2a != 0)
                {
                    DisplayUiVArrow(32, 32, OAM2_CHR(0xE0) + OAM2_PAL(3), 1);
                }

                if (gGuideSt_CSS->unk_2a <
                    (gGuideSt_CSS->sortMode != GUIDE_SORT_MODE_TOPIC ? gGuideSt_CSS->unk_3c : gGuideSt_CSS->unk_3d) - 6)
                {
                    DisplayUiVArrow(32, 136, OAM2_CHR(0xE0) + OAM2_PAL(3), 0);
                }
            }

            break;

        case GUIDE_STATE_1:
            DisplayFrozenUiHand(12, y1 * 8);
            DisplayUiHand(80, y2 * 8);

            break;

        case GUIDE_STATE_2:
            DisplayFrozenUiHand(12, y1 * 8);

            if (gGuideSt_CSS->numDetailLines > 4)
            {
                if ((gGuideSt_CSS->detailLinesScrolled) != 0)
                {
                    DisplayUiVArrow(144, 56, OAM2_CHR(0xE0) + OAM2_PAL(3), 1);
                }

                if (gGuideSt_CSS->detailLinesScrolled < gGuideSt_CSS->numDetailLines - 4)
                {
                    DisplayUiVArrow(144, 128, OAM2_CHR(0xE0) + OAM2_PAL(3), 0);
                }
            }

            break;
    }

    UpdateMenuScrollBarConfig(10, gGuideSt_CSS->unk_2c * 16, gGuideSt_CSS->unk_3e, 6);

    return;
}

//! FE8U = 0x080CE148
LYN_REPLACE_CHECK(PutGuideBottomBarText);
void PutGuideBottomBarText(void)
{
    if (gGuideSt_CSS->sortMode != GUIDE_SORT_MODE_TOPIC)
    {
        return;
    }

    ClearText(&gGuideSt_CSS->unk_ec);
    PutDrawText(
        &gGuideSt_CSS->unk_ec, TILEMAP_LOCATED(gBG0TilemapBuffer, 4, 18), TEXT_COLOR_SYSTEM_WHITE, 0, 22,
        GetStringFromIndex(0x05D4)); // TODO: msgid "About"

    Text_DrawString(
        &gGuideSt_CSS->unk_ec, GetStringFromIndex(gTextIds_GuideCategoriesTopic_CSS[gGuideSt_CSS->unk_54[gGuideSt_CSS->categoryIdx]]));

    return;
}

//! FE8U = 0x080CE1C0
LYN_REPLACE_CHECK(sub_80CE1C0);
void sub_80CE1C0(int strIndex, int textIndex, int y)
{
    const char * str;

    ClearText(&gGuideSt_CSS->unk_7c[textIndex]);

    str = (gGuideSt_CSS->sortMode != GUIDE_SORT_MODE_TOPIC)
        ? GetStringFromIndex(gTextIds_GuideCategoriesChapter_CSS[gGuideSt_CSS->unk_40[strIndex]])
        : GetStringFromIndex(gTextIds_GuideCategoriesTopic_CSS[gGuideSt_CSS->unk_54[strIndex]]);

    PutDrawText(
        &gGuideSt_CSS->unk_7c[textIndex], TILEMAP_LOCATED(gBG1TilemapBuffer, 2, y), TEXT_COLOR_SYSTEM_WHITE, 0, 9, str);
    return;
}

//! FE8U = 0x080CE248
LYN_REPLACE_CHECK(sub_80CE248);
void sub_80CE248(void)
{
    int i;

    int a = (gGuideSt_CSS->sortMode != GUIDE_SORT_MODE_TOPIC) ? gGuideSt_CSS->unk_3c : gGuideSt_CSS->unk_3d;

    for (i = 0; i < 6; i++)
    {
        if (i < a)
        {
            sub_80CE1C0(i, i, (i * 2) + 5);
        }
    }

    return;
}

//! FE8U = 0x080CE28C
LYN_REPLACE_CHECK(sub_80CE28C);
void sub_80CE28C(void)
{
    int iy;
    int ix;

    int yBase = 160;

    for (iy = 0; iy < 12; iy++)
    {
        for (ix = 0; ix < 28; ix++)
        {
            gBG1TilemapBuffer[(yBase + 1) + ix] = 0;
        }
        yBase += 0x20;
    }

    for (ix = 0; ix < 28; ix++)
    {
        gBG0TilemapBuffer[0x241 + ix + 0x00] = 0;
        gBG0TilemapBuffer[0x241 + ix + 0x20] = 0;
    }

    return;
}

//! FE8U = 0x080CE2E4
LYN_REPLACE_CHECK(GuideMenuRefresh_SyncBg1);
void GuideMenuRefresh_SyncBg1(void)
{
    BG_EnableSyncByMask(BG1_SYNC_BIT);
    return;
}

//! FE8U = 0x080CE2F0
LYN_REPLACE_CHECK(GuideMenuRefresh_SyncBg0Bg1);
void GuideMenuRefresh_SyncBg0Bg1(void)
{
    BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT);
    return;
}

//! FE8U = 0x080CE2FC
LYN_REPLACE_CHECK(GuideEntry_RedrawUp);
void GuideEntry_RedrawUp(struct GuideProc * proc)
{
    int idx = proc->unk_34;
    int textIdx = idx % 6;

    ClearText(&gGuideSt_CSS->unk_b4[textIdx]);

    PutDrawText(
        &gGuideSt_CSS->unk_b4[textIdx], TILEMAP_LOCATED(gBG1TilemapBuffer, 11, 5),
        CheckFlag(gGuideTable[gGuideSt_CSS->unk_68[idx]].readFlag) ? TEXT_COLOR_SYSTEM_WHITE : TEXT_COLOR_SYSTEM_GREEN, 0,
        18, GetStringFromIndex(gGuideTable[gGuideSt_CSS->unk_68[idx]].itemName));

    return;
}

//! FE8U = 0x080CE388
LYN_REPLACE_CHECK(GuideEntry_RedrawDown);
void GuideEntry_RedrawDown(struct GuideProc * proc)
{
    int idx = proc->unk_34;
    int textIdx = idx % 6;

    ClearText(&gGuideSt_CSS->unk_b4[textIdx]);

    PutDrawText(
        &gGuideSt_CSS->unk_b4[textIdx], TILEMAP_LOCATED(gBG1TilemapBuffer, 11, 15),
        CheckFlag(gGuideTable[gGuideSt_CSS->unk_68[idx]].readFlag) ? TEXT_COLOR_SYSTEM_WHITE : TEXT_COLOR_SYSTEM_GREEN, 0,
        18, GetStringFromIndex(gGuideTable[gGuideSt_CSS->unk_68[idx]].itemName));

    return;
}

//! FE8U = 0x080CE414
LYN_REPLACE_CHECK(sub_80CE414);
void sub_80CE414(void)
{
    int r6;

    register int r4 asm("r4");
    int r5;
    int r8;

    int y = 5;
    int idx = 0;

    for (r8 = 0, gGuideSt_CSS->unk_3e = 0; gGuideTable[r8].title != 12; r8++)
    {

        if (!CheckFlag(gGuideTable[r8].displayFlag))
        {
            continue;
        }

        if (gGuideSt_CSS->sortMode != GUIDE_SORT_MODE_TOPIC)
        {
            if (gGuideTable[r8].chapterTitle == gGuideSt_CSS->unk_40[gGuideSt_CSS->categoryIdx])
            {
                gGuideSt_CSS->unk_68[idx] = r8;
                idx++;
            }
        }
        else
        {
            if (gGuideTable[r8].title == gGuideSt_CSS->unk_54[gGuideSt_CSS->categoryIdx])
            {
                gGuideSt_CSS->unk_68[idx] = r8;
                idx++;
            }
        }
    }

    r6 = gGuideSt_CSS->unk_3e = idx;

    r5 = r4 = gGuideSt_CSS->unk_2c;

    for (r8 = 0; r8 <= 5 && r6 != 0; y += 2, r5++, r6--, r4++, r8++)
    {
        r5 = r5 % 6;

        ClearText(&gGuideSt_CSS->unk_b4[r5]);

        PutDrawText(
            &gGuideSt_CSS->unk_b4[r5], gBG1TilemapBuffer + TILEMAP_INDEX(11, y),
            (!CheckFlag(gGuideTable[gGuideSt_CSS->unk_68[r4]].readFlag)) ? TEXT_COLOR_SYSTEM_GREEN
                                                                     : TEXT_COLOR_SYSTEM_WHITE,
            0, 18, GetStringFromIndex(gGuideTable[gGuideSt_CSS->unk_68[r4]].itemName));
    }

    return;
}

//! FE8U = 0x080CE588
LYN_REPLACE_CHECK(GuideEntry_DrawInitial);
void GuideEntry_DrawInitial(void)
{
    int ix;
    int iy;

    int base = 160;

    for (iy = 0; iy < 12; iy++)
    {
        for (ix = 0; ix < 19; ix++)
        {
            int x = 10 + ix;

            gBG1TilemapBuffer[x + base] = 0;
        }

        base += 0x20;
    }

    return;
}

//! FE8U = 0x080CE5BC
LYN_REPLACE_CHECK(GetStringNextLine);
const char * GetStringNextLine(const char * str)
{
    if (str == NULL)
    {
        return NULL;
    }

    if (*str == 0) // [X]
    {
        return NULL;
    }

    while (1)
    {
        char c = *str;
        u32 width;

        if (c != 0)
        {
            if (c == 1) // [NL]
            {
                return str + 1;
            }
        }
        else
        {
            return NULL;
        }

        str = GetCharTextLen(str, &width);
    }
}

//! FE8U = 0x080CE5F0
LYN_REPLACE_CHECK(MoveGuideDetailText);
void MoveGuideDetailText(int idx, int moveDirection)
{
    int detailLinesScrolled;
    int i;

    int numDetailLines = 1;

    const char * str = GetStringFromIndex(gGuideTable[idx].details);
    while (1)
    {
        str = GetStringNextLine(str);
        if (str == NULL)
        {
            break;
        }

        numDetailLines++;
    }

    gGuideSt_CSS->numDetailLines = numDetailLines;

    detailLinesScrolled = gGuideSt_CSS->detailLinesScrolled;

    if (moveDirection != GUIDE_DETAILS_STAY)
    {
        if (numDetailLines > 4)
        {
            if (moveDirection == GUIDE_DETAILS_ADVANCE)
            {
                if (detailLinesScrolled + 4 <= numDetailLines - 4)
                {
                    detailLinesScrolled = detailLinesScrolled + 4;
                }
                else
                {
                    detailLinesScrolled = numDetailLines - 4;
                }
            }
            else
            {
                if (detailLinesScrolled - 4 >= 0)
                {
                    detailLinesScrolled = detailLinesScrolled - 4;
                }
                else
                {
                    detailLinesScrolled = 0;
                }
            }
        }

        if ((moveDirection != GUIDE_DETAILS_STAY) && (gGuideSt_CSS->detailLinesScrolled == detailLinesScrolled))
        {
            return;
        }
    }

    GuideEntry_DrawInitial();

    gGuideSt_CSS->detailLinesScrolled = detailLinesScrolled;

    ClearText(gGuideSt_CSS->unk_b4);

    PutDrawText(
        gGuideSt_CSS->unk_b4, TILEMAP_LOCATED(gBG1TilemapBuffer, 10, 5), TEXT_COLOR_SYSTEM_GOLD, 2, 18,
        GetStringFromIndex(gGuideTable[idx].itemName));

    str = GetStringFromIndex(gGuideTable[idx].details);

    for (i = 0; i < detailLinesScrolled + 4; i++)
    {

        if (i != 0)
        {
            str = GetStringNextLine(str);
            if (str == NULL)
            {
                break;
            }
        }

        if (i >= detailLinesScrolled)
        {
            int off;
            int textIndex = i % 5;

            ClearText(&gGuideSt_CSS->unk_b4[1 + textIndex]);

            PutDrawText(
                &gGuideSt_CSS->unk_b4[1 + textIndex],
                gBG1TilemapBuffer + 11 + ((((i - detailLinesScrolled) % 4) * 0x40) + (off = 0x100)),
                TEXT_COLOR_SYSTEM_WHITE, 0, 17, str);
        }
    }

    BG_EnableSyncByMask(BG1_SYNC_BIT);

    return;
}

extern const struct ProcCmd gProcScr_GuideEntryListRedraw_Up_CSS[];

//! FE8U = 0x080CE750
LYN_REPLACE_CHECK(sub_80CE750);
void sub_80CE750(ProcPtr proc, int b)
{
    struct GuideProc * child;
    int ix;
    int iy;
    register int hm asm("r9") = b;

    int off = 0x1a0;

    switch (gGuideSt_CSS->state)
    {
        case GUIDE_STATE_0:
            for (iy = 0; iy < 5; iy++)
            {
                for (ix = 0; ix < 8; ix++)
                {
                    gBG1TilemapBuffer[ix + off + 0x42] = gBG1TilemapBuffer[ix + off + 0x02];
                    gBG1TilemapBuffer[ix + off + 0x62] = gBG1TilemapBuffer[ix + off + 0x22];
                }
                off = off - 0x40;
            }

            sub_80CE1C0(hm, hm % 6, 5);

            break;

        case GUIDE_STATE_1:
            for (iy = 0; iy < 5; iy++)
            {
                for (ix = 0; ix < 19; ix++)
                {
                    gBG1TilemapBuffer[ix + off + 0x4a] = gBG1TilemapBuffer[ix + off + 0x0a];
                    gBG1TilemapBuffer[ix + off + 0x6a] = gBG1TilemapBuffer[ix + off + 0x2a];
                }
                off = off - 0x40;
            }

            child = Proc_Start(gProcScr_GuideEntryListRedraw_Up_CSS, proc);
            child->unk_34 = hm;
    }

    BG_EnableSyncByMask(BG1_SYNC_BIT);

    return;
}

extern const struct ProcCmd gProcScr_GuideEntryListRedraw_Down_CSS[];

//! FE8U = 0x080CE858
LYN_REPLACE_CHECK(sub_80CE858);
void sub_80CE858(ProcPtr proc, int b)
{
    struct GuideProc * child;
    int ix;
    int iy;
    register int hm asm("r9") = b;

    int off = 0xa0;

    switch (gGuideSt_CSS->state)
    {
        case GUIDE_STATE_0:
            for (iy = 0; iy < 5; iy++)
            {
                for (ix = 0; ix < 8; ix++)
                {
                    gBG1TilemapBuffer[ix + off + 0x02] = gBG1TilemapBuffer[ix + off + 0x42];
                    gBG1TilemapBuffer[ix + off + 0x22] = gBG1TilemapBuffer[ix + off + 0x62];
                }
                off = off + 0x40;
            }

            sub_80CE1C0(hm, hm % 6, 15);

            break;

        case GUIDE_STATE_1:
            for (iy = 0; iy < 5; iy++)
            {
                for (ix = 0; ix < 19; ix++)
                {
                    gBG1TilemapBuffer[ix + off + 0x0a] = gBG1TilemapBuffer[ix + off + 0x4a];
                    gBG1TilemapBuffer[ix + off + 0x2a] = gBG1TilemapBuffer[ix + off + 0x6a];
                }
                off = off + 0x40;
            }

            child = Proc_Start(gProcScr_GuideEntryListRedraw_Down_CSS, proc);
            child->unk_34 = hm;
    }

    BG_EnableSyncByMask(BG1_SYNC_BIT);

    return;
}

//! FE8U = 0x080CE95C
LYN_REPLACE_CHECK(GuideDetailsRedraw_Init);
void GuideDetailsRedraw_Init(struct GuideProc * proc)
{
    int textIdx;
    const char * str;
    int unk_34;

    unk_34 = proc->unk_34;
    textIdx = (unk_34 % 5);

    str = GetStringFromIndex(gGuideTable[gGuideSt_CSS->unk_68[gGuideSt_CSS->unk_2b]].details);

    while (unk_34 != 0)
    {
        str = GetStringNextLine(str);
        if (str == NULL)
        {
            break;
        }

        unk_34--;
    }

    ClearText(&gGuideSt_CSS->unk_b4[1 + textIdx]);
    PutDrawText(
        &gGuideSt_CSS->unk_b4[1 + textIdx], TILEMAP_LOCATED(gBG1TilemapBuffer, 11, 18), TEXT_COLOR_SYSTEM_WHITE, 0, 17,
        str);

    proc->unk_34 = 0;

    return;
}

//! FE8U = 0x080CE9E8
LYN_REPLACE_CHECK(GuideDetailsRedraw_Loop);
void GuideDetailsRedraw_Loop(struct GuideProc * proc)
{
    int iy;
    int ix;
    int baseA;
    int baseB;

    if (proc->unk_38 == 0)
    {
        baseA = 0x1c0;
        baseB = 0x260;
        for (iy = 0; iy < 7; iy++)
        {
            for (ix = 0; ix < 19; ix++)
            {
                gBG1TilemapBuffer[baseA + 0x2a + ix] = gBG1TilemapBuffer[baseA + 0x0a + ix];
            }
            baseA -= 0x20;
        }

        if (proc->unk_34 != 0)
        {
            baseB -= 0x20;
        }

        for (ix = 0; ix < 19; ix++)
        {
            gBG1TilemapBuffer[baseA + 0x2a + ix] = gBG1TilemapBuffer[baseB + 0x0a + ix];
        }
    }
    else
    {
        baseA = 0x100;
        baseB = 0x220;
        for (iy = 0; iy < 7; iy++)
        {
            for (ix = 0; ix < 19; ix++)
            {
                gBG1TilemapBuffer[baseA + 0x0a + ix] = gBG1TilemapBuffer[baseA + 0x2a + ix];
            }
            baseA += 0x20;
        }

        if (proc->unk_34 != 0)
        {
            baseB += 0x20;
        }

        for (ix = 0; ix < 0x13; ix++)
        {
            gBG1TilemapBuffer[baseA + 0x0a + ix] = gBG1TilemapBuffer[baseB + 0x2a + ix];
        }
    }

    BG_EnableSyncByMask(BG1_SYNC_BIT);

    if (proc->unk_34 == 0)
    {
        proc->unk_34 = 1;
        return;
    }

    Proc_Break(proc);

    return;
}

//! FE8U = 0x080CEAE8
LYN_REPLACE_CHECK(sub_80CEAE8);
void sub_80CEAE8(void)
{
    int i;
    u8 local[20];
    int r3;
    int r4;

    for (i = 0; i < 20; i++)
    {
        local[i] = 0;
        // gGuideSt_CSS->unk_54 = 0;
    }
    memset(gGuideSt_CSS->unk_54, 0, sizeof(gGuideSt_CSS->unk_54));

    i = 0;
    r4 = gGuideTable[i].title;

    while (gGuideTable[i].title != 0xc)
    {
        if (CheckFlag(gGuideTable[i].displayFlag))
        {
            local[r4] = r4;
        }

        i++;
        r4 = gGuideTable[i].title;
    }

    gGuideSt_CSS->unk_3d = 0;

    for (i = 0; i < 0xc; i++)
    {
        int tmp2;

        r4 = local[i];
        if (r4 == 0)
        {
            continue;
        }

        if (gGuideSt_CSS->unk_3d == 0)
        {
            gGuideSt_CSS->unk_54[0] = r4;
            gGuideSt_CSS->unk_3d++;
        }
        else
        {
            r3 = 0;
            tmp2 = (r3 < gGuideSt_CSS->unk_3d) && (gGuideSt_CSS->unk_54[0] == r4);
            if (tmp2 != 0)
            {
                continue;
            }
            gGuideSt_CSS->unk_54[gGuideSt_CSS->unk_3d] = r4;
            gGuideSt_CSS->unk_3d++;
        }
    }

    return;
}

//! FE8U = 0x080CEBA4
LYN_REPLACE_CHECK(sub_80CEBA4);
void sub_80CEBA4(void)
{
    int i;
    u8 local[20];
    int r3;
    int r4;

    for (i = 0; i < 20; i++)
    {
        local[i] |= 0xff;
        // gGuideSt_CSS->unk_40[i] = 0;
    }

    memset(gGuideSt_CSS->unk_40, 0, sizeof(gGuideSt_CSS->unk_40));

    i = 0;
    r4 = gGuideTable[i].title;

    while (r4 != 12)
    {
        if (CheckFlag(gGuideTable[i].displayFlag))
        {
            r4 = gGuideTable[i].chapterTitle;
            local[r4] = r4;
        }

        i++;
        r4 = gGuideTable[i].title;
    }

    gGuideSt_CSS->unk_3c = 0;

    for (i = 0; i < 12; i++)
    {
        int tmp2;

        if (local[i] == 0xff)
        {
            continue;
        }

        r4 = local[i];

        if (gGuideSt_CSS->unk_3c == 0)
        {
            gGuideSt_CSS->unk_40[0] = r4;
            gGuideSt_CSS->unk_3c++;
        }
        else
        {
            r3 = 0;
            tmp2 = (r3 < gGuideSt_CSS->unk_3c) && (gGuideSt_CSS->unk_40[0] == r4);
            if (tmp2 != 0)
            {
                continue;
            }
            gGuideSt_CSS->unk_40[gGuideSt_CSS->unk_3c] = r4;
            gGuideSt_CSS->unk_3c++;
        }
    }

    return;
}

//! FE8U = 0x080CEC68
LYN_REPLACE_CHECK(sub_80CEC68);
void sub_80CEC68(u16 off)
{
    int ix;
    int iy;

    int yBase = 160;

    for (iy = 0; iy < 12; iy++)
    {
        for (ix = 0; ix < 9; ix++)
        {
            gBG2TilemapBuffer[yBase + ix] = off + (gBG2TilemapBuffer[yBase + ix] & 0xFFF);
        }

        yBase += 32;
    }

    return;
}

// clang-format off

static struct ProcCmd const gProcScr_Guide_DrawSprites[] =
{
    PROC_NAME("E_guideSub"),

    PROC_CALL(GuideSpriteDraw_Init),
    PROC_REPEAT(GuideSpriteDraw_Loop),

    PROC_END,
};

// clang-format on

//! FE8U = 0x080CECB0
LYN_REPLACE_CHECK(Guide_Init);
void Guide_Init(ProcPtr proc)
{
    int i = 0;

    SetupBackgrounds(NULL);

    gGuideSt_CSS->state = GUIDE_STATE_0;

    gGuideSt_CSS->sortMode = CheckFlag(0xb3);

    gGuideSt_CSS->categoryIdx = 0;
    gGuideSt_CSS->unk_2a = 0;
    gGuideSt_CSS->unk_2b = 0;
    gGuideSt_CSS->unk_2c = 0;

    sub_80CEAE8();
    sub_80CEBA4();
    LoadUiFrameGraphics();

    SetDispEnable(1, 1, 1, 1, 1);

    BG_SetPosition(BG_0, 0, 0);
    BG_SetPosition(BG_1, 0, 0);
    BG_SetPosition(BG_2, 0, 0);
    BG_SetPosition(BG_3, 0, 0);

    BG_Fill(gBG0TilemapBuffer, 0);
    BG_Fill(gBG1TilemapBuffer, 0);
    BG_Fill(gBG2TilemapBuffer, 0);
    BG_Fill(gBG3TilemapBuffer, 0);

    SetWinEnable(1, 0, 0);
    SetWin0Box(0, 40, DISPLAY_WIDTH, 136);

    SetWin0Layers(1, 1, 1, 1, 1);
    SetWOutLayers(1, 0, 1, 1, 1);

    ApplyPalette(Pal_08B17B44, 0x12);
    Decompress(Img_08B17864, (void *)0x06011000);
    Decompress(Img_08B177C0, (void *)0x06011800);

    Decompress(Tsa_08B176CC, gGenericBuffer + 0x100);
    CallARM_FillTileRect(gBG2TilemapBuffer, gGenericBuffer + 0x100, 0x1000);

    ApplyPalette(gUiFramePaletteA + (gPlaySt.config.windowColor + 4) * 0x10, 2);

    ResetTextFont();

    InitText(&gGuideSt_CSS->unk_ec, 22);

    PutGuideBottomBarText();

    InitText(&gGuideSt_CSS->unk_ac, 9);
    InitText(&gGuideSt_CSS->unk_e4, 18);

    for (i = 0; i < 6; i++)
    {
        InitText(&gGuideSt_CSS->unk_7c[i], 9);
        InitText(&gGuideSt_CSS->unk_b4[i], 18);
    }

    sub_80CE248();
    sub_80CE414();

    StartMuralBackgroundExt(proc, 0, 18, 2, 0);
    Proc_Start(gProcScr_Guide_DrawSprites, proc);

    BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT | BG2_SYNC_BIT | BG3_SYNC_BIT);

    StartMenuScrollBarExt(proc, 224, 47, 0x800, 4);
    UpdateMenuScrollBarConfig(10, gGuideSt_CSS->unk_2c * 16, gGuideSt_CSS->unk_3e, 6);

    sub_8097668();

    return;
}

//! FE8U = 0x080CEF10
LYN_REPLACE_CHECK(Guide_SetBlend);
void Guide_SetBlend(void)
{
    SetBlendAlpha(15, 4);
    SetBlendTargetA(0, 0, 1, 0, 0);
    SetBlendTargetB(0, 0, 0, 1, 0);
    return;
}

//! FE8U = 0x080CEF48
LYN_REPLACE_CHECK(GetGuideAction);
int GetGuideAction(ProcPtr proc)
{
    switch (gGuideSt_CSS->state)
    {
        case GUIDE_STATE_0:
            if (gKeyStatusPtr->newKeys & SELECT_BUTTON)
            {
                return GUIDE_ACTION_SORT;
            }

            // fallthrough

        case GUIDE_STATE_1:
            if (gKeyStatusPtr->newKeys & A_BUTTON)
            {
                return GUIDE_ACTION_A_PRESS;
            }

            if (gKeyStatusPtr->newKeys & B_BUTTON)
            {
                return GUIDE_ACTION_CANCEL;
            }

            break;

        case GUIDE_STATE_2:
            if (gKeyStatusPtr->newKeys & B_BUTTON)
            {
                return GUIDE_ACTION_CANCEL;
            }

            if (gKeyStatusPtr->newKeys & (A_BUTTON | DPAD_RIGHT))
            {
                return GUIDE_ACTION_ADVANCE_TEXT;
            }

            if (gKeyStatusPtr->newKeys & DPAD_LEFT)
            {
                return GUIDE_ACTION_REVERSE_TEXT;
            }
    }

    if (gKeyStatusPtr->repeatedKeys & (DPAD_UP | DPAD_DOWN))
    {
        return GUIDE_ACTION_1;
    }
    // Jester - Adding this because there isn't a default retorn for this int function
    return 0;
}

// clang-format off

struct ProcCmd const gProcScr_GuideCategoryRedraw_CSS[] =
{
    PROC_NAME("E_guMenu1ReWrite"),
    PROC_SLEEP(1),

    PROC_CALL(sub_80CE28C),
    PROC_CALL(GuideMenuRefresh_SyncBg1),
    PROC_SLEEP(1),

    PROC_CALL(sub_80CE248),
    PROC_SLEEP(1),

    PROC_CALL(sub_80CE414),
    PROC_SLEEP(1),

    PROC_CALL(PutGuideBottomBarText),
    PROC_CALL(GuideMenuRefresh_SyncBg0Bg1),

    PROC_END,
};

struct ProcCmd const gProcScr_GuideEntryListRedraw_Initial_CSS[] =
{
    PROC_NAME("E_guMenu2ReWriteFirst"),
    PROC_SLEEP(1),

    PROC_CALL(GuideEntry_DrawInitial),
    PROC_CALL(GuideMenuRefresh_SyncBg1),
    PROC_SLEEP(1),

    PROC_CALL(sub_80CE414),
    PROC_SLEEP(1),

    PROC_CALL(PutGuideBottomBarText),
    PROC_CALL(GuideMenuRefresh_SyncBg1),

    PROC_END,
};

struct ProcCmd const gProcScr_GuideEntryListRedraw_Up_CSS[] =
{
    PROC_NAME("E_guMenu2ReWriteUp"),
    PROC_SLEEP(1),

    PROC_CALL(GuideEntry_RedrawUp),
    PROC_CALL(GuideMenuRefresh_SyncBg1),

    PROC_END,
};

struct ProcCmd const gProcScr_GuideEntryListRedraw_Down_CSS[] =
{
    PROC_NAME("E_guMenu2ReWriteDown"),
    PROC_SLEEP(1),

    PROC_CALL(GuideEntry_RedrawDown),
    PROC_CALL(GuideMenuRefresh_SyncBg1),

    PROC_END,
};

struct ProcCmd const gProcScr_GuideDetailsRedraw_CSS[] =
{
    PROC_NAME("E_guMess3ReWrite"),
    PROC_SLEEP(1),

    PROC_CALL(GuideDetailsRedraw_Init),
    PROC_REPEAT(GuideDetailsRedraw_Loop),

    PROC_END,
};

// clang-format on
LYN_REPLACE_CHECK(Guide_MainLoop);
void Guide_MainLoop(struct GuideProc * proc)
{
    struct GuideProc * proc_ = proc;
    s8 flag = 0;

    switch (GetGuideAction(proc))
    {
        case GUIDE_ACTION_A_PRESS:
            PlaySoundEffect(0x6a);

            gGuideSt_CSS->state++;

            switch (gGuideSt_CSS->state)
            {
                case GUIDE_STATE_1:
                    sub_80CEC68(0x2000);
                    BG_EnableSyncByMask(BG2_SYNC_BIT);

                    break;

                case GUIDE_STATE_2:
                    gGuideSt_CSS->detailLinesScrolled = 0;
                    SetFlag(gGuideTable[gGuideSt_CSS->unk_68[gGuideSt_CSS->unk_2b]].readFlag);
                    MoveGuideDetailText(gGuideSt_CSS->unk_68[gGuideSt_CSS->unk_2b], GUIDE_DETAILS_STAY);
                    LockMenuScrollBar();

                    return;

                default:
                    return;
            }

            break;

        case GUIDE_ACTION_ADVANCE_TEXT:
            MoveGuideDetailText(gGuideSt_CSS->unk_68[gGuideSt_CSS->unk_2b], GUIDE_DETAILS_ADVANCE);
            break;

        case GUIDE_ACTION_REVERSE_TEXT:
            MoveGuideDetailText(gGuideSt_CSS->unk_68[gGuideSt_CSS->unk_2b], GUIDE_DETAILS_REVERSE);
            break;

        case GUIDE_ACTION_CANCEL:
            PlaySoundEffect(0x6b);

            if (gGuideSt_CSS->state != GUIDE_STATE_0)
            {
                gGuideSt_CSS->state--;

                switch (gGuideSt_CSS->state)
                {
                    case GUIDE_STATE_0:
                        sub_80CEC68(0x1000);
                        BG_EnableSyncByMask(BG2_SYNC_BIT);
                        break;

                    case GUIDE_STATE_1:
                        Proc_StartBlocking(gProcScr_GuideEntryListRedraw_Initial_CSS, proc_);
                        sub_8097668();
                        return;

                    default:
                        return;
                }
            }
            else
            {
                Proc_Break(proc_);
                return;
            }

            break;

        case GUIDE_ACTION_SORT:
            PlaySoundEffect(0x6a);

            gGuideSt_CSS->sortMode = (gGuideSt_CSS->sortMode + 1) & 1;
            if (gGuideSt_CSS->sortMode != GUIDE_SORT_MODE_TOPIC)
            {
                SetFlag(0xb3);
            }
            else
            {
                ClearFlag(0xb3);
            }

            gGuideSt_CSS->categoryIdx = 0;
            gGuideSt_CSS->unk_2a = 0;
            gGuideSt_CSS->unk_2b = 0;
            gGuideSt_CSS->unk_2c = 0;

            Proc_StartBlocking(gProcScr_GuideCategoryRedraw_CSS, proc_);

            break;

        case GUIDE_ACTION_1:
            switch (gGuideSt_CSS->state)
            {
                case GUIDE_STATE_0:
                    if (gKeyStatusPtr->repeatedKeys & DPAD_UP)
                    {
                        if (gGuideSt_CSS->categoryIdx != 0)
                        {
                            gGuideSt_CSS->categoryIdx--;

                            if (((gGuideSt_CSS->categoryIdx - gGuideSt_CSS->unk_2a) < 1) && (gGuideSt_CSS->unk_2a != 0))
                            {
                                gGuideSt_CSS->unk_2a--;
                                sub_80CE750(proc_, gGuideSt_CSS->categoryIdx - 1);
                            }

                            flag = 1;
                        }

                        if (!flag)
                        {
                            return;
                        }
                    }
                    else
                    {
                        if (gGuideSt_CSS->categoryIdx <
                            ((gGuideSt_CSS->sortMode != GUIDE_SORT_MODE_TOPIC ? 
                                gGuideSt_CSS->unk_3c : gGuideSt_CSS->unk_3d) - 1))
                        {
                            gGuideSt_CSS->categoryIdx++;

                            if ((gGuideSt_CSS->categoryIdx - gGuideSt_CSS->unk_2a) > 4)
                            {
                                if (gGuideSt_CSS->categoryIdx <
                                    ((gGuideSt_CSS->sortMode != GUIDE_SORT_MODE_TOPIC
                                        ? gGuideSt_CSS->unk_3c : gGuideSt_CSS->unk_3d) - 1))
                                {
                                    gGuideSt_CSS->unk_2a++;
                                    sub_80CE858(proc_, gGuideSt_CSS->categoryIdx + 1);
                                }
                            }
                            flag = 1;
                        }

                        if (!flag)
                        {
                            return;
                        }
                    }

                    Proc_Start(gProcScr_GuideEntryListRedraw_Initial_CSS, proc_);
                    gGuideSt_CSS->unk_2b = 0;
                    gGuideSt_CSS->unk_2c = 0;

                    break;

                case GUIDE_STATE_1:
                    if (gKeyStatusPtr->repeatedKeys & DPAD_UP)
                    {
                        if (gGuideSt_CSS->unk_2b != 0)
                        {
                            gGuideSt_CSS->unk_2b--;

                            if ((gGuideSt_CSS->unk_2b - gGuideSt_CSS->unk_2c < 1) && (gGuideSt_CSS->unk_2c != 0))
                            {
                                gGuideSt_CSS->unk_2c--;
                                sub_80CE750(proc_, gGuideSt_CSS->unk_2b - 1);
                            }

                            flag = 1;
                        }
                    }
                    else
                    {
                        if (gGuideSt_CSS->unk_2b < (gGuideSt_CSS->unk_3e - 1))
                        {
                            gGuideSt_CSS->unk_2b++;

                            if ((gGuideSt_CSS->unk_2b - gGuideSt_CSS->unk_2c > 4) && (gGuideSt_CSS->unk_2b < gGuideSt_CSS->unk_3e - 1))
                            {
                                gGuideSt_CSS->unk_2c++;
                                sub_80CE858(proc_, gGuideSt_CSS->unk_2b + 1);
                            }

                            flag = 1;
                        }
                    }

                    break;

                case GUIDE_STATE_2:
                    if (gKeyStatusPtr->repeatedKeys & DPAD_UP)
                    {
                        if (gGuideSt_CSS->detailLinesScrolled != 0)
                        {
                            gGuideSt_CSS->detailLinesScrolled--;
                            proc_ = Proc_StartBlocking(gProcScr_GuideDetailsRedraw_CSS, proc_);
                            proc_->unk_34 = gGuideSt_CSS->detailLinesScrolled;
                            proc_->unk_38 = 0;
                            flag = 1;
                        }
                    }
                    else
                    {
                        if (gGuideSt_CSS->detailLinesScrolled < gGuideSt_CSS->numDetailLines - 4)
                        {
                            gGuideSt_CSS->detailLinesScrolled++;

                            proc_ = Proc_StartBlocking(gProcScr_GuideDetailsRedraw_CSS, proc_);
                            proc_->unk_34 = gGuideSt_CSS->detailLinesScrolled + 3;
                            proc_->unk_38 = 1;

                            flag = 1;
                        }
                    }
            }

            if (!flag)
            {
                return;
            }

            PlaySoundEffect(0x66);
    }

    return;
}

//! FE8U = 0x080CF448
LYN_REPLACE_CHECK(Guide_OnEnd);
void Guide_OnEnd(void)
{
    EndMuralBackground();
    Proc_EndEach(gProcScr_Guide_DrawSprites);
    EndMenuScrollBar();
    return;
}

// clang-format off

// struct ProcCmd const ProcScr_E_Guide1_CSS[] =
// {
//     PROC_NAME("E_Guide"),

//     PROC_CALL(LockGame),
//     PROC_CALL(StartFastFadeToBlack),
//     PROC_REPEAT(WaitForFade),

//     PROC_CALL(BMapDispSuspend),
//     PROC_YIELD,

//     PROC_CALL(Guide_Init),
//     PROC_CALL(StartGreenText),

//     PROC_CALL(StartFastFadeFromBlack),
//     PROC_REPEAT(WaitForFade),

//     PROC_CALL(Guide_SetBlend),
//     PROC_REPEAT(Guide_MainLoop),

//     PROC_CALL(StartFastFadeToBlack),
//     PROC_REPEAT(WaitForFade),

//     PROC_CALL(EndGreenText),

//     PROC_CALL(Guide_OnEnd),
//     PROC_YIELD,

//     PROC_CALL(BMapDispResume),
//     PROC_CALL(RefreshBMapGraphics),

//     PROC_CALL(StartFastFadeFromBlack),
//     PROC_REPEAT(WaitForFade),
//     PROC_CALL(UnlockGame),

//     PROC_END,
// };

struct ProcCmd const ProcScr_E_Guide2_CSS[] =
{
    PROC_NAME("E_Guide"),

    PROC_CALL(LockGame),

    PROC_CALL(BMapDispSuspend),
    PROC_YIELD,

    PROC_CALL(Guide_Init),
    PROC_CALL(StartGreenText),

    PROC_CALL(StartFastFadeFromBlack),
    PROC_REPEAT(WaitForFade),

    PROC_CALL(Guide_SetBlend),
    PROC_REPEAT(Guide_MainLoop),

    PROC_CALL(StartFastFadeToBlack),
    PROC_REPEAT(WaitForFade),

    PROC_CALL(EndGreenText),

    PROC_CALL(Guide_OnEnd),
    PROC_YIELD,

    PROC_CALL(BMapDispResume),
    PROC_CALL(RefreshBMapGraphics),
    PROC_CALL(UnlockGame),

    PROC_END,
};

LYN_REPLACE_CHECK(BmGuideTextSetAllGreen);
void BmGuideTextSetAllGreen(void)
{
    struct GuideEnt * it;

    for (it = gGuideTable; it->title != 12; it++)
    {
        SetFlag(it->displayFlag);
    }
    return;
}

/* This breaks unless I return immediately. No idea why, NoCashPrintf prints fuck all */
LYN_REPLACE_CHECK(BmGuideTextShowGreenOrNormal);
bool BmGuideTextShowGreenOrNormal(void)
{
    return FALSE;
    struct GuideEnt * it;

    for (it = gGuideTable; it->title != 12; it++)
    {
        if (CheckFlag(it->displayFlag) && !CheckFlag(it->readFlag))
        {
            return FALSE;
        }
    }
    return TRUE;
}

// Pad the data here so that the linker script doesn't need to specify the
// exact location of data after this file.
const u8 TextFiller_css[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// LYN_REPLACE_CHECK(MapMenu_GuideCommand);
// u8 MapMenu_GuideCommand(struct MenuProc* menu, struct MenuItemProc* menuItem) {
//     Proc_Start(ProcScr_E_Guide1_CSS, PROC_TREE_3);

//     return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
// }

LYN_REPLACE_CHECK(WorldMap_CallGuide);
s8 WorldMap_CallGuide(ProcPtr proc)
{
    Proc_BlockEachMarked(PROC_MARK_WMSTUFF);
    Proc_StartBlocking(ProcScr_E_Guide2_CSS, proc);
    return 0;
}
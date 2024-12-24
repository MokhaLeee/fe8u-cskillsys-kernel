#include "common-chax.h"
#include "help-box.h"
#include "combat-art.h"

STATIC_DECLAR void sub_808A200_vanilla(const struct HelpBoxInfo * info)
{
    int wTextBox;
    int hTextBox;

    struct HelpBoxProc * proc = Proc_Find(ProcScr_Helpbox_bug_08A01678);

    if (!proc)
    {
        proc = Proc_Start(ProcScr_Helpbox_bug_08A01678, PROC_TREE_3);

        PlaySoundEffect(0x70);
        sub_808A43C(proc, info->xDisplay, info->yDisplay);
        SetHelpBoxDefaultRect(proc);
    }
    else
    {
        proc->xBoxInit = proc->xBox;
        proc->yBoxInit = proc->yBox;
        proc->wBoxInit = proc->wBoxFinal;
        proc->hBoxInit = proc->hBoxFinal;
    }

    proc->info = info;
    proc->timer = 0;
    proc->timerMax = 12;

    proc->mid = info->mid;

    SetTextFontGlyphs(1);
    GetStringTextBox(GetStringFromIndex(proc->mid), &wTextBox, &hTextBox);
    SetTextFontGlyphs(0);

    sub_808A384(proc, wTextBox, hTextBox);
    sub_808A3C4(proc, info->xDisplay, info->yDisplay);

    ClearHelpBoxText();
    StartHelpBoxTextInit(proc->item, proc->mid);

    gpHelpBoxCurrentInfo = info;
}

LYN_REPLACE_CHECK(sub_808A200);
void sub_808A200(const struct HelpBoxInfo * info)
{
    sHelpBoxType = 0;
    sub_808A200_vanilla(info);
}

LYN_REPLACE_CHECK(HelpBoxSetupstringLines);
void HelpBoxSetupstringLines(struct ProcHelpBoxIntro * proc)
{
    int item = proc->item;

    SetTextFont(&gHelpBoxSt.font);
    SetTextFontGlyphs(0);

    if (sHelpBoxType == 0)
    {
        /* Vanilla */
        switch (GetHelpBoxItemInfoKind(item)) {
        case HB_EXTINFO_NONE:
            proc->pretext_lines = 0;
            break;

        case HB_EXTINFO_WEAPON:
            DrawHelpBoxWeaponLabels(item);
            proc->pretext_lines = 2;
            break;

        case HB_EXTINFO_STAFF:
            DrawHelpBoxStaffLabels(item);
            proc->pretext_lines = 1;
            break;

        case HB_EXTINFO_SAVEINFO:
            DrawHelpBoxSaveMenuLabels();
            proc->pretext_lines = 1;
            break;
        }
    }
    else
    {
        /* Hack here */
        if (NEW_HB_COMBAT_ART_BKSEL == sHelpBoxType)
        {
            if (!GetCombatArtInfo(proc->item)->battle_status.display_en_n)
            {
                DrawHelpBoxCombatArtBkselLabels();
                proc->pretext_lines = 2;
            }
            else
            {
                proc->pretext_lines = 0;
            }
        }
    }

    SetTextFont(0);
    Proc_Break(proc);
}

LYN_REPLACE_CHECK(HelpBoxDrawstring);
void HelpBoxDrawstring(struct ProcHelpBoxIntro * proc)
{
    int item = proc->item;
    SetTextFont(&gHelpBoxSt.font);

    if (sHelpBoxType == 0)
    {
        /* Vanilla */
        switch (GetHelpBoxItemInfoKind(item)) {
        case HB_EXTINFO_WEAPON:
            DrawHelpBoxWeaponStats(item);
            break;

        case HB_EXTINFO_SAVEINFO:
            DrawHelpBoxSaveMenuStats();
            break;
        }
    }
    else
    {
        /* Hack here */
        if (NEW_HB_COMBAT_ART_BKSEL == sHelpBoxType && !GetCombatArtInfo(proc->item)->battle_status.display_en_n)
        {
            DrawHelpBoxCombatArtBkselStats(proc);
        }
    }

    SetTextFont(0);
    Proc_Break(proc);
}

LYN_REPLACE_CHECK(sub_808A454);
int sub_808A454(int item) {

    if (sHelpBoxType == 0)
    {
        /* Vanilla */
        if (item == (u16)-2)
            return 3;

        if (GetItemAttributes(item) & IA_LOCK_3)
            return 0;

        if (GetItemAttributes(item) & IA_WEAPON)
            return 1;

        if (GetItemAttributes(item) & IA_STAFF)
            return 2;
    }
    else
    {
        /* Hack here */
        if (NEW_HB_COMBAT_ART_BKSEL == sHelpBoxType && !GetCombatArtInfo(item)->battle_status.display_en_n)
            return 2;
    }

    return 0;
}

LYN_REPLACE_CHECK(ApplyHelpBoxContentSize);
void ApplyHelpBoxContentSize(struct HelpBoxProc* proc, int width, int height)
{
    width = 0xF0 & (width + 15); // align to 16 pixel multiple

    if (sHelpBoxType == 0)
    {
        /* Vanilla */
        switch (GetHelpBoxItemInfoKind(proc->item)) {
        case 1: // weapon
            if (width < 0x90)
                width = 0x90;

            if (GetStringTextLen(GetStringFromIndex(proc->mid)) > 8)
                height += 0x20;
            else
                height += 0x10;

            break;
        
        case 2: // staff
            if (width < 0x60)
                width = 0x60;

            height += 0x10;
            break;

        case 3: // save stuff
            width = 0x80;
            height += 0x10;
            break;
        }
    }
    else
    {
        /* Hack here */
        if (NEW_HB_COMBAT_ART_BKSEL == sHelpBoxType && !GetCombatArtInfo(proc->item)->battle_status.display_en_n)
        {
            if (width < 0x90)
                width = 0x90;

            if (GetStringTextLen(GetStringFromIndex(proc->mid)) > 8)
                height += 0x20;
            else
                height += 0x10;
        }
    }

    proc->wBoxFinal = width;
    proc->hBoxFinal = height;
}

LYN_REPLACE_CHECK(StartHelpBoxExt);
void StartHelpBoxExt(const struct HelpBoxInfo * info, int unk)
{
    struct HelpBoxProc* proc;
    int wContent, hContent;

    proc = (void*) Proc_Find(gProcScr_HelpBox);

    if (!proc)
    {
        proc = (void*) Proc_Start(gProcScr_HelpBox, PROC_TREE_3);

        proc->unk52 = unk;

        SetHelpBoxInitPosition(proc, info->xDisplay, info->yDisplay);
        ResetHelpBoxInitSize(proc);
    }
    else
    {
        proc->xBoxInit = proc->xBox;
        proc->yBoxInit = proc->yBox;

        proc->wBoxInit = proc->wBox;
        proc->hBoxInit = proc->hBox;
    }

    proc->info = info;

    proc->timer    = 0;
    proc->timerMax = 12;

    proc->item = 0;
    proc->mid = info->mid;

#if CHAX
    sHelpBoxType = NEW_HB_DEFAULT;
#endif

    if (proc->info->populate)
        proc->info->populate(proc);

    SetTextFontGlyphs(1);
    GetStringTextBox(GetStringFromIndex(proc->mid), &wContent, &hContent);
    SetTextFontGlyphs(0);

    ApplyHelpBoxContentSize(proc, wContent, hContent);
    ApplyHelpBoxPosition(proc, info->xDisplay, info->yDisplay);

    ClearHelpBoxText();
    StartHelpBoxTextInit(proc->item, proc->mid);

    sLastHbi = info;
}

LYN_REPLACE_CHECK(DisplayHelpBoxObj);
//! FE8U = 0x08089980
void DisplayHelpBoxObj(int x, int y, int w, int h, int unk) {
    s8 flag;
    s8 flag_;
    s8 anotherFlag;

    int xCount;
    int yCount;

    int xPx;
    int yPx;
    int iy;
    int ix;

    flag = (w + 7) & 0x10;
    anotherFlag = w & 0xf;

    if (w < 0x20) {
        w = 0x20;
    }

    if (w > 0xC0) {
        w = 0xc0;
    }

    if (h < 0x10) {
        h = 0x10;
    }

#ifdef CONFIG_EXTENDED_HELPBOXES
    /* Now we limit it to 5 lines (0x10 * 5) */
    if (h > 0x50) {
        h = 0x50;
    }
#else
    /* Vanilla behaviour to limit the help text box to three lines (0x10 * 3) */
    if (h > 0x30) {
        h = 0x30;
    }
#endif

    xCount = (w + 0x1f) / 0x20;
    yCount = (h + 0x0f) / 0x10;

    flag_ = flag;

    for (ix = xCount - 1; ix >= 0; ix--) {
        for (iy = yCount; iy >= 0; iy--) {

            yPx = (iy + 1) * 0x10;
            if (yPx > h) {
                yPx = h;
            }
            yPx -= 0x10;

            xPx = (ix + 1) * 0x20;

            if (flag_ != 0) {
                xPx -= 0x20;
                PutSprite(0,
                x + xPx,
                y + yPx,
                gObject_16x16,
                gHelpBoxSt.oam2_base + ix * 4 + iy * 0x40);
            } else {

                if (xPx > w)
                    xPx = w;

                xPx -= 0x20;
                PutSprite(
                    0,
                    x + xPx,
                    y + yPx,
                    gObject_32x16,
                    gHelpBoxSt.oam2_base + ix * 4 + iy * 0x40);
            }
        }

        flag_ = 0;
    }

    flag_ = flag;

    for (ix = xCount - 1; ix >= 0; ix--) {
        xPx = (ix + 1) * 0x20;

        if (flag_ != 0) {
            xPx -= 0x20;

            PutSprite(0, x + xPx, y - 8, gObject_16x8, gHelpBoxSt.oam2_base + 0x1b);
            PutSprite(0, x + xPx, y + h, gObject_16x8, gHelpBoxSt.oam2_base + 0x3b);

            flag_ = 0;
        } else {
            if (xPx > w) {
                xPx = w;
            }
            xPx -= 0x20;

            PutSprite(0, x + xPx, y - 8, gObject_32x8, gHelpBoxSt.oam2_base + 0x1b);
            PutSprite(0, x + xPx, y + h, gObject_32x8, gHelpBoxSt.oam2_base + 0x3b);

        }

    }

    for (iy = yCount; iy >= 0; iy--) {
        yPx = (iy + 1) * 0x10;
        if (yPx > h) {
            yPx = h;
        }
        yPx -= 0x10;

        PutSprite(0, x - 8, y + yPx, gObject_8x16, gHelpBoxSt.oam2_base + 0x5f);
        PutSprite(0, x + w, y + yPx, gObject_8x16, gHelpBoxSt.oam2_base + 0x1f);

        if (anotherFlag != 0) {
            PutSprite(0, x + w - 8, y + yPx, gObject_8x16, gHelpBoxSt.oam2_base + 0x1a);
        }
    }

    PutSprite(0, x - 8, y - 8, gObject_8x8, gHelpBoxSt.oam2_base + 0x5b); // top left
    PutSprite(0, x + w, y - 8, gObject_8x8, gHelpBoxSt.oam2_base + 0x5c); // top right
    PutSprite(0, x - 8, y + h, gObject_8x8, gHelpBoxSt.oam2_base + 0x5d); // bottom left
    PutSprite(0, x + w, y + h, gObject_8x8, gHelpBoxSt.oam2_base + 0x5e); // bottom right

    if (anotherFlag != 0) {
        PutSprite(0, x + w - 8, y - 8, gObject_8x8, gHelpBoxSt.oam2_base + 0x1b);
        PutSprite(0, x + w - 8, y + h, gObject_8x8, gHelpBoxSt.oam2_base + 0x3b);
    }

    if (unk == 0) {
        PutSprite(0, x, y - 0xb, gObject_32x16, (0x3FF & gHelpBoxSt.oam2_base) + 0x7b);
    }

    return;
}

//! FE8U = 0x0808A118
LYN_REPLACE_CHECK(ClearHelpBoxText);
void ClearHelpBoxText(void) {

    SetTextFont(&gHelpBoxSt.font);

    SpriteText_DrawBackground(&gHelpBoxSt.text[0]);
    SpriteText_DrawBackground(&gHelpBoxSt.text[1]);
    SpriteText_DrawBackground(&gHelpBoxSt.text[2]);

#ifdef CONFIG_EXTENDED_HELPBOXES
    SpriteText_DrawBackground(&gHelpBoxSt.text[3]);
    SpriteText_DrawBackground(&gHelpBoxSt.text[4]);
#endif

    Proc_EndEach(gProcScr_HelpBoxTextScroll);
    Proc_EndEach(ProcScr_HelpBoxIntro);

    SetTextFont(0);

    return;
}

//! FE8U = 0x08089CD4
LYN_REPLACE_CHECK(DrawHelpBoxWeaponStats);
void DrawHelpBoxWeaponStats(int item)
{
    Text_InsertDrawString(&gHelpBoxSt.text[0], 32, 7, GetItemDisplayRankString(item));
    Text_InsertDrawString(&gHelpBoxSt.text[0], 67, 7, GetItemDisplayRangeString(item));
    Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[0], 129, 7, GetItemWeight(item));

    Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[1], 32, 7, GetItemMight(item));
    Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[1], 81, 7, GetItemHit(item));
    Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[1], 129, 7, GetItemCrit(item));
}
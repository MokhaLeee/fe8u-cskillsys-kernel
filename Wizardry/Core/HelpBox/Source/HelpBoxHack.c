#include "global.h"
#include "fontgrp.h"
#include "statscreen.h"
#include "soundwrapper.h"
#include "bmitem.h"
#include "helpbox.h"

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

/* LynJump */
void sub_808A200(const struct HelpBoxInfo * info)
{
    sHelpBoxType = 0;
    sub_808A200_vanilla(info);
}

/* LynJump */
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
            DrawHelpBoxCombatArtBkselLabels();
            proc->pretext_lines = 2;
        }
    }

    SetTextFont(0);
    Proc_Break(proc);
}

/* LynJump */
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
        if (NEW_HB_COMBAT_ART_BKSEL == sHelpBoxType)
        {
            DrawHelpBoxCombatArtBkselStats();
        }
    }

    SetTextFont(0);
    Proc_Break(proc);
}

/* LynJump */
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
        if (NEW_HB_COMBAT_ART_BKSEL == sHelpBoxType)
            return 2;
    }

    return 0;
}

/* LynJump */
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
        if (NEW_HB_COMBAT_ART_BKSEL == sHelpBoxType)
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

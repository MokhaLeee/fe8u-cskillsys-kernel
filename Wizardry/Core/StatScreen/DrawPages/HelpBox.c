#include "common-chax.h"
#include "stat-screen.h"
#include "kernel-lib.h"
#include "skill-system.h"

LYN_REPLACE_CHECK(StartStatScreenHelp);
void StartStatScreenHelp(int pageid, struct Proc * proc)
{
    LoadHelpBoxGfx(NULL, -1); // default

    if (!gStatScreen.help)
    {
        switch (pageid) {
        case STATSCREEN_PAGE_0:
            StartUnitScreenHelp(pageid, proc);
            break;

        case STATSCREEN_PAGE_1:
            gStatScreen.help = &gHelpInfo_Ss1Item0;
            break;

        case STATSCREEN_PAGE_2:
            StartSkillScreenHelp(pageid, proc);
            break;

        case 3:
            gStatScreen.help = RTextPageSupport;
            break;
        } // switch (pageid)
    }
    StartMovingHelpBox(gStatScreen.help, proc);
}

LYN_REPLACE_CHECK(DisplayPage);
void DisplayPage(int pageid)
{
    typedef void(* func_type)(void);
    extern const func_type gStatScreenDrawPages[];

    CpuFastFill(0, gUiTmScratchA, sizeof(gUiTmScratchA));
    CpuFastFill(0, gUiTmScratchC, sizeof(gUiTmScratchC));

    gStatScreenDrawPages[pageid]();
}

void HbPopuplate_Page3Skill(struct HelpBoxProc * proc)
{
    struct SkillList * list = GetUnitSkillList(gStatScreen.unit);
    proc->mid = GetSkillDescMsg(list->sid[proc->info->mid]);
}

void HbRedirect_Page3Skill(struct HelpBoxProc * proc)
{
    if (proc->info->mid < GetUnitSkillList(gStatScreen.unit)->amt)
        return;

    switch (proc->moveKey) {
    case DPAD_DOWN:
        TryRelocateHbDown(proc);
        break;

    case DPAD_UP:
        TryRelocateHbUp(proc);
        break;

    case DPAD_LEFT:
        TryRelocateHbLeft(proc);
        break;

    case DPAD_RIGHT:
    default:
        TryRelocateHbRight(proc);
        break;
    } // switch
}

LYN_REPLACE_CHECK(LoadHelpBoxGfx);
void LoadHelpBoxGfx(void * vram, int palId)
{
// Repoint the vram used for the stat screen help box
#ifdef CONFIG_EXTENDED_HELPBOXES
    if (vram == NULL) {
        vram = (void *)0x06012000;
    }
#else
    if (vram == NULL) {
        vram = (void *)0x06013000;
    }
#endif

    if (palId < 0) {
        palId = 5;
    }

    palId = (palId & 0xF) + 0x10;

    Decompress(gGfx_HelpTextBox, vram + 0x360);
    Decompress(gGfx_HelpTextBox2, vram + 0x760);
    Decompress(gGfx_HelpTextBox3, vram + 0xb60);
    Decompress(gGfx_HelpTextBox4, vram + 0xf60);
    Decompress(gGfx_HelpTextBox5, vram + 0x1360);

    InitSpriteTextFont(&gHelpBoxSt.font, vram, palId);

    InitSpriteText(&gHelpBoxSt.text[0]);
    InitSpriteText(&gHelpBoxSt.text[1]);
    InitSpriteText(&gHelpBoxSt.text[2]);

#ifdef CONFIG_EXTENDED_HELPBOXES
    InitSpriteText(&gHelpBoxSt.text[3]);
    InitSpriteText(&gHelpBoxSt.text[4]);
#endif

    SetTextFont(0);

    ApplyPalette(Pal_HelpBox, palId);

    gHelpBoxSt.oam2_base = (((u32)vram << 0x11) >> 0x16) + (palId & 0xF) * 0x1000;
}

//! FE8U = 0x0808A00C
LYN_REPLACE_CHECK(HelpBoxIntroDrawTexts);
void HelpBoxIntroDrawTexts(struct ProcHelpBoxIntro * proc)
{
    struct HelpBoxScrollProc * otherProc;
    int textSpeed;

    SetTextFont(&gHelpBoxSt.font);

    SetTextFontGlyphs(1);

    Text_SetColor(&gHelpBoxSt.text[0], 6);
    Text_SetColor(&gHelpBoxSt.text[1], 6);
    Text_SetColor(&gHelpBoxSt.text[2], 6);

#ifdef CONFIG_EXTENDED_HELPBOXES
    Text_SetColor(&gHelpBoxSt.text[3], 6);
    Text_SetColor(&gHelpBoxSt.text[4], 6);
#endif

    SetTextFont(0);

    Proc_EndEach(gProcScr_HelpBoxTextScroll);

    otherProc = Proc_Start(gProcScr_HelpBoxTextScroll, PROC_TREE_3);
    otherProc->font = &gHelpBoxSt.font;

    otherProc->texts[0] = &gHelpBoxSt.text[0];
    otherProc->texts[1] = &gHelpBoxSt.text[1];
    otherProc->texts[2] = &gHelpBoxSt.text[2];

#ifdef CONFIG_EXTENDED_HELPBOXES
    otherProc->texts[3] = &gHelpBoxSt.text[3];
    otherProc->texts[4] = &gHelpBoxSt.text[4];
#endif

    otherProc->pretext_lines = proc->pretext_lines;

    // GetStringFromIndex writes to sMsgString.buffer1, which is then used by StringInsertSpecialPrefixByCtrl a couple lines later
    GetStringFromIndex(proc->msg);

    otherProc->string = StringInsertSpecialPrefixByCtrl();
    otherProc->chars_per_step = 1;
    otherProc->step = 0;

    textSpeed = gPlaySt.config.textSpeed;
    switch (gPlaySt.config.textSpeed) {
    case 0: /* default speed */
        otherProc->speed = 2;
        break;

    case 1: /* slow */
        otherProc->speed = textSpeed;
        break;

    case 2: /* fast */
        otherProc->speed = 1;
        otherProc->chars_per_step = textSpeed;
        break;

    case 3: /* draw all at once */
        otherProc->speed = 0;
        otherProc->chars_per_step = 0x7f;
        break;
    }
}
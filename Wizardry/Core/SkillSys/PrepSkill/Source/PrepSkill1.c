/**
 * On select unit
 */

#include "global.h"
#include "proc.h"
#include "prepscreen.h"
#include "hardware.h"
#include "soundwrapper.h"
#include "statscreen.h"
#include "icon.h"
#include "sysutil.h"
#include "helpbox.h"

#include "common-chax.h"
#include "skill-system.h"
#include "prep-skill.h"

STATIC_DECLAR void ProcPrepSkill1_OnEnd(struct ProcPrepSkill1 * proc)
{
    struct ProcAtMenu * pproc = proc->proc_parent;
    pproc->state = 1; /* Unit Select */
    pproc->yDiff = proc->yDiff_cur;
    pproc->cur_counter = proc->cur_counter;
    PrepSetLatestCharId(GetUnitFromPrepList(proc->list_num_cur)->pCharacterData->number);
    EndMuralBackground_();
}

STATIC_DECLAR void ProcPrepSkill1_InitScreen(struct ProcPrepSkill1 * proc)
{
    u16 BgConfig[12] = {
        // tile offset    map offset    screen size
        0x0000,            0xE000,        0,            // BG 0
        0x0000,            0xE800,        0,            // BG 1
        0x0000,            0xF000,        0,            // BG 2
        0x8000,            0xF800,        0,            // BG 3
    };

    int i;
    struct Unit * unit = GetUnitFromPrepList(proc->list_num_cur);

    SetupBackgrounds(BgConfig);
    SetDispEnable(0, 0, 0, 0, 0);
    BG_Fill(gBG0TilemapBuffer, 0);
    BG_Fill(gBG1TilemapBuffer, 0);
    BG_Fill(gBG2TilemapBuffer, 0);

    gLCDControlBuffer.bg0cnt.priority = 2;
    gLCDControlBuffer.bg1cnt.priority = 2;
    gLCDControlBuffer.bg2cnt.priority = 1;
    gLCDControlBuffer.bg3cnt.priority = 3;

    BG_SetPosition(BG_0, 0, 0);
    BG_SetPosition(BG_1, 0, 0);
    BG_SetPosition(BG_2, 0, proc->yDiff_cur - 0x18);
    BG_SetPosition(BG_3, 0, 0);

    PrepSkill1_InitTexts();
    PrepUnit_InitGfx();

    BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT | BG2_SYNC_BIT);
    SetDefaultColorEffects();

    PrepUnit_InitSMS((ProcPtr)proc);
    StartParallelWorker(PrepUnit_DrawSMSAndObjs, proc);
    ResetSysHandCursor(proc);
    DisplaySysHandCursorTextShadow(0x600, 0x1);
    ShowSysHandCursor(
        (proc->list_num_cur % 2) * 56 + 0x70,
        (proc->list_num_cur / 2) * 16 + 0x18 - proc->yDiff_cur,
        0x7, 0x800);

    StartHelpPromptSprite(0x20, 0x8F, 9, proc);
    PrepUnit_DrawLeftUnitName(unit);

    StartParallelFiniteLoop(PrepSkill1_DrawLeftSkillIcon, 0, proc);

    for (i = 0; i < 6; i++)
        PrepUnit_DrawUnitListNames((ProcPtr)proc, proc->yDiff_cur / 0x10 + i);

    StartParallelFiniteLoop(PrepSkill1_DrawRightTopBar, 0, proc);

    StartGreenText(proc);
    LoadHelpBoxGfx(BG_SCREEN_ADDR(0x29), 5);
    RestartMuralBackground();
}

STATIC_DECLAR void ProcPrepSkill1_Idle(struct ProcPrepSkill1 * proc)
{
    if (proc->list_num_pre == proc->list_num_cur)
    {
        int key_pre = gKeyStatusPtr->repeatedKeys;

        proc->scroll_val = 4;

        /* Speed up if L button pressed */
        if (L_BUTTON & gKeyStatusPtr->heldKeys)
        {
            key_pre = gKeyStatusPtr->heldKeys;
            proc->scroll_val = 8;
        }

        /* Directly start the game */
        if (START_BUTTON & gKeyStatusPtr->newKeys)
        {
            if (0 == proc->cur_counter)
            {
                PlaySoundEffect(0x6C);
            }
            else
            {
                PlaySoundEffect(0x6A);
                Proc_Goto(proc, PL_PREPSKILL1_PRESS_START);
            }
            return;
        }

        if (R_BUTTON & gKeyStatusPtr->newKeys)
        {
            Proc_Goto(proc, PL_PREPSKILL1_PRESS_R);
            return;
        }

        if (A_BUTTON & gKeyStatusPtr->newKeys)
        {
            PlaySoundEffect(0x6A);
            Proc_Goto(proc, PL_PREPSKILL1_PRESS_A);
            return;
        }

        if (B_BUTTON & gKeyStatusPtr->newKeys)
        {
            PlaySoundEffect(0x6B);
            Proc_Goto(proc, PL_PREPSKILL1_PRESS_B);
            return;
        }

        if (DPAD_LEFT & key_pre)
        {
            if (1 & proc->list_num_cur)
                proc->list_num_cur--;
        }

        if (DPAD_RIGHT & key_pre)
        {
            if (!(1 & proc->list_num_cur) && proc->list_num_cur < (PrepGetUnitAmount() - 1))
                proc->list_num_cur++;
        }

        if (DPAD_UP & key_pre)
        {
            if ((proc->list_num_cur - 2) >= 0)
                proc->list_num_cur -= 2;
        }

        if (DPAD_DOWN & key_pre)
        {
            if ((proc->list_num_cur + 2) <= (PrepGetUnitAmount() - 1))
                proc->list_num_cur += 2;
        }

        if (proc->list_num_pre == proc->list_num_cur)
            return;

        StartParallelFiniteLoop(PrepSkill1_DrawLeftSkillIcon, 0, proc);
        StartParallelFiniteLoop(PrepUnit_DrawLeftUnitNameCur, 0, proc);
        StartParallelFiniteLoop(PrepSkill1_DrawRightTopBar, 0, proc);
        PlaySoundEffect(0x65);
    
        if (ShouldPrepUnitMenuScroll(proc))
        {
            if (proc->list_num_cur < proc->list_num_pre)
                PrepUnit_DrawUnitListNames(proc, proc->yDiff_cur / 16 - 1);
            if (proc->list_num_cur > proc->list_num_pre)
                PrepUnit_DrawUnitListNames(proc, proc->yDiff_cur / 16 + 6);

            SetSysHandCursorXPos((1 & proc->list_num_cur) * 56 + 0x70);
        }
        else
        {
            proc->list_num_pre = proc->list_num_cur;
            ShowSysHandCursor(
                (1 & proc->list_num_pre) * 56 + 0x70,
                (proc->list_num_pre >> 1) * 16 + 0x18 - proc->yDiff_cur,
                0x7, 0x800
            );
        }
    
        if (proc->list_num_pre == proc->list_num_cur)
            return;
    }

    if (proc->list_num_cur < proc->list_num_pre)
        proc->yDiff_cur -= proc->scroll_val;

    if (proc->list_num_cur > proc->list_num_pre)
        proc->yDiff_cur += proc->scroll_val;

    if (0 == proc->yDiff_cur % 0x10) {
        PrepUpdateMenuTsaScroll(proc->yDiff_cur / 16 - 1);
        PrepUpdateMenuTsaScroll(proc->yDiff_cur / 16 + 6);
        sub_809AE10(proc);
        proc->list_num_pre = proc->list_num_cur;
    }

    BG_SetPosition(BG_2, 0, proc->yDiff_cur - 0x18);
}

STATIC_DECLAR void ProcPrepSkill_OnGameStart(struct ProcPrepSkill1 * proc)
{
    struct ProcAtMenu * pproc = proc->proc_parent;
    pproc->end_prep = 1;
    Proc_Goto(proc->proc_parent, 0x6);
    proc->button_blank = 1;
}

STATIC_DECLAR const struct ProcCmd ProcScr_PrepSkillUnitSel[] = {
    PROC_NAME("PrepSkillUnitSel"),
    PROC_YIELD,
    PROC_SET_END_CB(ProcPrepSkill1_OnEnd),

PROC_LABEL(PL_PREPSKILL1_INIT),
    PROC_CALL(ProcPrepSkill1_InitScreen),
    PROC_CALL_ARG(NewFadeIn, 0x10),
    PROC_WHILE(FadeInExists),

PROC_LABEL(PL_PREPSKILL1_IDLE),
    PROC_REPEAT(ProcPrepSkill1_Idle),

PROC_LABEL(PL_PREPSKILL1_PRESS_START),
    PROC_CALL(ProcPrepSkill_OnGameStart),
    PROC_SLEEP(0x1E),
    PROC_CALL_ARG(NewFadeOut, 0x8),
    PROC_WHILE(FadeOutExists),
    PROC_GOTO(PL_PREPSKILL1_END),

PROC_LABEL(PL_PREPSKILL1_PRESS_R),
    PROC_CALL(PrepUnitDisableDisp),
    PROC_SLEEP(0x2),
    PROC_CALL(sub_809B014),
    PROC_CALL(sub_809B504),
    PROC_YIELD,
    PROC_CALL(sub_809B520),
    PROC_CALL(ProcPrepSkill1_InitScreen),
    PROC_SLEEP(0x2),
    PROC_CALL(PrepUnitEnableDisp),
    PROC_GOTO(PL_PREPSKILL1_IDLE),

PROC_LABEL(PL_PREPSKILL1_PRESS_A),
    PROC_CALL_ARG(NewFadeOut, 0x10),
    PROC_WHILE(FadeOutExists),
    PROC_CALL(StartPrepSelectSkillScreen),
    PROC_YIELD,
    PROC_GOTO(PL_PREPSKILL1_INIT),

PROC_LABEL(PL_PREPSKILL1_PRESS_B),
    PROC_CALL_ARG(NewFadeOut, 0x10),
    PROC_WHILE(FadeOutExists),

PROC_LABEL(PL_PREPSKILL1_END),
    PROC_END
};

void StartPrepEquipScreen(struct ProcAtMenu * pproc)
{
    struct ProcPrepSkill1 * proc;
    proc = Proc_StartBlocking(ProcScr_PrepSkillUnitSel, pproc);

    MakePrepUnitList();
    proc->list_num_cur = UnitGetIndexInPrepList(PrepGetLatestCharId());
    proc->list_num_pre = proc->list_num_cur;
    proc->max_counter  = pproc->max_counter;
    proc->cur_counter  = pproc->cur_counter;
    proc->yDiff_cur    = pproc->yDiff;
    proc->button_blank = 0;
}

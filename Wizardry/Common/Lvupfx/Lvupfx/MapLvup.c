#include "common-chax.h"
#include "strmag.h"
#include "lvup.h"

#include "constants/texts.h"
#include "event-rework.h"
#include "action-expa.h"
#include "kernel-lib.h"
#include "constants/video-global.h"

LYN_REPLACE_CHECK(GetManimLevelUpStatGain);
int GetManimLevelUpStatGain(int actor_id, int stat_num)
{
    switch (stat_num) {
    case 0:
        return 1;

    case 1:
        return gManimSt.actor[actor_id].bu->changeHP;

    case 2:
        return gManimSt.actor[actor_id].bu->changePow;

    case 3:
        return BU_CHG_MAG(gManimSt.actor[actor_id].bu);

    case 4:
        return gManimSt.actor[actor_id].bu->changeSkl;

    case 5:
        return gManimSt.actor[actor_id].bu->changeSpd;

    case 6:
        return gManimSt.actor[actor_id].bu->changeLck;

    case 7:
        return gManimSt.actor[actor_id].bu->changeDef;

    case 8:
        return gManimSt.actor[actor_id].bu->changeRes;

    case 9:
        return gManimSt.actor[actor_id].bu->changeCon;

    default:
        return 0;
    }
}

LYN_REPLACE_CHECK(GetManimLevelUpBaseStat);
int GetManimLevelUpBaseStat(int actor_id, int stat_num)
{
    struct Unit * unit = GetUnit(gManimSt.actor[actor_id].unit->index);

    switch (stat_num) {
    case 0:
        return gManimSt.actor[actor_id].bu->levelPrevious;

    case 1:
        return unit->maxHP;

    case 2:
        return unit->pow;

    case 3:
        return UNIT_MAG(unit);

    case 4:
        return unit->skl;

    case 5:
        return unit->spd;

    case 6:
        return unit->lck;

    case 7:
        return unit->def;

    case 8:
        return unit->res;

    default:
        return 0;
    }
}

struct MapLvupInfo {
    u8 x, y;
    u16 msg;
};

STATIC_DECLAR const struct MapLvupInfo NewMapLvupInfos[] = {
    {0x09, 0x00, 0x4E7}, /* Lv */

    {0x01, 0x04, 0x4E9}, /* HP */
    {0x01, 0x06, 0x4FE}, /* Str */
    {0x01, 0x08, 0x4FF}, /* Mag */
    {0x01, 0x0A, 0x4EE}, /* Lck */

    {0x09, 0x04, 0x4EC}, /* Skl */
    {0x09, 0x06, 0x4ED}, /* Spd */
    {0x09, 0x08, 0x4EF}, /* Def */
    {0x09, 0x0A, 0x4F0}, /* Res */

    {-1, -1, 0}
};

LYN_REPLACE_CHECK(PutManimLevelUpFrame);
void PutManimLevelUpFrame(int actor_id, int x, int y)
{
    int i;

    BG_Fill(gBG0TilemapBuffer, 0);

    /* Background level up box image */
    Decompress(Img_LevelUpBoxFrame, (void *)VRAM + GetBackgroundTileDataOffset(1) + 0x200 * CHR_SIZE);
    Decompress(Tsa_LevelUpBoxFrame, gGenericBuffer);
    PutTmLinear((void *)gGenericBuffer, (void *)gBG1TilemapBuffer, 0x20 * 0x1C, TILEREF(0x200, BGPAL_MANIM_INFO));
    ApplyPalette(Pal_LevelUpBoxFrame, BGPAL_MANIM_INFO);

    PutString(
        TILEMAP_LOCATED(gBG0TilemapBuffer, x + 2, y),
        TEXT_COLOR_SYSTEM_WHITE,
        GetStringFromIndex(gManimSt.actor[actor_id].unit->pClassData->nameTextId));

    for (i = 0; NewMapLvupInfos[i].x != 0xFF; i++)
    {
        PutStringCentered(
            TILEMAP_LOCATED(gBG0TilemapBuffer, x + NewMapLvupInfos[i].x, y + NewMapLvupInfos[i].y),
            TEXT_COLOR_SYSTEM_GOLD, 3,
            GetStringFromIndex(NewMapLvupInfos[i].msg));
    }

    BG_EnableSyncByMask(BG0_SYNC_BIT + BG1_SYNC_BIT);
}

LYN_REPLACE_CHECK(PutManimLevelUpStat);
void PutManimLevelUpStat(int actor_id, int x, int y, int stat_num, bool after_gain)
{
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG0TilemapBuffer, x + NewMapLvupInfos[stat_num].x + 4, y + NewMapLvupInfos[stat_num].y),
        TEXT_COLOR_SYSTEM_BLUE,
        GetManimLevelUpBaseStat(actor_id, stat_num) + (after_gain ? GetManimLevelUpStatGain(actor_id, stat_num) : 0));
}

LYN_REPLACE_CHECK(StartManimLevelUp);
void StartManimLevelUp(int actor_id, ProcPtr parent)
{
    struct ManimLevelUpProc * proc;

    proc = Proc_StartBlocking(ProcScr_ManimLevelUp_CUSTOM, parent);
    proc->actor_id = actor_id;
}

// 0xFFFFFFFF // centered
// 0x000FFFFF // top-centered
// 0x00B0FFFF // bottom-centered
// 0x000FFB0F // top-left-centered
// 0x000fffbf // top-right-centered

static const EventScr EventScr_LevelUpSpeech[] = {
    EVBIT_MODIFY(0x4) // Can't skip scene
    TEXTSTART

    SVAL(EVT_SLOT_C, 2)
    BLE(0x0, EVT_SLOT_2, EVT_SLOT_C)
    SVAL(EVT_SLOT_C, 5)
    BLE(0x1, EVT_SLOT_2, EVT_SLOT_C)
    SVAL(EVT_SLOT_C, 8)
    BLE(0x2, EVT_SLOT_2, EVT_SLOT_C)
    GOTO(0xFFFF) /* Should never be reached */

LABEL(0x0)
    SVAL(EVT_SLOT_C, 1)
    BEQ(0x100, EVT_SLOT_1, EVT_SLOT_C)
    SVAL(EVT_SLOT_C, 2)
    BEQ(0x101, EVT_SLOT_1, EVT_SLOT_C)
    GOTO(0xFFFF) /* Should never be reached */

    LABEL(0x100)
        TEXTSHOW(MSG_Poor_Eirika)
        GOTO(0xFFFF)
    LABEL(0x101)
        TEXTSHOW(MSG_Poor_Seth)
        GOTO(0xFFFF)

LABEL(0x1)
    SVAL(EVT_SLOT_C, 1)
    BEQ(0x200, EVT_SLOT_1, EVT_SLOT_C)
    SVAL(EVT_SLOT_C, 2)
    BEQ(0x201, EVT_SLOT_1, EVT_SLOT_C)
    GOTO(0xFFFF) /* Should never be reached */

    LABEL(0x200)
        TEXTSHOW(MSG_Good_Eirika)
        GOTO(0xFFFF)
    LABEL(0x201)
        TEXTSHOW(MSG_Good_Seth)
        GOTO(0xFFFF)

LABEL(0x2)
    SVAL(EVT_SLOT_C, 1)
    BEQ(0x300, EVT_SLOT_1, EVT_SLOT_C)
    SVAL(EVT_SLOT_C, 2)
    BEQ(0x301, EVT_SLOT_1, EVT_SLOT_C)
    GOTO(0xFFFF) /* Should never be reached */

    LABEL(0x300)
        TEXTSHOW(MSG_Great_Eirika)
        GOTO(0xFFFF)
    LABEL(0x301)
        TEXTSHOW(MSG_Great_Seth)
        GOTO(0xFFFF)

LABEL(0xFFFF)
    TEXTEND
    REMA
    NoFade
    ENDA
};

void DisplayCharacterSpeech(struct ManimLevelUpProc *proc)
{
    Proc_Break(proc);
    SetWinEnable(0, 0, 0);
    Proc_EndEach(ProcScr_ManimLevelUpStatGainLabel);

    gEventSlots[EVT_SLOT_1] = gManimSt.actor[proc->actor_id].unit->pCharacterData->number;

    KernelCallEvent(EventScr_LevelUpSpeech, EV_EXEC_CUTSCENE, proc);
};

const struct ProcCmd ProcScr_ManimLevelUp_CUSTOM[] = {
    PROC_SET_END_CB(ManimLevelUp_Clear),
    PROC_SLEEP(1),
    PROC_CALL(InitManimLevelUpWindow),
    PROC_CALL(ManimLevelUp_DimBgm),
    PROC_YIELD,
    PROC_CALL(ManimLevelUp_StartLevelUpText),
    PROC_SLEEP(70),
    PROC_CALL(ManimLevelUp_EndLevelUpText),
    PROC_SLEEP(1),
    PROC_CALL(ManimLevelUp_RestoreBgm),
    PROC_YIELD,
    PROC_CALL(ManimLevelUp_InitMainScreen),
    PROC_YIELD,
    PROC_REPEAT(ManimLevelUp_ScrollIn),
    PROC_SLEEP(30),
    PROC_REPEAT(ManimLevelUp_PutStatGainLabels),
    PROC_SLEEP(30),
#ifdef CONFIG_TALK_LEVEL_UP
    PROC_CALL(DisplayCharacterSpeech), /* My character speech insertion */
#endif
    PROC_SLEEP(20), /* The sleep here is set at 60 by default for the screen after stats are allocated */
    PROC_CALL(EndManimLevelUpStatGainLabels),
    PROC_SLEEP(1),
    PROC_REPEAT(ManimLevelUp_ScrollOut),
    PROC_CALL(ClearManimLevelUpWindow),
    PROC_CALL(ResetDialogueScreen),
    PROC_SLEEP(4),
    PROC_END,
};

LYN_REPLACE_CHECK(ManimLevelUp_InitMainScreen);
void ManimLevelUp_InitMainScreen(struct ManimLevelUpProc *proc)
{
    int i;

    ResetTextFont();
    BG_Fill(gBG0TilemapBuffer, 0);
    PutManimLevelUpFrame(proc->actor_id, 1, 1);

    for (i = 0; i < 9; i++)
        PutManimLevelUpStat(proc->actor_id, 1, 1, i, false);

    BG_EnableSyncByMask(BG0_SYNC_BIT);

    proc->next_stat_num = 0;
    proc->clock = 0;
    proc->y_scroll_offset = -144;

    gLCDControlBuffer.bg0cnt.priority = 0;
    gLCDControlBuffer.bg1cnt.priority = 1;
    gLCDControlBuffer.bg2cnt.priority = 1;
    gLCDControlBuffer.bg3cnt.priority = 2;

    SetDefaultColorEffects();
    SetWinEnable(0, 0, 0);

    /* Level up screen board */
    BG_SetPosition(BG_0, 0, proc->y_scroll_offset);
    /* Level up screen stats */
    BG_SetPosition(BG_1, 0, proc->y_scroll_offset);

/* Turn on portrait blinking and talking */
#ifdef CONFIG_TALK_LEVEL_UP
    StartFace(0, gManimSt.actor[proc->actor_id].unit->pCharacterData->portraitId,
        184, 32 - proc->y_scroll_offset, 0x2);

    /* WHY WON'T YOU TALK?!! */
    SetTalkFaceMouthMove(gManimSt.actor[proc->actor_id].unit->pCharacterData->portraitId);
#else 
    StartFace(0, gManimSt.actor[proc->actor_id].unit->pCharacterData->portraitId,
        184, 32 - proc->y_scroll_offset, 0x1042);
#endif

    gFaces[0]->yPos = 32 - proc->y_scroll_offset;

    // TODO: constants
    StartManimLevelUpStatGainLabels(0x200, 3, 1, proc);
}

LYN_REPLACE_CHECK(ManimLevelUp_ScrollOut);
void ManimLevelUp_ScrollOut(struct ManimLevelUpProc *proc)
{
    proc->y_scroll_offset -= 8;

    BG_SetPosition(BG_0, 0, proc->y_scroll_offset);
    BG_SetPosition(BG_1, 0, proc->y_scroll_offset);

    // NOTE: this is inconsistent with math in ManimLevelUp_InitMainScreen
    // gFaces[0]->yPos = 32 - proc->y_scroll_offset;

    if (proc->y_scroll_offset <= -144)
        Proc_Break(proc);
}
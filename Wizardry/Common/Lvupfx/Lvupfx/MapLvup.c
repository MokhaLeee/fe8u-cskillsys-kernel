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

#ifdef CONFIG_TALK_LEVEL_UP

typedef struct
{
    const int key;
    const char * values[3];
} LevelUpStrings;

static const LevelUpStrings character_level_up_strings[] = 
{ 
    {0},
    { CHARACTER_EIRIKA,    {"I can do better than this!",  "I'm on the right track.", "Wow, what a result!"} },
    { CHARACTER_SETH,      {"There are lessons in failure", "An acceptable result.", "Duty demands perfection."} },
    { CHARACTER_GILLIAM,   {"Even steel falters. I'll try again.", "Slow and steady. As it should be.", "My shield holds firm!"} },
    { CHARACTER_FRANZ,     {"I need to train harder. This won't do.", "Getting better, step by step.", "My training's paying off!"} },
    { CHARACTER_MOULDER,   {"The body is weak, but the spirit endures.", "Modest, but acceptable progress.", "My faith in the divine is renewed!"} },
    { CHARACTER_VANESSA,   {"I won't let this hold me back.", "Small gains, but I'll soar higher.", "Fantastic! I'm free as a bird."} },
    { CHARACTER_ROSS,      {"Aw, come on!", "Not bad!", "Look at me, Dad! I'm unstoppable!"} },
    { CHARACTER_NEIMI,     {"I need to do better...", "A little stronger...", "I can feek it! I'm helping!"} },
    { CHARACTER_COLM,      {"Ugh, this is not my day...", "Decent, I guess.", "Now we're talking!"} },
    { CHARACTER_GARCIA,    {"Even a warrior stumbles sometimes.", "Steady progress.", "Just like the old days!"} },
};

const int dict_size_level_up_strings = sizeof(character_level_up_strings) / sizeof(character_level_up_strings[0]);

void DisplayCharacterSpeech(struct ManimLevelUpProc *proc)
{
    int message = -1;
    int unitID = gManimSt.actor[proc->actor_id].unit->pCharacterData->number;

    if (gEventSlots[2] <= 2) 
        message = 0;
    else if (gEventSlots[2] <= 5) 
        message = 1;
    else if (gEventSlots[2] <= 8) 
        message = 2;

    PutStringCentered(TILEMAP_LOCATED(gBG0TilemapBuffer, 10, 30), TEXT_COLOR_SYSTEM_WHITE, 20, character_level_up_strings[unitID].values[message]);
};

#endif

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
#ifdef CONFIG_TALK_LEVEL_UP
    PROC_SLEEP(15),
    PROC_REPEAT(ManimLevelUp_PutStatGainLabels),
    PROC_SLEEP(15),
    PROC_CALL(DisplayCharacterSpeech), /* My character speech insertion */
    PROC_SLEEP(45),
#else
    PROC_SLEEP(30),
    PROC_REPEAT(ManimLevelUp_PutStatGainLabels),
    PROC_SLEEP(60),
#endif
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

/* Change the unit's expression during level up depending on their stat gains */
if (gEventSlots[EVT_SLOT_2] <= 2) 
    StartFace(0, gManimSt.actor[proc->actor_id].unit->pCharacterData->portraitId,
        184, 32 - proc->y_scroll_offset, FACE_DISP_KIND(FACE_96x80));
else if (gEventSlots[EVT_SLOT_2] <= 5) 
    StartFace(0, gManimSt.actor[proc->actor_id].unit->pCharacterData->portraitId,
        184, 32 - proc->y_scroll_offset, FACE_DISP_KIND(FACE_96x80));
else if (gEventSlots[EVT_SLOT_2 <= 8])
    StartFace(0, gManimSt.actor[proc->actor_id].unit->pCharacterData->portraitId,
        184, 32 - proc->y_scroll_offset, FACE_DISP_KIND(FACE_96x80) | FACE_DISP_SMILE);

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
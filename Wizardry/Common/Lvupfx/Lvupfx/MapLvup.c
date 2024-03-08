#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"
#include "bmlib.h"
#include "hardware.h"
#include "mapanim.h"
#include "constants/video-global.h"

#include "common-chax.h"
#include "strmag.h"
#include "lvup.h"

/* LynJump */
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

/* LynJump */
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

/* LynJump */
void PutManimLevelUpFrame(int actor_id, int x, int y)
{
    int i;

    BG_Fill(gBG1TilemapBuffer, 0);

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

/* LynJump */
void PutManimLevelUpStat(int actor_id, int x, int y, int stat_num, bool after_gain)
{
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG0TilemapBuffer, x + NewMapLvupInfos[stat_num].x + 4, y + NewMapLvupInfos[stat_num].y),
        TEXT_COLOR_SYSTEM_BLUE,
        GetManimLevelUpBaseStat(actor_id, stat_num) + (after_gain ? GetManimLevelUpStatGain(actor_id, stat_num) : 0));
}

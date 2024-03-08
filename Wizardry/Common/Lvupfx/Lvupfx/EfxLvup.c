#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"
#include "hardware.h"
#include "anime.h"
#include "fontgrp.h"
#include "ekrlevelup.h"
#include "ekrbattle.h"
#include "efxbattle.h"

#include "common-chax.h"
#include "strmag.h"
#include "lvup.h"

struct EfxLvupInfo {
    u8 x, y;
    u16 msg;
};

STATIC_DECLAR const struct EfxLvupInfo NewEfxLvupInfos[] = {
    {0x02, 0x0B, 0x4E9}, /* HP */
    {0x02, 0x0D, 0x4FE}, /* Str */
    {0x02, 0x0F, 0x4FF}, /* Mag */
    {0x02, 0x11, 0x4EE}, /* Lck */

    {0x0A, 0x0B, 0x4EC}, /* Skl */
    {0x0A, 0x0D, 0x4ED}, /* Spd */
    {0x0A, 0x0F, 0x4EF}, /* Def */
    {0x0A, 0x11, 0x4F0}, /* Res */

    {-1, -1, 0}
};

STATIC_DECLAR void EkrLvup_InitStatusTextVanilla(struct ProcEkrLevelup * proc)
{
    int i;
    struct Text * th, * th_base = gBanimText + EKRLVUP_STAT_MAX;

    for (i = 0; i < EKRLVUP_STAT_MAX; i++)
    {
        InitText(&th_base[i], 2);
        Text_SetCursor(&th_base[i], 8);
        Text_SetColor(&th_base[i], TEXT_COLOR_SYSTEM_BLUE);
        Text_DrawNumber(&th_base[i], gEkrLvupBaseStatus[i]);
        PutText(&th_base[i], gBG2TilemapBuffer + 3 + sEfxLvupPartsPos[i]);
    }

    /* class */
    th = th_base + EKRLVUP_STAT_CLASS;
    InitText(th, 8);
    Text_DrawString(th, GetStringFromIndex(gpEkrLvupUnit->pClassData->nameTextId));
    PutText(th, TILEMAP_LOCATED(gBG2TilemapBuffer, 3, 7));

    /* level msg */
    th = th_base + EKRLVUP_STAT_LV_MSG;
    InitText(th, 3);
    Text_SetColor(th, TEXT_COLOR_SYSTEM_GOLD);
    Text_DrawString(th, GetStringFromIndex(0x4E7));
    PutText(th, TILEMAP_LOCATED(gBG2TilemapBuffer, 10, 7));

    /* level value */
    th = th_base + EKRLVUP_STAT_LV_VAL;
    InitText(th, 2);
    Text_SetCursor(th, 8);
    Text_SetColor(th, TEXT_COLOR_SYSTEM_BLUE);
    Text_DrawNumber(th, gEkrLvupPreLevel);
    PutText(th, TILEMAP_LOCATED(gBG2TilemapBuffer, 13, 7));
}

/* LynJump */
void EkrLvup_InitStatusText(struct ProcEkrLevelup * proc)
{
    int i;
    struct BattleUnit * bunit, * bunit2;
    struct Unit * unit;

    if (proc->ais_main == NULL)
    {
        bunit2 = gpEkrBattleUnitLeft;
        gpEkrLvupUnit = unit = &bunit2->unit;
        gpEkrLvupBattleUnit = bunit = gpEkrBattleUnitRight;
    }
    else
    {
        bunit2 = gpEkrBattleUnitRight;
        gpEkrLvupUnit = unit = &bunit2->unit;
        gpEkrLvupBattleUnit = bunit = gpEkrBattleUnitLeft;
    }

    if (proc->is_promotion == false)
    {
        unit = GetUnit(unit->index);

        gEkrLvupPreLevel = bunit2->levelPrevious;
        gEkrLvupBaseStatus[0] = unit->maxHP;
        gEkrLvupBaseStatus[1] = unit->pow;
        gEkrLvupBaseStatus[2] = UNIT_MAG(unit);
        gEkrLvupBaseStatus[3] = unit->lck;
        gEkrLvupBaseStatus[4] = unit->skl;
        gEkrLvupBaseStatus[5] = unit->spd;
        gEkrLvupBaseStatus[6] = unit->def;
        gEkrLvupBaseStatus[7] = unit->res;
        gEkrLvupPostLevel = bunit2->levelPrevious + 1;

        gEkrLvupPostStatus[0] = unit->maxHP + bunit2->changeHP;
        gEkrLvupPostStatus[1] = unit->pow + bunit2->changePow;
        gEkrLvupPostStatus[2] = UNIT_MAG(unit) + BU_CHG_MAG(bunit2);
        gEkrLvupPostStatus[3] = unit->lck + bunit2->changeLck;
        gEkrLvupPostStatus[4] = unit->skl + bunit2->changeSkl;
        gEkrLvupPostStatus[5] = unit->spd + bunit2->changeSpd;
        gEkrLvupPostStatus[6] = unit->def + bunit2->changeDef;
        gEkrLvupPostStatus[7] = unit->res + bunit2->changeRes;
    }
    else
    {
        gEkrLvupPreLevel = unit->level;
        gEkrLvupBaseStatus[0] = unit->maxHP;
        gEkrLvupBaseStatus[1] = unit->pow;
        gEkrLvupBaseStatus[2] = UNIT_MAG(unit);
        gEkrLvupBaseStatus[3] = unit->lck;
        gEkrLvupBaseStatus[4] = unit->skl;
        gEkrLvupBaseStatus[5] = unit->spd;
        gEkrLvupBaseStatus[6] = unit->def;
        gEkrLvupBaseStatus[7] = unit->res;
        gEkrLvupPostLevel = 1;

        gEkrLvupPostStatus[0] = bunit->unit.maxHP;
        gEkrLvupPostStatus[1] = bunit->unit.pow;
        gEkrLvupPostStatus[2] = UNIT_MAG(&bunit->unit);
        gEkrLvupPostStatus[3] = bunit->unit.lck;
        gEkrLvupPostStatus[4] = bunit->unit.skl;
        gEkrLvupPostStatus[5] = bunit->unit.spd;
        gEkrLvupPostStatus[6] = bunit->unit.def;
        gEkrLvupPostStatus[7] = bunit->unit.res;
    }

    InitTextFont(&gBanimFont, BG_CHR_ADDR(0x146), 0x146, 0);

    for (i = 0; i < EKRLVUP_STAT_MAX; i++)
    {
        const char * str = GetStringFromIndex(NewEfxLvupInfos[i].msg);

        InitText(&gBanimText[i], 3);
        Text_SetCursor(&gBanimText[i], 0);
        Text_SetColor(&gBanimText[i], TEXT_COLOR_SYSTEM_GOLD);
        Text_DrawString(&gBanimText[i], str);
        PutText(&gBanimText[i], gBG2TilemapBuffer + sEfxLvupPartsPos[i]);
    }

    EkrLvup_InitStatusTextVanilla(proc);
}

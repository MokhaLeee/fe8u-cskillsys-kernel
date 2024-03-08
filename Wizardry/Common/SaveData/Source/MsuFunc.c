#include "common-chax.h"
#include "bmunit.h"
#include "bmsave.h"
#include "sram-layout.h"
#include "uimenu.h"
#include "bmmind.h"
#include "event.h"
#include "bmmind.h"

#include "debug-kit.h"
#include "save-data.h"
#include "strmag.h"

void MSU_SavePlaySt(u8 * dst, const u32 size)
{
    gPlaySt.time_saved = GetGameClock();
    WriteAndVerifySramFast(&gPlaySt, dst, sizeof(gPlaySt));
}

void MSU_LoadPlaySt(u8 * src, const u32 size)
{
    ReadSramFast(src, &gPlaySt, sizeof(gPlaySt));
    SetGameTime(gPlaySt.time_saved);
}

void MSU_SaveAction(u8 * dst, const u32 size)
{
    StoreRNStateToActionStruct();
    WriteAndVerifySramFast(&gActionData, dst, sizeof(struct ActionData));
}

void MSU_LoadAction(u8 * src, const u32 size)
{
    ReadSramFast(src, &gActionData, sizeof(struct ActionData));
    LoadRNStateFromActionStruct();
}

void MSU_SaveMenuMask(u8 * dst, const u32 size)
{
    u8 list[MENU_OVERRIDE_MAX];
    GetForceDisabledMenuItems(list);
    WriteAndVerifySramFast(list, dst, sizeof(list));
}

void MSU_LoadMenuMask(u8 * src, const u32 size)
{
    u8 list[MENU_OVERRIDE_MAX];
    ReadSramFast(src, list, sizeof(list));
    SetForceDisabledMenuItems(list);
}

void MSU_SaveWorldMap(u8 * dst, const u32 size)
{
    WriteWorldMapStuff(dst, &gGMData);
}

void MSU_LoadWorldMap(u8 * src, const u32 size)
{
    ReadWorldMapStuff(src, &gGMData);
}

void MSU_SaveDungeon(u8 * dst, const u32 size)
{
    struct Dungeon dungeon[2];
    SaveDungeonRecords(dungeon);
    WriteAndVerifySramFast(dungeon, dst, sizeof(dungeon));
}

void MSU_LoadDungeon(u8 * src, const u32 size)
{
    struct Dungeon dungeon[2];
    ReadSramFast(src, dungeon, sizeof(dungeon));
    LoadDungeonRecords(dungeon);
}

void MSU_SaveDungeon2(u8 * dst, const u32 size)
{
    struct Dungeon dungeon;
    SaveDungeonState(&dungeon);
    WriteAndVerifySramFast(&dungeon, dst, sizeof(struct Dungeon));
}

void MSU_LoadDungeon2(u8 * src, const u32 size)
{
    struct Dungeon dungeon;
    ReadSramFast(src, &dungeon, sizeof(struct Dungeon));
    LoadDungeonState(&dungeon);
}

void MSU_SaveEvtCounter(u8 * dst, const u32 size)
{
    int val = GetEventSlotCounter();
    WriteAndVerifySramFast(&val, dst, sizeof(int));
}

void MSU_LoadEvtCounter(u8 * src, const u32 size)
{
    int val;
    ReadSramFast(src, &val, sizeof(int));
    SetEventSlotCounter(val);
}

void MSU_SaveBonusClaim(void)
{
    return;
}

void MSU_LoadBonusClaimWIP(void)
{
    u32 buf;
    void * src = GetSaveReadAddr(gPlaySt.gameSaveSlot);
    ReadSramFast(src + 0x0638, &buf, sizeof(buf));  /* 0x0638 is countered from gEmsSavChunks */
    SetBonusContentClaimFlags(buf);
}

static void NewPackSuspandUnit(struct Unit * src, struct EmsPackedSusUnit * dst)
{
    int i;
    struct Unit tmp_unit;

    if (!dst)
        return;

    if (!src || !UNIT_IS_VALID(src))
    {
        ClearUnit(&tmp_unit);
        src = &tmp_unit;
        dst->jid = 0;
        dst->pid = 0;
    }
    else
    {
        dst->jid = UNIT_CLASS_ID(src);
        dst->pid = UNIT_CHAR_ID(src);
    }

    dst->max_hp = src->maxHP;
    dst->pow = src->pow;
    dst->mag = UNIT_MAG(src);
    dst->skl = src->skl;
    dst->spd = src->spd;
    dst->lck = src->lck;
    dst->def = src->def;
    dst->res = src->res;
    dst->mov = src->movBonus;
    dst->con = src->conBonus;
    dst->level = src->level;
    dst->exp = src->exp;
    dst->xPos = src->xPos;
    dst->yPos = src->yPos;

    for (i = 0; i < 8; i++)
        dst->ranks[i] = src->ranks[i];

    for (i = 0; i < 0x5; i++)
        dst->items[i] = src->items[i];

    dst->state = src->state;
    dst->cur_hp = src->curHP;
    dst->rescue = src->rescue;
    dst->ballista = src->ballistaIndex;
    dst->status = src->statusIndex;
    dst->duration = src->statusDuration;
    dst->torch = src->torchDuration;
    dst->barrier = src->barrierDuration;

    if (UNIT_FACTION(src) == FACTION_BLUE)
    {
        for (i = 0; i < 7; i++)
            dst->pad.ally.skills[i] = src->supports[i];

        dst->pad.ally.support_gain = src->supportBits;
    }
    else
    {
        dst->pad.ai.skill = src->supports[0];
        dst->pad.ai.ai1 = src->ai1;
        dst->pad.ai.ai1_cur = src->ai1data;
        dst->pad.ai.ai2 = src->ai2;
        dst->pad.ai.ai2_cur = src->ai2data;
        dst->pad.ai.ai_flag = src->aiFlags;
        dst->pad.ai.ai_config = src->ai3And4;
    }
    dst->_u3A = src->_u3A;
    dst->_u3B = src->_u3B;
}

static void NewUnpackSuspandUnit(struct EmsPackedSusUnit * src, struct Unit * dst)
{
    int i;

    if (!src || !dst)
        return;

    dst->pCharacterData = GetCharacterData(src->pid);
    dst->pClassData = GetClassData(src->jid);
    dst->maxHP = src->max_hp;
    dst->pow = src->pow;
    UNIT_MAG(dst) = src->mag;
    dst->skl = src->skl;
    dst->spd = src->spd;
    dst->lck = src->lck;
    dst->def = src->def;
    dst->res = src->res;
    dst->movBonus = src->mov;
    dst->conBonus = src->con;
    dst->level = src->level;
    dst->exp = src->exp;
    dst->xPos = src->xPos;
    dst->yPos = src->yPos;

    for (i = 0; i < 8; i++)
        dst->ranks[i] = src->ranks[i];

    dst->curHP = src->cur_hp;
    dst->rescue = src->rescue;
    dst->ballistaIndex = src->ballista;
    dst->statusIndex = src->status;
    dst->statusDuration = src->duration;
    dst->torchDuration = src->torch;
    dst->barrierDuration = src->barrier;

    if (UNIT_FACTION(dst) == FACTION_BLUE)
    {
        for (i = 0; i < 7; i++)
            dst->supports[i] = src->pad.ally.skills[i];

        dst->supportBits = src->pad.ally.support_gain;
    }
    else
    {
        dst->supports[0] = src->pad.ai.skill;
        dst->ai1 = src->pad.ai.ai1;
        dst->ai1data = src->pad.ai.ai1_cur;
        dst->ai2 = src->pad.ai.ai2;
        dst->ai2data = src->pad.ai.ai2_cur;
        dst->aiFlags = src->pad.ai.ai_flag;
        dst->ai3And4 = src->pad.ai.ai_config;
    }

    dst->state = src->state;

    dst->_u3A = src->_u3A;
    dst->_u3B = src->_u3B;

    for (i = 0; i < 0x5; i++)
        dst->items[i] = src->items[i];

    if (dst->exp > 99)
        dst->exp = -1;

    if (0x3F == dst->xPos)
        dst->xPos = -1;

    if (0x3F == dst->yPos)
        dst->yPos = -1;
}

void MSU_SaveBlueUnits(u8 * dst, const u32 size)
{
    int i, amt = size / SIZE_OF_SUS_UNIT_PACK;

    for (i = 0; i < amt; i++)
    {
        struct EmsPackedSusUnit pack;

        NewPackSuspandUnit(&gUnitArrayBlue[i], &pack);
        WriteAndVerifySramFast(&pack, dst, SIZE_OF_SUS_UNIT_PACK);
        dst += SIZE_OF_SUS_UNIT_PACK;
    }
}

void MSU_LoadBlueUnits(u8 * src, const u32 size)
{
    int i, amt = size / SIZE_OF_SUS_UNIT_PACK;

    for (i = 0; i < amt; i++)
    {
        struct EmsPackedSusUnit pack;

        ReadSramFast(src, &pack, SIZE_OF_SUS_UNIT_PACK);
        NewUnpackSuspandUnit(&pack, &gUnitArrayBlue[i]);
        src += SIZE_OF_SUS_UNIT_PACK;
    }
}

void MSU_SaveRedUnits(u8 * dst, const u32 size)
{
    int i, amt = size / SIZE_OF_SUS_UNIT_PACK;

    for (i = 0; i < amt; i++)
    {
        struct EmsPackedSusUnit pack;

        NewPackSuspandUnit(&gUnitArrayRed[i], &pack);
        WriteAndVerifySramFast(&pack, dst, SIZE_OF_SUS_UNIT_PACK);
        dst += SIZE_OF_SUS_UNIT_PACK;
    }
}

void MSU_LoadRedUnits(u8 * src, const u32 size)
{
    int i, amt = size / SIZE_OF_SUS_UNIT_PACK;

    for (i = 0; i < amt; i++)
    {
        struct EmsPackedSusUnit pack;

        ReadSramFast(src, &pack, SIZE_OF_SUS_UNIT_PACK);
        NewUnpackSuspandUnit(&pack, &gUnitArrayRed[i]);
        src += SIZE_OF_SUS_UNIT_PACK;
    }
}

void MSU_SaveGreenUnits(u8 * dst, const u32 size)
{
    int i, amt = size / SIZE_OF_SUS_UNIT_PACK;

    for (i = 0; i < amt; i++)
    {
        struct EmsPackedSusUnit pack;

        NewPackSuspandUnit(&gUnitArrayGreen[i], &pack);
        WriteAndVerifySramFast(&pack, dst, SIZE_OF_SUS_UNIT_PACK);
        dst += SIZE_OF_SUS_UNIT_PACK;
    }
}

void MSU_LoadGreenUnits(u8 * src, const u32 size)
{
    int i, amt = size / SIZE_OF_SUS_UNIT_PACK;

    for (i = 0; i < amt; i++)
    {
        struct EmsPackedSusUnit pack;

        ReadSramFast(src, &pack, SIZE_OF_SUS_UNIT_PACK);
        NewUnpackSuspandUnit(&pack, &gUnitArrayGreen[i]);
        src += SIZE_OF_SUS_UNIT_PACK;
    }
}

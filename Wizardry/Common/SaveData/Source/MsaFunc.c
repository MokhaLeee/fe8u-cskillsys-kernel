#include "common-chax.h"
#include "bmunit.h"
#include "bmsave.h"
#include "sram-layout.h"

#include "debug-kit.h"
#include "save-data.h"
#include "strmag.h"

void MSA_SavePlaySt(u8 * dst, const u32 size)
{
    gPlaySt.time_saved = GetGameClock();
    WriteAndVerifySramFast(&gPlaySt, dst, sizeof(gPlaySt));
}

void MSA_LoadPlaySt(u8 * src, const u32 size)
{
    ReadSramFast(src, &gPlaySt, sizeof(gPlaySt));
    SetGameTime(gPlaySt.time_saved);
}

void MSA_SaveBonusClaim(u8 * dst, const u32 size)
{
    WriteAndVerifySramFast(
        &gBonusContentClaimFlags,
        dst,
        sizeof(gBonusContentClaimFlags));
}

void MSA_LoadBonusClaim(u8 * src, const u32 size)
{
    ReadSramFast(
        src,
        &gBonusContentClaimFlags,
        sizeof(gBonusContentClaimFlags));
}

void MSA_SaveWorldMap(u8 * dst, const u32 size)
{
    WriteWorldMapStuff(dst, &gGMData);
}

void MSA_LoadWorldMap(u8 * src, const u32 size)
{
    ReadWorldMapStuff(src, &gGMData);
}

void MSA_SaveDungeon(u8 * dst, const u32 size)
{
    struct Dungeon dungeon[2];
    SaveDungeonRecords(dungeon);
    WriteAndVerifySramFast(dungeon, dst, sizeof(dungeon));
}

void MSA_LoadDungeon(u8 * src, const u32 size)
{
    struct Dungeon dungeon[2];
    ReadSramFast(src, dungeon, sizeof(dungeon));
    LoadDungeonRecords(dungeon);
}

static void NewPackSaveUnit(struct Unit * src, struct EmsPackedSavUnit * dst)
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

    /* With BWL support patch */
    for (i = 0; i < 7; i++)
        dst->skills[i] = src->supports[i];

    for (i = 0; i < 0x5; i++)
        dst->items[i] = src->items[i];

    dst->state = src->state;
}

static void NewUnpackSaveUnit(struct EmsPackedSavUnit * src, struct Unit * dst)
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

    for (i = 0; i < 0x5; i++)
        dst->items[i] = src->items[i];

    dst->state = src->state;

    SetUnitHp(dst, GetUnitMaxHp(dst));
    dst->supportBits = 0;

    if (dst->exp > 99)
        dst->exp = -1;

    if (0x3F == dst->xPos)
        dst->xPos = -1;

    if (0x3F == dst->yPos)
        dst->yPos = -1;
}

void MSA_SaveUnits(u8 * dst, const u32 size)
{
    int i, amt = size / sizeof(struct EmsPackedSavUnit);

    for (i = 0; i < amt; i++)
    {
        struct EmsPackedSavUnit pack;

        NewPackSaveUnit(&gUnitArrayBlue[i], &pack);
        WriteAndVerifySramFast(&pack, dst, sizeof(pack));
        dst += sizeof(pack);
    }
}

void MSA_LoadUnits(u8 * src, const u32 size)
{
    int i, amt = size / sizeof(struct EmsPackedSavUnit);

    for (i = 0; i < amt; i++)
    {
        struct EmsPackedSavUnit pack;

        ReadSramFast(src, &pack, sizeof(pack));
        NewUnpackSaveUnit(&pack, &gUnitArrayBlue[i]);
        src += sizeof(pack);
    }
}

const size_t gSize_EmsPackedSavUnit = sizeof(struct EmsPackedSavUnit);

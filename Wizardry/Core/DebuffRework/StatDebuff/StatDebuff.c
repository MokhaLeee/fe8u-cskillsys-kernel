#include "common-chax.h"
#include "debuff.h"

extern u32 sStatDebuffStatusAlly[CONFIG_UNIT_AMT_ALLY];
extern u32 sStatDebuffStatusEnemy[CONFIG_UNIT_AMT_ENEMY];
extern u32 sStatDebuffStatusNpc[CONFIG_UNIT_AMT_NPC];

static u32 * const sStatDebuffStatusPool[0x100] = {
    [FACTION_BLUE + 0x01] = sStatDebuffStatusAlly + 0,
    [FACTION_BLUE + 0x02] = sStatDebuffStatusAlly + 1,
    [FACTION_BLUE + 0x03] = sStatDebuffStatusAlly + 2,
    [FACTION_BLUE + 0x04] = sStatDebuffStatusAlly + 3,
    [FACTION_BLUE + 0x05] = sStatDebuffStatusAlly + 4,
    [FACTION_BLUE + 0x06] = sStatDebuffStatusAlly + 5,
    [FACTION_BLUE + 0x07] = sStatDebuffStatusAlly + 6,
    [FACTION_BLUE + 0x08] = sStatDebuffStatusAlly + 7,
    [FACTION_BLUE + 0x09] = sStatDebuffStatusAlly + 8,
    [FACTION_BLUE + 0x0A] = sStatDebuffStatusAlly + 9,
    [FACTION_BLUE + 0x0B] = sStatDebuffStatusAlly + 10,
    [FACTION_BLUE + 0x0C] = sStatDebuffStatusAlly + 11,
    [FACTION_BLUE + 0x0D] = sStatDebuffStatusAlly + 12,
    [FACTION_BLUE + 0x0E] = sStatDebuffStatusAlly + 13,
    [FACTION_BLUE + 0x0F] = sStatDebuffStatusAlly + 14,
    [FACTION_BLUE + 0x10] = sStatDebuffStatusAlly + 15,
    [FACTION_BLUE + 0x11] = sStatDebuffStatusAlly + 16,
    [FACTION_BLUE + 0x12] = sStatDebuffStatusAlly + 17,
    [FACTION_BLUE + 0x13] = sStatDebuffStatusAlly + 18,
    [FACTION_BLUE + 0x14] = sStatDebuffStatusAlly + 19,
    [FACTION_BLUE + 0x15] = sStatDebuffStatusAlly + 20,
    [FACTION_BLUE + 0x16] = sStatDebuffStatusAlly + 21,
    [FACTION_BLUE + 0x17] = sStatDebuffStatusAlly + 22,
    [FACTION_BLUE + 0x18] = sStatDebuffStatusAlly + 23,
    [FACTION_BLUE + 0x19] = sStatDebuffStatusAlly + 24,
    [FACTION_BLUE + 0x1A] = sStatDebuffStatusAlly + 25,
    [FACTION_BLUE + 0x1B] = sStatDebuffStatusAlly + 26,
    [FACTION_BLUE + 0x1C] = sStatDebuffStatusAlly + 27,
    [FACTION_BLUE + 0x1D] = sStatDebuffStatusAlly + 28,
    [FACTION_BLUE + 0x1E] = sStatDebuffStatusAlly + 29,
    [FACTION_BLUE + 0x1F] = sStatDebuffStatusAlly + 30,
    [FACTION_BLUE + 0x20] = sStatDebuffStatusAlly + 31,
    [FACTION_BLUE + 0x21] = sStatDebuffStatusAlly + 32,
    [FACTION_BLUE + 0x22] = sStatDebuffStatusAlly + 33,
    [FACTION_BLUE + 0x23] = sStatDebuffStatusAlly + 34,
    [FACTION_BLUE + 0x24] = sStatDebuffStatusAlly + 35,
    [FACTION_BLUE + 0x25] = sStatDebuffStatusAlly + 36,
    [FACTION_BLUE + 0x26] = sStatDebuffStatusAlly + 37,
    [FACTION_BLUE + 0x27] = sStatDebuffStatusAlly + 38,
    [FACTION_BLUE + 0x28] = sStatDebuffStatusAlly + 39,
    [FACTION_BLUE + 0x29] = sStatDebuffStatusAlly + 40,
    [FACTION_BLUE + 0x2A] = sStatDebuffStatusAlly + 41,
    [FACTION_BLUE + 0x2B] = sStatDebuffStatusAlly + 42,
    [FACTION_BLUE + 0x2C] = sStatDebuffStatusAlly + 43,
    [FACTION_BLUE + 0x2D] = sStatDebuffStatusAlly + 44,
    [FACTION_BLUE + 0x2E] = sStatDebuffStatusAlly + 45,
    [FACTION_BLUE + 0x2F] = sStatDebuffStatusAlly + 46,
    [FACTION_BLUE + 0x30] = sStatDebuffStatusAlly + 47,
    [FACTION_BLUE + 0x31] = sStatDebuffStatusAlly + 48,
    [FACTION_BLUE + 0x32] = sStatDebuffStatusAlly + 49,
    [FACTION_BLUE + 0x33] = sStatDebuffStatusAlly + 50,
    [FACTION_BLUE + 0x34] = sStatDebuffStatusAlly + 51,

    [FACTION_RED + 0x01] = sStatDebuffStatusEnemy + 0,
    [FACTION_RED + 0x02] = sStatDebuffStatusEnemy + 1,
    [FACTION_RED + 0x03] = sStatDebuffStatusEnemy + 2,
    [FACTION_RED + 0x04] = sStatDebuffStatusEnemy + 3,
    [FACTION_RED + 0x05] = sStatDebuffStatusEnemy + 4,
    [FACTION_RED + 0x06] = sStatDebuffStatusEnemy + 5,
    [FACTION_RED + 0x07] = sStatDebuffStatusEnemy + 6,
    [FACTION_RED + 0x08] = sStatDebuffStatusEnemy + 7,
    [FACTION_RED + 0x09] = sStatDebuffStatusEnemy + 8,
    [FACTION_RED + 0x0A] = sStatDebuffStatusEnemy + 9,
    [FACTION_RED + 0x0B] = sStatDebuffStatusEnemy + 10,
    [FACTION_RED + 0x0C] = sStatDebuffStatusEnemy + 11,
    [FACTION_RED + 0x0D] = sStatDebuffStatusEnemy + 12,
    [FACTION_RED + 0x0E] = sStatDebuffStatusEnemy + 13,
    [FACTION_RED + 0x0F] = sStatDebuffStatusEnemy + 14,
    [FACTION_RED + 0x10] = sStatDebuffStatusEnemy + 15,
    [FACTION_RED + 0x11] = sStatDebuffStatusEnemy + 16,
    [FACTION_RED + 0x12] = sStatDebuffStatusEnemy + 17,
    [FACTION_RED + 0x13] = sStatDebuffStatusEnemy + 18,
    [FACTION_RED + 0x14] = sStatDebuffStatusEnemy + 19,
    [FACTION_RED + 0x15] = sStatDebuffStatusEnemy + 20,
    [FACTION_RED + 0x16] = sStatDebuffStatusEnemy + 21,
    [FACTION_RED + 0x17] = sStatDebuffStatusEnemy + 22,
    [FACTION_RED + 0x18] = sStatDebuffStatusEnemy + 23,
    [FACTION_RED + 0x19] = sStatDebuffStatusEnemy + 24,
    [FACTION_RED + 0x1A] = sStatDebuffStatusEnemy + 25,
    [FACTION_RED + 0x1B] = sStatDebuffStatusEnemy + 26,
    [FACTION_RED + 0x1C] = sStatDebuffStatusEnemy + 27,
    [FACTION_RED + 0x1D] = sStatDebuffStatusEnemy + 28,
    [FACTION_RED + 0x1E] = sStatDebuffStatusEnemy + 29,
    [FACTION_RED + 0x1F] = sStatDebuffStatusEnemy + 30,
    [FACTION_RED + 0x20] = sStatDebuffStatusEnemy + 31,
    [FACTION_RED + 0x21] = sStatDebuffStatusEnemy + 32,
    [FACTION_RED + 0x22] = sStatDebuffStatusEnemy + 33,
    [FACTION_RED + 0x23] = sStatDebuffStatusEnemy + 34,
    [FACTION_RED + 0x24] = sStatDebuffStatusEnemy + 35,
    [FACTION_RED + 0x25] = sStatDebuffStatusEnemy + 36,
    [FACTION_RED + 0x26] = sStatDebuffStatusEnemy + 37,
    [FACTION_RED + 0x27] = sStatDebuffStatusEnemy + 38,
    [FACTION_RED + 0x28] = sStatDebuffStatusEnemy + 39,
    [FACTION_RED + 0x29] = sStatDebuffStatusEnemy + 40,
    [FACTION_RED + 0x2A] = sStatDebuffStatusEnemy + 41,
    [FACTION_RED + 0x2B] = sStatDebuffStatusEnemy + 42,
    [FACTION_RED + 0x2C] = sStatDebuffStatusEnemy + 43,
    [FACTION_RED + 0x2D] = sStatDebuffStatusEnemy + 44,
    [FACTION_RED + 0x2E] = sStatDebuffStatusEnemy + 45,
    [FACTION_RED + 0x2F] = sStatDebuffStatusEnemy + 46,
    [FACTION_RED + 0x30] = sStatDebuffStatusEnemy + 47,
    [FACTION_RED + 0x31] = sStatDebuffStatusEnemy + 48,
    [FACTION_RED + 0x32] = sStatDebuffStatusEnemy + 49,

    [FACTION_GREEN + 0x01] = sStatDebuffStatusNpc + 0,
    [FACTION_GREEN + 0x02] = sStatDebuffStatusNpc + 1,
    [FACTION_GREEN + 0x03] = sStatDebuffStatusNpc + 2,
    [FACTION_GREEN + 0x04] = sStatDebuffStatusNpc + 3,
    [FACTION_GREEN + 0x05] = sStatDebuffStatusNpc + 4,
    [FACTION_GREEN + 0x06] = sStatDebuffStatusNpc + 5,
    [FACTION_GREEN + 0x07] = sStatDebuffStatusNpc + 6,
    [FACTION_GREEN + 0x08] = sStatDebuffStatusNpc + 7,
    [FACTION_GREEN + 0x09] = sStatDebuffStatusNpc + 8,
    [FACTION_GREEN + 0x0A] = sStatDebuffStatusNpc + 9,
    [FACTION_GREEN + 0x0B] = sStatDebuffStatusNpc + 10,
    [FACTION_GREEN + 0x0C] = sStatDebuffStatusNpc + 11,
    [FACTION_GREEN + 0x0D] = sStatDebuffStatusNpc + 12,
    [FACTION_GREEN + 0x0E] = sStatDebuffStatusNpc + 13,
    [FACTION_GREEN + 0x0F] = sStatDebuffStatusNpc + 14,
    [FACTION_GREEN + 0x10] = sStatDebuffStatusNpc + 15,
    [FACTION_GREEN + 0x11] = sStatDebuffStatusNpc + 16,
    [FACTION_GREEN + 0x12] = sStatDebuffStatusNpc + 17,
    [FACTION_GREEN + 0x13] = sStatDebuffStatusNpc + 18,
    [FACTION_GREEN + 0x14] = sStatDebuffStatusNpc + 19,
};

static inline u32 * GetUnitStatDebuffStatus(struct Unit * unit)
{
    return sStatDebuffStatusPool[unit->index & 0xFF];
}

static inline void _BIT_SET(u32 * bits, int idx)
{
    bits[idx / 32] |= 1 << (idx % 32);
}

static inline void _BIT_CLR(u32 * bits, int idx)
{
    bits[idx / 32] &= ~(1 << (idx % 32));
}

static inline bool _BIT_CHK(u32 * bits, int idx)
{
    if (bits[idx / 32] & (1 << (idx % 32)))
        return true;

    return false;
}

void SetUnitStatDebuff(struct Unit * unit, enum UNIT_STAT_DEBUFF_IDX debuff)
{
    if (debuff >= UNIT_STAT_DEBUFF_MAX)
    {
        Errorf("ENOTDIR: %d", debuff);
        abort();
    }
    _BIT_SET(GetUnitStatDebuffStatus(unit), debuff);
}

void ClearUnitStatDebuff(struct Unit * unit, enum UNIT_STAT_DEBUFF_IDX debuff)
{
    if (debuff >= UNIT_STAT_DEBUFF_MAX)
    {
        Errorf("ENOTDIR: %d", debuff);
        abort();
    }
    _BIT_CLR(GetUnitStatDebuffStatus(unit), debuff);
}

bool CheckUnitStatDebuff(struct Unit * unit, enum UNIT_STAT_DEBUFF_IDX debuff)
{
    if (debuff >= UNIT_STAT_DEBUFF_MAX)
    {
        Errorf("ENOTDIR: %d", debuff);
        abort();
    }
    return _BIT_CHK(GetUnitStatDebuffStatus(unit), debuff);
}

void MSU_SaveStatDebuff(u8 * dst, const u32 size)
{
    if (size < (sizeof(sStatDebuffStatusAlly) + sizeof(sStatDebuffStatusEnemy) + sizeof(sStatDebuffStatusNpc)))
    {
        Errorf("ENOMEM: %d", size);
        abort();
    }

    WriteAndVerifySramFast(
        sStatDebuffStatusAlly,
        dst,
        sizeof(sStatDebuffStatusAlly));

    dst += sizeof(sStatDebuffStatusAlly);

    WriteAndVerifySramFast(
        sStatDebuffStatusEnemy,
        dst,
        sizeof(sStatDebuffStatusEnemy));

    dst += sizeof(sStatDebuffStatusEnemy);

    WriteAndVerifySramFast(
        sStatDebuffStatusNpc,
        dst,
        sizeof(sStatDebuffStatusNpc));
}

void MSU_LoadStatDebuff(u8 * src, const u32 size)
{
    if (size < (sizeof(sStatDebuffStatusAlly) + sizeof(sStatDebuffStatusEnemy) + sizeof(sStatDebuffStatusNpc)))
    {
        Errorf("ENOMEM: %d", size);
        abort();
    }

    ReadSramFast(
        src,
        sStatDebuffStatusAlly,
        sizeof(sStatDebuffStatusAlly));

    src += sizeof(sStatDebuffStatusAlly);

    WriteAndVerifySramFast(
        src,
        sStatDebuffStatusEnemy,
        sizeof(sStatDebuffStatusEnemy));

    src += sizeof(sStatDebuffStatusEnemy);

    WriteAndVerifySramFast(
        src,
        sStatDebuffStatusNpc,
        sizeof(sStatDebuffStatusNpc));
}

void TickUnitStatDebuff(struct Unit * unit, enum DEBUFF_INFO_TYPE type)
{
    int i;
    u32 * bitfile = GetUnitStatDebuffStatus(unit);
    for (i = 0; i < UNIT_STAT_DEBUFF_MAX; i++)
    {
        if (_BIT_CHK(bitfile, i))
        {
            if (type == STATUS_INFO_TYPE_DEBUFF)
            {
                if (gpStatDebuffInfos[i].type == STATUS_INFO_TYPE_DEBUFF)
                {
                    _BIT_CLR(bitfile, i);
                }
            }
            else
            {
                _BIT_CLR(bitfile, i);
            }
        }
    }
}

/**
 * Pre-battle calc
 */
void PreBattleCalcStatDebuffs(struct BattleUnit * bu, struct BattleUnit * defender)
{
    int i;
    u32 * bitfile = GetUnitStatDebuffStatus(&bu->unit);
    for (i = 0; i < UNIT_STAT_DEBUFF_MAX; i++)
    {
        if (_BIT_CHK(bitfile, i))
        {
            const struct DebuffInfo * info = &gpStatDebuffInfos[i];

            bu->battleAttack       += info->battle_status.atk;
            bu->battleDefense      += info->battle_status.def;
            bu->battleHitRate      += info->battle_status.hit;
            bu->battleAvoidRate    += info->battle_status.avo;
            bu->battleCritRate     += info->battle_status.crit;
            bu->battleSilencerRate += info->battle_status.silencer;
            bu->battleDodgeRate    += info->battle_status.dodge;
        }
    }
}

/**
 * Modular status-getter
 */
#define STAT_DEBUFF_MSG_BUF_AMT 7
#define STAT_DEBUFF_MSG_BUF_NEXT(idx) (((idx) - 1) & 3)

struct StatDebuffMsgBuf {
    u32 bitfile;
    s8 uid;
    u32 special_mask;
    s16 pow, mag, skl, spd, def, res, lck, mov;
};
extern struct StatDebuffMsgBuf sStatDebuffMsgBuf[STAT_DEBUFF_MSG_BUF_AMT];
extern int sStatDebuffMsgBufNext;

enum STAT_BUFF_MSG_BUF_SPECIAL_MASK {
    SP_STAT_CANNOT_MOVE = (1 << 0x00),
};

STATIC_DECLAR void GenerateStatDebuffMsgBufExt(struct Unit * unit, u32 * bitfile, struct StatDebuffMsgBuf * buf)
{
    int i;
    bool in_panic = false;

    memset(buf, 0, sizeof(*buf));

    buf->bitfile = *bitfile;
    buf->uid = unit->index;

    Debugf("pid=%#x, bitfile [%p]=%#lx", UNIT_CHAR_ID(unit), bitfile, *bitfile);

    if (GetUnitStatusIndex(unit) == NEW_UNIT_STATUS_PANIC)
        in_panic = true;

    for (i = 0; i < UNIT_STAT_DEBUFF_MAX; i++)
    {
        if (_BIT_CHK(bitfile, i))
        {
            const struct DebuffInfo * info = &gpStatDebuffInfos[i];

            if (in_panic == true && info->type == STATUS_INFO_TYPE_BUFF)
            {
                buf->pow -= info->unit_status.pow;
                buf->mag -= info->unit_status.mag;
                buf->skl -= info->unit_status.skl;
                buf->spd -= info->unit_status.spd;
                buf->def -= info->unit_status.def;
                buf->res -= info->unit_status.res;
                buf->lck -= info->unit_status.lck;
                buf->mov -= info->unit_status.mov;
            }
            else
            {
                buf->pow += info->unit_status.pow;
                buf->mag += info->unit_status.mag;
                buf->skl += info->unit_status.skl;
                buf->spd += info->unit_status.spd;
                buf->def += info->unit_status.def;
                buf->res += info->unit_status.res;
                buf->lck += info->unit_status.lck;
                buf->mov += info->unit_status.mov;
            }

            if (info->cannot_move == true)
                buf->special_mask |= SP_STAT_CANNOT_MOVE;
        }
    }
}

STATIC_DECLAR struct StatDebuffMsgBuf * GetExistingStatDebuffMsgBuf(struct Unit * unit)
{
    int i;
    u32 bitfile = *GetUnitStatDebuffStatus(unit);

    for (i = STAT_DEBUFF_MSG_BUF_AMT - 1; i >= 0; i--)
    {
        struct StatDebuffMsgBuf * buf = &sStatDebuffMsgBuf[i];

        if (buf->bitfile == bitfile && buf->uid == unit->index)
        {
            if (i <= 3)
            {
                /* Generic list */
                sStatDebuffMsgBufNext = STAT_DEBUFF_MSG_BUF_NEXT(i);
            }
            return &sStatDebuffMsgBuf[i];
        }
    }
    return NULL;
}

STATIC_DECLAR struct StatDebuffMsgBuf * GetStatDebuffMsgBuf(struct Unit * unit)
{
    struct StatDebuffMsgBuf * buf = GetExistingStatDebuffMsgBuf(unit);
    if (!buf)
    {
        if (unit->index == gActiveUnit->index)
            buf = &sStatDebuffMsgBuf[STAT_DEBUFF_MSG_BUF_AMT - 3];
        else if (unit->index == gBattleActor.unit.index)
            buf = &sStatDebuffMsgBuf[STAT_DEBUFF_MSG_BUF_AMT - 2];
        else if (unit->index == gBattleTarget.unit.index)
            buf = &sStatDebuffMsgBuf[STAT_DEBUFF_MSG_BUF_AMT - 1];
        else
        {
            buf = &sStatDebuffMsgBuf[sStatDebuffMsgBufNext];
            sStatDebuffMsgBufNext = STAT_DEBUFF_MSG_BUF_NEXT(sStatDebuffMsgBufNext);
        }
        GenerateStatDebuffMsgBufExt(unit, GetUnitStatDebuffStatus(unit), buf);
    }

    Debugf("unit %#x at buf %d: pow=%d, mag=%d, skl=%d, spd=%d, lck=%d, def=%d, res=%d, mov=%d", 
            UNIT_CHAR_ID(unit), buf - sStatDebuffMsgBuf,
            buf->pow, buf->mag, buf->skl, buf->spd, buf->lck, buf->def, buf->res, buf->mov);

    return buf;
}

int PowGetterStatDebuff(int status, struct Unit * unit)
{
    return status + GetStatDebuffMsgBuf(unit)->pow;
}

int MagGetterStatDebuff(int status, struct Unit * unit)
{
    return status + GetStatDebuffMsgBuf(unit)->mag;
}

int SklGetterStatDebuff(int status, struct Unit * unit)
{
    return status + GetStatDebuffMsgBuf(unit)->skl;
}

int SpdGetterStatDebuff(int status, struct Unit * unit)
{
    return status + GetStatDebuffMsgBuf(unit)->spd;
}

int DefGetterStatDebuff(int status, struct Unit * unit)
{
    return status + GetStatDebuffMsgBuf(unit)->def;
}

int ResGetterStatDebuff(int status, struct Unit * unit)
{
    return status + GetStatDebuffMsgBuf(unit)->res;
}

int LckGetterStatDebuff(int status, struct Unit * unit)
{
    return status + GetStatDebuffMsgBuf(unit)->lck;
}

int MovGetterStatDebuff(int status, struct Unit * unit)
{
    return status + GetStatDebuffMsgBuf(unit)->mov;
}

void StatDeuff_OnNewGameInit(void)
{
    if (UNIT_STAT_DEBUFF_MAX_REAL >= UNIT_STAT_DEBUFF_MAX)
    {
        Errorf("StatDebuff overflowed: %d", UNIT_STAT_DEBUFF_MAX_REAL);
        abort();
    }
}

void StatDeuff_OnNewGameSave(void)
{
    CpuFastFill16(0, sStatDebuffStatusAlly, sizeof(sStatDebuffStatusAlly));
    CpuFastFill16(0, sStatDebuffStatusEnemy, sizeof(sStatDebuffStatusEnemy));
    memset(sStatDebuffStatusNpc, 0, sizeof(sStatDebuffStatusNpc));

    memset(sStatDebuffMsgBuf, 0, sizeof(sStatDebuffMsgBuf));
    sStatDebuffMsgBufNext = 0;
}

void StatDeuff_OnLoadUnit(struct Unit * unit)
{
    *GetUnitStatDebuffStatus(unit) = 0;
}

void StatDeuff_OnClearUnit(struct Unit * unit)
{
    *GetUnitStatDebuffStatus(unit) = 0;
}

void StatDeuff_OnCopyUnit(struct Unit * from, struct Unit * to)
{
    *GetUnitStatDebuffStatus(to) = *GetUnitStatDebuffStatus(from);
}

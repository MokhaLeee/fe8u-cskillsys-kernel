#include "global.h"
#include "bmsave.h"

#include "bwl.h"

/* LynJump! */
void PidStatsAddBattleAmt(struct Unit * unit)
{
    u32 pid;
    struct NewBwl * bwl;

    if (FACTION_BLUE != UNIT_FACTION(unit))
        return;

    pid = UNIT_CHAR_ID(unit);

    bwl = GetNewBwl(pid);
    if (NULL == bwl)
        return;

    if (bwl->battleAmt < 4000)
        bwl->battleAmt++;
}

/* LynJump! */
void PidStatsAddWinAmt(u8 pid)
{
    struct NewBwl * bwl = GetNewBwl(pid);
    if (NULL == bwl)
        return;

    if (bwl->winAmt < 1000)
        bwl->winAmt++;
}

/* LynJump! */
void PidStatsRecordLoseData(u8 pid)
{
    struct NewBwl * bwl;

    bwl = GetNewBwl(pid);
    if (NULL == bwl)
        return;

    if (bwl->lossAmt < 255)
        bwl->lossAmt++;
}

/* LynJump! */
void PidStatsRecordDefeatInfo(u8 pid, u8 killerPid, int deathCause)
{
    return;
}

/* LynJump! */
void PidStatsAddActAmt(u8 pid)
{
    return;
}

/* LynJump! */
void PidStatsAddStatViewAmt(u8 pid)
{
    return;
}

/* LynJump! */
void PidStatsAddDeployAmt(u8 pid)
{
    return;
}

/* LynJump! */
void PidStatsAddSquaresMoved(u8 pid, int amount)
{
    return;
}

/* LynJump! */
void PidStatsAddExpGained(u8 pid, int expGain)
{
    return;
}

/* LynJump! */
void PidStatsSubFavval08(u8 pid)
{
    return;
}

/* LynJump! */
void PidStatsSubFavval100(u8 pid)
{
    return;
}

/* LynJump! */
int PidStatsGetTotalBattleAmt(void)
{
    int i, ret = 0;

    for (i = 0; i < NEW_BWL_ARRAY_NUM; i++)
    {
        struct NewBwl * bwl;
        bwl = GetNewBwl(i);
        if (bwl)
            ret += bwl->battleAmt;
    }

    return ret;
}

/* LynJump! */
int PidStatsGetTotalWinAmt(void)
{
    int i, ret = 0;

    for (i = 0; i < NEW_BWL_ARRAY_NUM; i++)
    {
        struct NewBwl * bwl;
        bwl = GetNewBwl(i);
        if (bwl)
            ret += bwl->winAmt;
    }

    return ret;
}

/* LynJump! */
int PidStatsGetTotalLossAmt(void)
{
    int i, ret = 0;

    for (i = 0; i < NEW_BWL_ARRAY_NUM; i++)
    {
        struct NewBwl * bwl;
        bwl = GetNewBwl(i);
        if (bwl)
            ret += bwl->lossAmt;
    }

    return ret;
}

/* LynJump! */
int PidStatsGetTotalLevel(void)
{
    return 1;
}

/* LynJump! */
int PidStatsGetTotalExpGain(void)
{
    return 0;
}

/* LynJump! */
int PidStatsGetExpGain(u8 pid)
{
    return 0;
}

/* LynJump! */
int PidStatsGetFavval(u8 pid)
{
    return 0;
}

/* LynJump! */
void PidStatsAddFavval(u8 pid, int val)
{
    return;
}

// void PidStatsRecordBattleRes(void)

/* LynJump! */
char * GetPidDefeatedEndingString(int pid)
{
    return "Died at unknown";
}

/* LynJump! */
void sub_80B6CA8(ProcPtr proc)
{
    return;
}

/* LynJump! */
u16 GetChapterDeathCount(void)
{
    return 0;
}

/* LynJump! */
void DisplayBwl(void)
{
    return;
}

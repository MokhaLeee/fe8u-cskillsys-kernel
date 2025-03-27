#include "common-chax.h"
#include "bwl.h"

const int Sizeof_NewBwl = sizeof(struct NewBwl);

bool CheckHasBwl(u8 pid)
{
	if (pid >= NEW_BWL_ARRAY_NUM)
		return false;

	if (GetCharacterData(pid)->affinity == 0)
		return false;

	return true;
}

struct NewBwl *GetNewBwl(u8 pid)
{
	struct NewBwl *entry = (struct NewBwl *)gPidStatsData;

	if (!CheckHasBwl(pid))
		return NULL;

	return entry + (pid - 1);
}

LYN_REPLACE_CHECK(PidStatsAddBattleAmt);
void PidStatsAddBattleAmt(struct Unit *unit)
{
	u32 pid;
	struct NewBwl *bwl;

	if (UNIT_FACTION(unit) != FACTION_BLUE)
		return;

	pid = UNIT_CHAR_ID(unit);
	bwl = GetNewBwl(pid);

	if (bwl == NULL)
		return;

	if (bwl->battleAmt < 4000)
		bwl->battleAmt++;
}

LYN_REPLACE_CHECK(PidStatsAddWinAmt);
void PidStatsAddWinAmt(u8 pid)
{
	struct NewBwl *bwl = GetNewBwl(pid);

	if (bwl == NULL)
		return;

	if (bwl->winAmt < 1000)
		bwl->winAmt++;
}

LYN_REPLACE_CHECK(PidStatsRecordLoseData);
void PidStatsRecordLoseData(u8 pid)
{
	struct NewBwl *bwl = GetNewBwl(pid);

	if (bwl == NULL)
		return;

	if (bwl->lossAmt < 255)
		bwl->lossAmt++;
}

LYN_REPLACE_CHECK(PidStatsRecordDefeatInfo);
void PidStatsRecordDefeatInfo(u8 pid, u8 killerPid, int deathCause) {}

LYN_REPLACE_CHECK(PidStatsAddActAmt);
void PidStatsAddActAmt(u8 pid) {}

LYN_REPLACE_CHECK(PidStatsAddStatViewAmt);
void PidStatsAddStatViewAmt(u8 pid) {}

LYN_REPLACE_CHECK(PidStatsAddDeployAmt);
void PidStatsAddDeployAmt(u8 pid) {}

LYN_REPLACE_CHECK(PidStatsAddSquaresMoved);
void PidStatsAddSquaresMoved(u8 pid, int amount) {}

LYN_REPLACE_CHECK(PidStatsAddExpGained);
void PidStatsAddExpGained(u8 pid, int expGain) {}

LYN_REPLACE_CHECK(PidStatsSubFavval08);
void PidStatsSubFavval08(u8 pid) {}

LYN_REPLACE_CHECK(PidStatsSubFavval100);
void PidStatsSubFavval100(u8 pid) {}

LYN_REPLACE_CHECK(PidStatsGetTotalBattleAmt);
int PidStatsGetTotalBattleAmt(void)
{
	int i, ret = 0;

	for (i = 0; i < NEW_BWL_ARRAY_NUM; i++) {
		struct NewBwl *bwl = GetNewBwl(i);

		if (bwl)
			ret += bwl->battleAmt;
	}

	return ret;
}

LYN_REPLACE_CHECK(PidStatsGetTotalWinAmt);
int PidStatsGetTotalWinAmt(void)
{
	int i, ret = 0;

	for (i = 0; i < NEW_BWL_ARRAY_NUM; i++) {
		struct NewBwl *bwl = GetNewBwl(i);

		if (bwl)
			ret += bwl->winAmt;
	}

	return ret;
}

LYN_REPLACE_CHECK(PidStatsGetTotalLossAmt);
int PidStatsGetTotalLossAmt(void)
{
	int i, ret = 0;

	for (i = 0; i < NEW_BWL_ARRAY_NUM; i++) {
		struct NewBwl *bwl = GetNewBwl(i);

		if (bwl)
			ret += bwl->lossAmt;
	}

	return ret;
}

LYN_REPLACE_CHECK(PidStatsGetTotalLevel);
int PidStatsGetTotalLevel(void)
{
	return 1;
}

LYN_REPLACE_CHECK(PidStatsGetTotalExpGain);
int PidStatsGetTotalExpGain(void)
{
	return 0;
}

LYN_REPLACE_CHECK(PidStatsGetExpGain);
int PidStatsGetExpGain(u8 pid)
{
	return 0;
}

LYN_REPLACE_CHECK(PidStatsGetFavval);
int PidStatsGetFavval(u8 pid)
{
	return 0;
}

LYN_REPLACE_CHECK(PidStatsAddFavval);
void PidStatsAddFavval(u8 pid, int val) {}

// void PidStatsRecordBattleRes(void)

LYN_REPLACE_CHECK(GetPidDefeatedEndingString);
char *GetPidDefeatedEndingString(int pid)
{
	return "Died at unknown";
}

LYN_REPLACE_CHECK(sub_80B6CA8);
void sub_80B6CA8(struct EndingBattleDisplayProc *proc) {}

LYN_REPLACE_CHECK(GetChapterDeathCount);
u16 GetChapterDeathCount(void)
{
	return 0;
}

LYN_REPLACE_CHECK(DisplayBwl);
void DisplayBwl(void) {}

#include "common-chax.h"
#include "kernel-tutorial.h"
#include "kernel-tutorial-internal.h"

extern u32 sKTutorialBits[3]; /* total 96 flags */
extern u32 sKTutorialBitsHistory[3];

static inline void set_bit(u32 * bits, int idx)
{
    bits[idx / 32] |= 1 << (idx % 32);
}

static inline bool check_bit(u32 * bits, int idx)
{
    if (bits[idx / 32] & (1 << (idx % 32)))
        return true;

    return false;
}

/* In GameInit */
static void ClearTrigKtutFlags(void)
{
    memset(sKTutorialBits, 0, sizeof(sKTutorialBits));
}

void ClearKtutorialFlags(void)
{
    ClearTrigKtutFlags();
    memset(sKTutorialBitsHistory, 0, sizeof(sKTutorialBitsHistory));
}

/* In SaveData */
void EMS_SaveKTutorialFlags(u8 * dst, const u32 size)
{
    Assert(size >= (sizeof(sKTutorialBitsHistory) + sizeof(sKTutorialBits)));

    WriteAndVerifySramFast(
        sKTutorialBits,
        dst,
        sizeof(sKTutorialBits));

    WriteAndVerifySramFast(
        sKTutorialBitsHistory,
        dst + sizeof(sKTutorialBits),
        sizeof(sKTutorialBitsHistory));
}

/* In SaveData */
void EMS_LoadKTutorialFlags(u8 * src, const u32 size)
{
    Assert(size >= (sizeof(sKTutorialBitsHistory) + sizeof(sKTutorialBits)));

    ReadSramFast(
        src,
        sKTutorialBits,
        sizeof(sKTutorialBits));

    ReadSramFast(
        src + sizeof(sKTutorialBits),
        sKTutorialBitsHistory,
        sizeof(sKTutorialBitsHistory));
}

/* API */
bool CanExecKTutorial(void)
{
    if (gConfigKTutorialLevel == KTUT_LEVEL_NEVER)
        return false;

    if (gConfigKTutorialLevel == KTUT_LEVEL_MIDDLE)
    {
        /* Only in tutorial mode */
        if (!TUTORIAL_MODE())
            return false;
    }

    if (gConfigKTutorialLevel == KTUT_LEVEL_HIGH)
        return true;

    return true;
}

void TriggerKtutorial(int flag)
{
    set_bit(sKTutorialBits, flag);
}

int GetTriggerKtutorial(void)
{
    int i;
    for (i = 1; i < KTUTORIAL_MAX; i++)
        if (check_bit(sKTutorialBits, i) && !check_bit(sKTutorialBitsHistory, i))
            return i;

    return -1;
}

void PutKtutHistory(int flag)
{
    /**
     * If we triggered more than one k-tut,
     * We can just trigger one tutorial, but other will be negleted.
     * Here we clear all trigger flags.
     */
    ClearTrigKtutFlags();

    if (flag < KTUTORIAL_MAX)
        set_bit(sKTutorialBitsHistory, flag);
}

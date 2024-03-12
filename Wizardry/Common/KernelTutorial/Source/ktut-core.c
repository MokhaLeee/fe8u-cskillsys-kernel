#include "common-chax.h"
#include "kernel-tutorial.h"
#include "kernel-tutorial-internal.h"

extern u32 sKTutorialBits[3]; /* total 96 flags */
extern u32 sTriggeredKTutorialBits[3];

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
void ResetKTutorialFlags(void)
{
    memset(sKTutorialBits, 0, sizeof(sKTutorialBits));
    memset(sTriggeredKTutorialBits, 0, sizeof(sTriggeredKTutorialBits));
}

/* In SaveData */
void EMS_SaveKTutorialFlags(u8 * dst, const u32 size)
{
    Assert(size >= (sizeof(sTriggeredKTutorialBits) + sizeof(sKTutorialBits)));

    WriteAndVerifySramFast(
        sKTutorialBits,
        dst,
        sizeof(sKTutorialBits));

    WriteAndVerifySramFast(
        sTriggeredKTutorialBits,
        dst + sizeof(sKTutorialBits),
        sizeof(sTriggeredKTutorialBits));
}

/* In SaveData */
void EMS_LoadKTutorialFlags(u8 * src, const u32 size)
{
    Assert(size >= (sizeof(sTriggeredKTutorialBits) + sizeof(sKTutorialBits)));

    ReadSramFast(
        src,
        sKTutorialBits,
        sizeof(sKTutorialBits));

    ReadSramFast(
        src + sizeof(sKTutorialBits),
        sTriggeredKTutorialBits,
        sizeof(sTriggeredKTutorialBits));
}

/* API */
void SetKTutorialFlag(int flag)
{
    set_bit(sKTutorialBits, flag);
}

int TryTriggerTutorialFlag(void)
{
    int i;
    for (i = 1; i < (3 * 32); i++)
    {
        if (check_bit(sKTutorialBits, i) && !check_bit(sTriggeredKTutorialBits, i))
        {
            set_bit(sTriggeredKTutorialBits, i);
            return i;
        }
    }
    return -1;
}

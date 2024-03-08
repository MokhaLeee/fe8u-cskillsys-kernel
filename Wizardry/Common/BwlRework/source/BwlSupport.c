#include "global.h"
#include "bmunit.h"
#include "bmsave.h"
#include "bmmind.h"

#include "bwl.h"

void StartSupportTalk(u8, u8, int);
void SetSupportLevelGained(u8 charA, u8 charB);
s8 HasUnitGainedSupportLevel(struct Unit * unit, int num);
extern int sSupportMaxExpLookup[];

static u8 * GetUnitBwlSupports(u8 pid)
{
    struct NewBwl * bwl = GetNewBwl(pid);
    if (bwl)
        return bwl->supports;

    return NULL;
}

/* LynJump! */
s8 ActionSupport(ProcPtr proc)
{
    int subjectExp;
    int targetExp;
    u8 * supp1, * supp2;

    struct Unit * target = GetUnit(gActionData.targetIndex);

    int targetSupportNum = GetUnitSupporterNum(gActiveUnit, target->pCharacterData->number);
    int subjectSupportNum = GetUnitSupporterNum(target, gActiveUnit->pCharacterData->number);

    CanUnitSupportNow(target, subjectSupportNum);

    UnitGainSupportLevel(gActiveUnit, targetSupportNum);
    UnitGainSupportLevel(target, subjectSupportNum);

    StartSupportTalk(
        gActiveUnit->pCharacterData->number,
        target->pCharacterData->number,
        GetUnitSupportLevel(gActiveUnit, targetSupportNum)
    );

    supp1 = GetUnitBwlSupports(UNIT_CHAR_ID(gActiveUnit));
    supp2 = GetUnitBwlSupports(UNIT_CHAR_ID(target));

    if (supp1 && supp2)
    {
        subjectExp = supp1[targetSupportNum];
        targetExp = supp2[subjectSupportNum];

        if (subjectExp != targetExp)
        {
            if (subjectExp > targetExp)
                supp2[subjectSupportNum] = subjectExp;

            if (subjectExp < targetExp)
                supp1[targetSupportNum] = targetExp;
        }
    }

    return 0;
}

/* LynJump! */
int GetUnitSupportLevel(struct Unit * unit, int num)
{
    int supportExp;
    u8 * supp;

    if (!UNIT_IS_VALID(unit))
        return SUPPORT_LEVEL_NONE;

    supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));

    if (supp)
    {
        supportExp = unit->supports[num];
    }
    else
    {
        /**
         * for none-BWL characters,
         * directly judge on its rom data
         */
        supportExp = unit->pCharacterData->pSupportData->supportExpBase[num];
    }

    if (supportExp > 240)
        return SUPPORT_LEVEL_A;

    if (supportExp > 160)
        return SUPPORT_LEVEL_B;

    if (supportExp > 80)
        return SUPPORT_LEVEL_C;

    return SUPPORT_LEVEL_NONE;
}

/* LynJump! */
void UnitGainSupportExp(struct Unit * unit, int num)
{
    u8 * supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));

    if (UNIT_SUPPORT_DATA(unit) && supp)
    {
        int gain = UNIT_SUPPORT_DATA(unit)->supportExpGrowth[num];
        int currentExp = supp[num];
        int maxExp = sSupportMaxExpLookup[GetUnitSupportLevel(unit, num)];

        if (currentExp + gain > maxExp)
            gain = maxExp - currentExp;

        supp[num] = currentExp + gain;
        gPlaySt.chapterTotalSupportGain += gain;
    }
}

/* LynJump! */
void UnitGainSupportLevel(struct Unit* unit, int num)
{
    u8 * supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));
    if (supp)
        supp[num]++;

    gPlaySt.chapterTotalSupportGain++;

    SetSupportLevelGained(unit->pCharacterData->number, GetUnitSupporterCharacter(unit, num));
}

/* LynJump! */
s8 CanUnitSupportNow(struct Unit * unit, int num)
{
    int exp, maxExp;
    u8 * supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));

    if (gPlaySt.chapterStateBits & PLAY_FLAG_EXTRA_MAP)
        return FALSE;

    if (gPlaySt.chapterStateBits & PLAY_FLAG_TUTORIAL)
        return FALSE;

    if (HasUnitGainedSupportLevel(unit, num))
        return FALSE;

    if (GetUnitTotalSupportLevel(unit) >= MAX_SIMULTANEOUS_SUPPORT_COUNT)
        return FALSE;

    if (GetUnitTotalSupportLevel(GetUnitSupporterUnit(unit, num)) >= MAX_SIMULTANEOUS_SUPPORT_COUNT)
        return FALSE;

    if (!supp)
        return FALSE;

    exp    = supp[num];
    maxExp = sSupportMaxExpLookup[GetUnitSupportLevel(unit, num)];

    if (exp == SUPPORT_EXP_A)
        return FALSE;

    return (exp == maxExp) ? TRUE : FALSE;
}

/* LynJump! */
void InitUnitsupports(struct Unit * unit)
{
    u8 * supp1, * supp2;
    int i, count = GetUnitSupporterCount(unit);

    for (i = 0; i < count; ++i)
    {
        struct Unit * other = GetUnitSupporterUnit(unit, i);

        if (!other)
            continue;

        supp1 = GetUnitBwlSupports(UNIT_CHAR_ID(unit));
        supp2 = GetUnitBwlSupports(UNIT_CHAR_ID(other));

        if (!supp1 || !supp2)
            continue;

        supp2[GetUnitSupporterNum(other, unit->pCharacterData->number)] = 0;
        supp1[i] = 0;
    }
}

/* LynJump! */
void UnitLoadSupports(struct Unit * unit)
{
    int i, count = GetUnitSupporterCount(unit);
    u8 * supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));

    if (supp)
    {
        CpuFill16(0, supp, UNIT_SUPPORT_MAX_COUNT);

        for (i = 0; i < count; ++i)
            supp[i] = GetUnitSupporterInitialExp(unit, i);
    }
}

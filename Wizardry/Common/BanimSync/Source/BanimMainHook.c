#include "common-chax.h"
#include "banim-sync.h"
#include "banim-sync-static.h"

int  AnimInterpret(struct Anim * anim);

/**
 * 1. C0D
 * 2. On battle start
 */
void BanimSwitch_HookOnMain(struct Anim * anim)
{
    bool banim_switched = false;
    int type;
    struct Anim * anim1, * anim2;

    if (anim->state3 & ANIM_BIT3_NEXT_ROUND_START)
    {
        type = GetAnimNextRoundType(anim);
        if (type != ANIM_ROUND_INVALID)
        {

#if CHAX
            if (GetAISLayerId(anim) == 0)
            {
                if (TrySwitchBanim(anim) == 1)
                {
                    UpdateBanimFrame();
                }
            }
#endif

            anim1 = gAnims[GetAnimPosition(anim) * 2];
            SwitchAISFrameDataFromBARoundType(anim1, type);
            anim1->state3 &= ~ANIM_BIT3_NEXT_ROUND_START;
            anim1->state3 |= ANIM_BIT3_C01_BLOCKING_IN_BATTLE;

            anim2 = gAnims[GetAnimPosition(anim) * 2 + 1];
            SwitchAISFrameDataFromBARoundType(anim2, type);
            anim2->state3 &= ~ANIM_BIT3_NEXT_ROUND_START;
            anim2->state3 |= ANIM_BIT3_C01_BLOCKING_IN_BATTLE;

            anim1->nextRoundId = anim1->nextRoundId + 1;
            anim2->nextRoundId = anim2->nextRoundId + 1;

            AnimScrAdvance(anim1);
            AnimScrAdvance(anim2);
        }
        else
        {
            anim1 = gAnims[GetAnimPosition(anim) * 2 + 0];
            anim1->state3 &= ~ANIM_BIT3_NEXT_ROUND_START;

            anim2 = gAnims[GetAnimPosition(anim) * 2 + 1];
            anim2->state3 &= ~ANIM_BIT3_NEXT_ROUND_START;
        } /* ANIM_ROUND_INVALID */
    }
    else
    {
        if (anim->state3 & ANIM_BIT3_NEW_ROUND_START)
        {
            type = GetAnimNextRoundType(anim);
            if (type != ANIM_ROUND_INVALID)
            {
                anim1 = gAnims[GetAnimPosition(anim) * 2];
                anim2 = gAnims[GetAnimPosition(anim) * 2 + 1];

#if CHAX
                anim1->nextRoundId = anim1->nextRoundId + 1;
                anim2->nextRoundId = anim2->nextRoundId + 1;

                if (GetAISLayerId(anim) == 0)
                {
                    if (TrySwitchBanim(anim) == 1)
                    {
                        UpdateBanimFrame();
                        banim_switched = true;
                    }
                }
#endif

                SwitchAISFrameDataFromBARoundType(anim1, type);
                anim1->state3 &= ~ANIM_BIT3_NEW_ROUND_START;
                anim1->state3 |= ANIM_BIT3_C01_BLOCKING_IN_BATTLE;

                SwitchAISFrameDataFromBARoundType(anim2, type);
                anim2->state3 &= ~ANIM_BIT3_NEW_ROUND_START;
                anim2->state3 |= ANIM_BIT3_C01_BLOCKING_IN_BATTLE;

                if (banim_switched)
                {
                    int frame_front = BanimDefaultModeConfig[BattleTypeToAnimModeEndOfDodge[gEkrDistanceType] * 4 + 0];
                    int idx = GetAnimPosition(anim) == EKR_POS_L ? gpBanimModesLeft[frame_front] : gpBanimModesRight[frame_front];

                    const void * _ptr;
                    struct BanimModeData * unk = (void *)(idx + gBanimScrLeft + GetAnimPosition(anim) * 0x2A00);

                    anim1->pImgSheet = unk->img;
                    _ptr = anim1->pSpriteDataPool;
                    _ptr += unk->unk2;
                    anim1->pSpriteData = _ptr;
                    _ptr = anim2->pSpriteDataPool + 0x57F0;
                    anim2->pSpriteData = _ptr;

                    if (gpImgSheet[GetAnimPosition(anim1)] != anim1->pImgSheet)
                    {
                        NewEkrChienCHR(anim1);
                        gpImgSheet[GetAnimPosition(anim1)] = anim1->pImgSheet;
                    }
                }

#if !CHAX
                anim1->nextRoundId = anim1->nextRoundId + 1;
                anim2->nextRoundId = anim2->nextRoundId + 1;
#endif

                AnimScrAdvance(anim1);
                AnimScrAdvance(anim2);
            }
        }
        else
        {
            if (GetAISLayerId(anim) == 0)
            {
                type = GetBattleAnimRoundType(anim->nextRoundId * 2 + GetAnimPosition(anim));
                if (type == ANIM_ROUND_INVALID)
                    gBanimDoneFlag[GetAnimPosition(anim)] = 1;
            }
        }
    }
}

void BanimSwitch_HookC0D(struct Anim * anim, int type)
{
    struct Anim * anim1, * anim2;

#if CHAX
    if (GetAISLayerId(anim) == 0)
    {
        if (TrySwitchBanim(anim) == 1)
            UpdateBanimFrame();
    }
#endif

    anim1 = gAnims[GetAnimPosition(anim) * 2];
    anim2 = gAnims[GetAnimPosition(anim) * 2 + 1];

    SwitchAISFrameDataFromBARoundType(anim1, type);
    SwitchAISFrameDataFromBARoundType(anim2, type);
}

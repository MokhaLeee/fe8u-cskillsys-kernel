#include "common-chax.h"
#include "battle-system.h"

bool IsAttackerAnim(struct Anim * anim)
{
    if (GetAnimPosition(anim) == POS_L)
    {
        if (gpEkrBattleUnitLeft == &gBattleActor)
            return true;
    }
    else
    {
        if (gpEkrBattleUnitRight == &gBattleActor)
            return true;
    }
    return false;
}

int GetBattleActorHpAtRound(int round)
{
    int i, ret = 0;

    if (round > NEW_BATTLE_HIT_MAX)
        round = NEW_BATTLE_HIT_MAX;

    ret = GetUnitCurrentHp(GetUnit(gBattleActor.unit.index));

    for (i = 0; i < round; i++)
    {
        struct BattleHit * hit = &gBattleHitArrayRe[i];

        if (hit->info & BATTLE_HIT_INFO_RETALIATION)
            ret += hit->hpChange;

        if (hit->info & BATTLE_HIT_INFO_END)
            break;
    }
    return ret;
}

int GetBattleTargetHpAtRound(int round)
{
    int i, ret = 0;

    if (round > NEW_BATTLE_HIT_MAX)
        round = NEW_BATTLE_HIT_MAX;

    ret = GetUnitCurrentHp(GetUnit(gBattleTarget.unit.index));

    for (i = 0; i < round; i++)
    {
        struct BattleHit * hit = &gBattleHitArrayRe[i];

        if (!(hit->info & BATTLE_HIT_INFO_RETALIATION))
            ret += hit->hpChange;

        if (hit->info & BATTLE_HIT_INFO_END)
            break;
    }
    return ret;
}

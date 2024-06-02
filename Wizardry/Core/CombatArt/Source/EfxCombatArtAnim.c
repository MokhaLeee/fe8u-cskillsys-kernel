#include "global.h"

#include "common-chax.h"
#include "battle-system.h"
#include "skill-system.h"
#include "combat-art.h"

#define LOCAL_TRACE 0

extern u8 sEfxCombatArtRoundData[NEW_BATTLE_HIT_MAX];

void InitEfxCombatArtRoundData(void)
{
    CpuFill16(0, sEfxCombatArtRoundData, sizeof(sEfxCombatArtRoundData));
}

int GetEfxCombatArtIndex(const u8 cid)
{
    if (COMBART_VALID(cid))
        return gpEfxCombatArtAnimInfos[cid].aid;

    LTRACEF("Try get invalid CombatArt info: %#X", cid);
    return 0;
}

int GetEfxCombatArtPriority(const u8 cid)
{
    if (COMBART_VALID(cid))
        return gpEfxCombatArtAnimInfos[cid].priority;

    LTRACEF("Try get invalid CombatArt info: %#X", cid);
    return 0;
}

int GetEfxCombatArtSfx(const u8 cid)
{
    if (COMBART_VALID(cid))
        return gpEfxCombatArtAnimInfos[cid].sfx;

    LTRACEF("Try get invalid CombatArt info: %#X", cid);
    return 0;
}

void RegisterEfxSkillCombatArt(int round, const u8 cid)
{
    if (round < NEW_BATTLE_HIT_MAX)
    {
        u8 old = sEfxCombatArtRoundData[round];
        if (COMBART_VALID(old) && GetEfxCombatArtPriority(old) >= GetEfxCombatArtPriority(cid))
            return;

        LTRACEF("CombatArt %#x at round %d", cid, round);
        sEfxCombatArtRoundData[round] = cid;
    }
}

u8 GetEfxCombatArt(int round)
{
    if (round < NEW_BATTLE_HIT_MAX)
        return sEfxCombatArtRoundData[round];

    return 0;
}

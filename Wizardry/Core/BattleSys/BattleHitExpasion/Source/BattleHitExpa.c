#include "common-chax.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "combat-art.h"
#include "battle-system.h"
#include "combo-attack.h"

bool CheckBattleHitOverflow(void)
{
    return (gBattleHitIterator - gBattleHitArrayRe) >= NEW_BATTLE_HIT_MAX;
}

static void back_up_current_info(void)
{
    void * buf = BattleRoundInfoBak;

    Assert(sizeof(BattleRoundInfoBak) >=
        (sizeof(gBattleHitArrayRe) + sizeof(gBattleActorGlobalFlag) + sizeof(gBattleTargetGlobalFlag) + sizeof(gBattleTargetGlobalFlag)));

    memcpy(buf, gBattleHitArrayRe, sizeof(gBattleHitArrayRe));
    buf += sizeof(gBattleHitArrayRe);

    memcpy(buf, &gBattleActorGlobalFlag, sizeof(gBattleActorGlobalFlag));
    buf += sizeof(gBattleActorGlobalFlag);

    memcpy(buf, &gBattleTargetGlobalFlag, sizeof(gBattleTargetGlobalFlag));
    buf += sizeof(gBattleTargetGlobalFlag);

    memcpy(buf, gComboAtkList, sizeof(gComboAtkList));
    buf += sizeof(gBattleTargetGlobalFlag);
}

void RestoreBattleRoundInfo(void)
{
    void * buf = BattleRoundInfoBak;

    Assert(sizeof(BattleRoundInfoBak) >=
        (sizeof(gBattleHitArrayRe) + sizeof(gBattleActorGlobalFlag) + sizeof(gBattleTargetGlobalFlag) + sizeof(gBattleTargetGlobalFlag)));

    memcpy(gBattleHitArrayRe, buf, sizeof(gBattleHitArrayRe));
    buf += sizeof(gBattleHitArrayRe);

    memcpy(&gBattleActorGlobalFlag, buf, sizeof(gBattleActorGlobalFlag));
    buf += sizeof(gBattleActorGlobalFlag);

    memcpy(&gBattleTargetGlobalFlag, buf, sizeof(gBattleTargetGlobalFlag));
    buf += sizeof(gBattleTargetGlobalFlag);

    memcpy(gComboAtkList, buf, sizeof(gComboAtkList));
    buf += sizeof(gBattleTargetGlobalFlag);
}

/* LynJump */
void ClearBattleHits(void)
{
    /**
     * Since we hold a need that restore hit info during post-action
     * So I decide directly backup them.
     * And let users use API "RestoreBattleRoundInfo" to restore cleared info.
     */
    back_up_current_info();

    CpuFill16(0, gBattleHitArrayRe, sizeof(gBattleHitArrayRe));
    InitEfxCombatArtRoundData();
    InitEfxSkillRoundData();
    ClearBattleGlobalFlags();
    ResetComboAtkList();
    gBattleHitIterator = gBattleHitArrayRe;
}

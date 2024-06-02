#include "common-chax.h"
#include "kernel-lib.h"
#include "battle-system.h"
#include "skill-system.h"
#include "map-anims.h"
#include "combat-art.h"
#include "weapon-range.h"
#include "constants/skills.h"

STATIC_DECLAR void CallEventThunderfx(ProcPtr proc)
{
    CallMapAnim_ThunderStorm(proc, gBattleTargetPositionBackup.x, gBattleTargetPositionBackup.y);
}

STATIC_DECLAR void SetThunderstormAoeDamage(ProcPtr proc)
{
    int i;
    struct Unit * unit = gActiveUnit;
    BmMapFill(gBmMapMovement, -1);
    BmMapFill(gBmMapRange, 0);

    AddMap(gBattleTargetPositionBackup.x, gBattleTargetPositionBackup.y, 0b11, 1, 0);

    InitTargets(unit->xPos, unit->yPos);
    ForEachUnitInRange(AddUnitToTargetListIfNotAllied);

    for (i = 0; i < GetSelectTargetCount(); i++)
    {
        int hp;
        struct SelectTarget * starget = GetTarget(i);
        struct Unit * tunit = GetUnit(starget->uid);
        if (!UNIT_IS_VALID(tunit))
            continue;

        hp = GetUnitCurrentHp(tunit);
        hp -= 10;
        if (hp <= 0)
            hp = 1;

        SetUnitHp(tunit, hp);
    }
}

STATIC_DECLAR const EventScr EventScr_CallThunderfxAtPosition[] = {
    STARTFADE
    EvtColorFadeSetup(0x0, 0x20, 8, 128, 128, 128) // ENOSUPP in EAstdlib
    STAL(30)
    SOUN(0x11A)
    ASMC(CallEventThunderfx)
    STAL(1)
    ASMC(SetThunderstormAoeDamage)
    STAL(60)
    EvtColorFadeSetup(0x0, 0x20, 4, 256, 256, 256) // ENOSUPP in EAstdlib
    NoFade
    ENDA
};

bool PostActionThunderstorm(ProcPtr parent)
{
    struct Unit * unit = gActiveUnit;

#if defined(SID_Thunderstorm) && (SID_Thunderstorm < MAX_SKILL_NUM)
    if (!SkillTester(unit, SID_Thunderstorm))
#else
    if (1)
#endif
        return false;

    if (gBattleStats.range < 3)
        return false;

    if (gActionData.unitActionType == UNIT_ACTION_COMBAT)
    {
        if (gBattleActorGlobalFlag.hitted == true)
        {
            KernelCallEvent(EventScr_CallThunderfxAtPosition, EV_EXEC_CUTSCENE, parent);
            return true;
        }
    }
    return false;
}

#include "common-chax.h"
#include "kernel-lib.h"
#include "battle-system.h"
#include "skill-system.h"
#include "map-anims.h"
#include "debuff.h"
#include "combat-art.h"
#include "weapon-range.h"
#include "constants/skills.h"

STATIC_DECLAR void PostActionThunder_CameraOnActor(ProcPtr proc)
{
    EnsureCameraOntoPosition(proc, gActiveUnit->xPos, gActiveUnit->yPos);
}

STATIC_DECLAR void PostActionThunder_CameraOnTarget(ProcPtr proc)
{
    EnsureCameraOntoPosition(proc, gBattleTargetPositionBackup.x, gBattleTargetPositionBackup.y);
}

STATIC_DECLAR void PostActionThunder_Init(ProcPtr proc)
{
    struct Unit * unit;
    struct MuProc * mu;

    unit = gActiveUnit;

    MapAnim_CommonInit();

    HideUnitSprite(unit);
    mu = StartMu(unit);

    FreezeSpriteAnim(mu->sprite_anim);
    SetMuDefaultFacing(mu);
    SetDefaultColorEffects();
}

STATIC_DECLAR void PostActionThunder_StartAction(ProcPtr proc)
{
    StartMuActionAnim(GetUnitMu(gActiveUnit));
}

STATIC_DECLAR void PostActionThunder_ResetActor(ProcPtr proc)
{
    struct MuProc * mu = GetUnitMu(gActiveUnit);
    FreezeSpriteAnim(mu->sprite_anim);
    SetMuDefaultFacing(mu);
}

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

#if defined(SID_Thunderstorm) && (COMMON_SKILL_VALID(SID_Thunderstorm))
        hp -= SKILL_EFF0(SID_Thunderstorm);
#else
        hp -= 10;
#endif

        if (hp <= 0)
            hp = 1;

        SetUnitHp(tunit, hp);
    }
}

STATIC_DECLAR const EventScr EventScr_CallThunderfxAtPosition[] = {
    STAL(1)
    ASMC(PostActionThunder_CameraOnActor)
    STAL(1)
    ASMC(PostActionThunder_Init)
    STAL(2)
    ASMC(PostActionThunder_StartAction)
    STAL(10)
    ASMC(PostActionThunder_CameraOnTarget)
    STAL(10)
    STARTFADE
    EvtColorFadeSetup(0x0, 0x20, 8, 128, 128, 128) // ENOSUPP in EAstdlib
    STAL(30)
    SOUN(0x11A)
    ASMC(CallEventThunderfx)
    STAL(1)
    ASMC(SetThunderstormAoeDamage)
    STAL(60)
    EvtColorFadeSetup(0x0, 0x20, 4, 256, 256, 256) // ENOSUPP in EAstdlib
    STAL(1)
    ASMC(PostActionThunder_ResetActor)
    STAL(1)
    ASMC(PostActionThunder_CameraOnActor)
    STAL(1)
    ASMC(MapAnim_CommonEnd)
    STAL(10)
    NoFade
    ENDA
};

bool PostActionThunderstorm(ProcPtr parent)
{
    struct Unit * unit = gActiveUnit;

    if (!UNIT_ALIVE(unit) || UNIT_STONED(unit))
        return false;

#if defined(SID_Thunderstorm) && (COMMON_SKILL_VALID(SID_Thunderstorm))
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

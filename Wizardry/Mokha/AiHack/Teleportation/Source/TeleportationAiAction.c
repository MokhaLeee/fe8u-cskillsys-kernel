#include "common-chax.h"

#define LOCAL_TRACE 1

STATIC_DECLAR void TeleportationAction_MoveCameraOnSubject(ProcPtr proc)
{
    EnsureCameraOntoPosition(proc, gActiveUnit->xPos, gActiveUnit->yPos);
}

STATIC_DECLAR void TeleportationAction_AnimateSubjectIdle(ProcPtr proc)
{
    struct MUProc * mu = MU_GetByUnit(gActiveUnit);
    if (!mu)
        mu = MU_Create(gActiveUnit);

    MU_StartActionAnim(mu);
}

void TeleportationAction_SelfWarpOutMuHide(ProcPtr proc)
{
    MU_Hide(MU_GetByUnit(gActiveUnit));
}

const struct ProcCmd ProcScr_SpellAssocSelfWarpMuHide[] = {
    PROC_SLEEP(0x5),
    PROC_CALL(TeleportationAction_SelfWarpOutMuHide),
    PROC_END
};

STATIC_DECLAR void TeleportationAction_SelfWarpOut(ProcPtr proc)
{
    LTRACEF("WarpOut at x=%d, y=%d", gActiveUnit->xPos, gActiveUnit->yPos);

    StartEventWarpAnim(proc, gActiveUnit->xPos, gActiveUnit->yPos, 0, 0);
    Proc_Start(ProcScr_SpellAssocSelfWarpMuHide, proc);
}

void TeleportationAction_SelfWarpInUpdateMu(ProcPtr proc)
{
    int x = gActionData.xMove;
    int y = gActionData.yMove;
    int ix = x * 16 - gBmSt.camera.x;
    int iy = y * 16 - gBmSt.camera.y;

    struct MUProc * pmu = MU_GetByUnit(gActiveUnit);

    gActiveUnit->xPos = x;
    gActiveUnit->yPos = y;

    MU_SetDisplayPosition(pmu, ix, iy);
}

void TeleportationAction_SelfWarpOutMuShow(ProcPtr proc)
{
    MU_Show(MU_GetByUnit(gActiveUnit));
}

const struct ProcCmd ProcScr_SpellAssocSelfWarpMuShow[] = {
    PROC_SLEEP(0x14),
    PROC_CALL(TeleportationAction_SelfWarpOutMuShow),
    PROC_END
};

void TeleportationAction_SelfWarpIn(ProcPtr proc)
{
    LTRACEF("WarpIn at x=%d, y=%d", gActiveUnit->xPos, gActiveUnit->yPos);

    StartEventWarpAnim(proc, gActiveUnit->xPos, gActiveUnit->xPos, 1, 0);
    Proc_Start(ProcScr_SpellAssocSelfWarpMuShow, proc);
}

void TeleportationAction_SubjectResetAnim(ProcPtr proc)
{
    MU_StartDelayedFaceTarget(MU_GetByUnit(gActiveUnit));
}

const struct ProcCmd ProcScr_AiActionTeleportation[] = {
    PROC_CALL(TeleportationAction_MoveCameraOnSubject),
    PROC_SLEEP(0x2),
    PROC_CALL(TeleportationAction_AnimateSubjectIdle),
    PROC_SLEEP(0x1E),
    PROC_CALL(TeleportationAction_SelfWarpOut),
    PROC_WHILE(EventWarpAnimExists),
    PROC_SLEEP(0xA),
    PROC_CALL(TeleportationAction_SelfWarpInUpdateMu),
    PROC_CALL(TeleportationAction_MoveCameraOnSubject),
    PROC_SLEEP(0xA),
    PROC_CALL(TeleportationAction_SelfWarpIn),
    PROC_WHILE(EventWarpAnimExists),
    PROC_CALL(SetDefaultColorEffects),
    PROC_SLEEP(0x2),
    PROC_CALL(TeleportationAction_SubjectResetAnim),
    PROC_SLEEP(0x1E),
    PROC_END
};

/* Action */
bool Action_Teleportation(ProcPtr parent)
{
    Proc_StartBlocking(ProcScr_AiActionTeleportation, parent);
    return false;
}

/* AiActionConf::exec */
void AiAction_Teleportation(ProcPtr parent)
{
    LTRACEF("Teleportation AiAction to x=%d, y=%d", gAiDecision.xTarget, gAiDecision.yTarget);

    gActionData.xMove = gAiDecision.xTarget;
    gActionData.yMove = gAiDecision.yTarget;

    Proc_StartBlocking(ProcScr_AiActionTeleportation, parent);
}

/* AiActionConf:idle */
bool AiAction_TeleportationIdle(ProcPtr parent)
{
    if (Proc_Find(ProcScr_AiActionTeleportation) != NULL)
        return false;

    LTRACEF("Teleportation AiAction done at x=%d, y=%d", gActiveUnit->xPos, gActiveUnit->yPos);

    gAiDecision.xMove = gAiDecision.xTarget;
    gAiDecision.yMove = gAiDecision.yTarget;

    gActiveUnit->state &= ~US_HAS_MOVED_AI;
    gAiState.unitIt--;
    return true;
}

#include "common-chax.h"
#include "battle-system.h"
#include "combat-art.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "skill-mapanim-internal.h"

STATIC_DECLAR void MapAnimRoundAnim_DisplaySkillIcon(void)
{
    int cid, sid_atk, sid_def;
    u32 actor_icon, target_icon;
    u32 left_icon, right_icon;

    struct MAInfoFrameProc * infoproc;
    infoproc = Proc_Find(ProcScr_MapBattleInfoBox);
    if (infoproc)
    {
        struct ProcMapAnimSkillfx * procfx;
        int round = GetBattleHitRound(gManimSt.pCurrentRound) - 1;

        cid = GetEfxCombatArt(round);
        sid_atk = GetActorEfxSkill(round);
        sid_def = GetTargetEfxSkill(round);

        Debugf("cid %#x, sid-atk %#x, sid-def %#x at round %d",
                    cid, sid_atk, sid_def, round);

        actor_icon = 0;
        if (COMBART_VALID(cid))
            actor_icon = COMBART_ICON(cid);
        else if (SKILL_VALID(sid_atk))
            actor_icon = SKILL_ICON(sid_atk);

        target_icon = 0;
        if (SKILL_VALID(sid_def))
            target_icon = SKILL_ICON(sid_def);

        if (gManimSt.subjectActorId == POS_L)
        {
            left_icon  = actor_icon;
            right_icon = target_icon;
        }
        else
        {
            right_icon = actor_icon;
            left_icon  = target_icon;
        }

        Debugf("left_icon %#x right_icon %#x at round %d",
                    left_icon, right_icon, round);

        /* Left */
        if (left_icon != 0)
        {
            procfx = Proc_Start(ProcScr_MapAnimSkillfx, infoproc);
            procfx->x = infoproc->x;
            procfx->y = infoproc->y;
            procfx->icon_idx = left_icon;
            procfx->pos = POS_L;
        }

        /* Right */
        if (right_icon != 0)
        {
            procfx = Proc_Start(ProcScr_MapAnimSkillfx, infoproc);
            procfx->x = infoproc->x;
            procfx->y = infoproc->y;
            procfx->icon_idx = right_icon;
            procfx->pos = POS_R;
        }
    }
}

/* LynJump */
void MapAnim_DisplayRoundAnim(ProcPtr proc)
{
    MapAnimRoundAnim_DisplaySkillIcon();
    Proc_StartBlocking(GetItemAnim6CCode(), proc);
}

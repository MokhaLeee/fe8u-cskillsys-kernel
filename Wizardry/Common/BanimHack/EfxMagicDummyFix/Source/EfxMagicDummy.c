#include "global.h"
#include "anime.h"
#include "ekrbattle.h"
#include "efxbattle.h"
#include "efxmagic.h"

s16 GetAnimRoundType(struct Anim * anim);

void EfxDummymagicMain(struct ProcEfx * proc)
{
    struct Anim * animc = GetAnimAnotherSide(proc->anim);
    int cur, frame = EfxGetCamMovDuration();
    int time = proc->timer++;

    if (time == 0)
    {
        NewEfxSpellCast();
        NewEfxFarAttackWithDistance(proc->anim, -1);
    }

    cur = proc->timer;
    if (cur == (frame + 1))
    {
        return;
    }

    if (cur == (frame + 4))
    {
        int hitted = CheckRoundMiss(GetAnimRoundType(animc));
        animc->state3 |= ANIM_BIT3_TAKE_BACK_ENABLE | ANIM_BIT3_HIT_EFFECT_APPLIED;
        StartBattleAnimHitEffectsDefault(animc, hitted);

        if (hitted == EKR_HITTED)
            EfxPlayHittedSFX(animc);

        return;
    }

    if (cur == (frame + 0x30))
    {
        SpellFx_Finish();
        RegisterEfxSpellCastEnd();
        Proc_Break(proc);
    }
}

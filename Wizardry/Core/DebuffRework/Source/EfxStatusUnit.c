#include "global.h"
#include "anime.h"
#include "ekrbattle.h"
#include "efxbattle.h"
#include "hardware.h"
#include "mu.h"
#include "bmudisp.h"
#include "bmmap.h"
#include "constants/video-banim.h"

#include "common-chax.h"
#include "debuff.h"

static const s16 frames_lowlow[] = {
    0x10, 0x100,
    0xFFFE
};

static const s16 frames_low[] = {
    0x0,  0x28,
    0x4,  0xC,
    0x8,  0xA,
    0x10, 0x28,
    0xC,  0xC,
    0x8,  0xA,
    0x4,  0xC,
    0xFFFE
};

static const s16 frames_normal[] = {
    0x0,  0x14,
    0x4,  0x6,
    0x8,  0x5,
    0xC,  0x6,
    0x10, 0x14,
    0xC,  0x6,
    0x8,  0x5,
    0x4,  0x6,
    0xFFFE
};

static const s16 frames_fast[] = {
    0x0,  0xA,
    0x4,  0x3,
    0x8,  0x2,
    0xC,  0x3,
    0x10, 0xA,
    0xC,  0x3,
    0x8,  0x2,
    0x4,  0x3,
    0xFFFE
};


static s16 const * const frams_config[] = {
    frames_lowlow,
    frames_low,
    frames_normal,
    frames_fast
};

/* LynJump */
void EfxStatusUnitMain(struct ProcEfxStatusUnit * proc)
{
    int ret, speed;
    const struct DebuffInfo * info;

    if (proc->invalid == true)
        return;

    if (proc->debuff != proc->debuf_bak)
    {
        proc->timer = 0;
        proc->frame = 0;
        proc->debuf_bak = proc->debuff;
    }

    info = &gpDebuffInfos[proc->debuff];

    if (info->efx_config.r || info->efx_config.g || info->efx_config.b)
    {
        speed = info->efx_config.speed;
        LIMIT_AREA(speed, 0, 3);

        ret = EfxAdvanceFrameLut(
            (void *)&proc->timer,
            (void *)&proc->frame,
            frams_config[speed]
        );

        if (ret > 0)
        {
            proc->red = info->efx_config.r * ret / 0x10;
            proc->green = info->efx_config.g * ret / 0x10;
            proc->blue = info->efx_config.b * ret / 0x10;
        }

        /* Here we don't touch on vanilla perity routine */
        switch (proc->debuff) {
        case UNIT_STATUS_PETRIFY:
        case UNIT_STATUS_13:
            if (GetAnimPosition(proc->anim) == EKR_POS_L)
                EfxDecodeSplitedPalette(
                    PAL_OBJ(OBPAL_EFX_UNK_7),
                    (s8 *)gFadeComponents,
                    (s8 *)&gFadeComponents[0x30],
                    (s16 *)&gFadeComponents[0x180],
                    16, proc->red, 16);
            else
                EfxDecodeSplitedPalette(
                    PAL_OBJ(OBPAL_EFX_UNK_9),
                    (s8 *)gFadeComponents,
                    (s8 *)&gFadeComponents[0x30],
                    (s16 *)&gFadeComponents[0x2A0],
                    16, proc->red, 16);

            RefreshEntityBmMaps();
            RefreshUnitSprites();
            MU_EndAll();
            break;

        default:
            EfxStatusUnitFlashing(proc->anim, proc->red, proc->green, proc->blue);
            break;
        }

        EnablePaletteSync();
    }
}

#include "global.h"
#include "proc.h"
#include "ctc.h"
#include "bmlib.h"
#include "hardware.h"
#include "bm.h"
#include "soundwrapper.h"

#include "common-chax.h"
#include "map-anims.h"

#define GRAVITYFX_VRAMOFF 0x53C0

extern const u16 Img_HeavyGravity[];

struct HeavyGravityProc {
    PROC_HEADER;
    int timer;
    int x, y;
};

struct HeavyGravityProc2 {
    PROC_HEADER;
    int timer;
    int ix, iy;
};

STATIC_DECLAR void HeavyGravitySubAnimMain(struct HeavyGravityProc2 * proc)
{
    PutSprite(
        1,
        (proc->ix + 0x200) & 0x1FF,
        (proc->iy + 0x0FD) & 0x0FF,
        gObject_16x8,
        OAM2_PAL(0) + OAM2_CHR(GRAVITYFX_VRAMOFF / 0x20) + OAM2_LAYER(0x1)
    );

    proc->iy++;

    if (++proc->timer > 0xF)
        Proc_Break(proc);
}

STATIC_DECLAR const struct ProcCmd ProcScr_MapAnimHeavyGravitySubAnim[] = {
    PROC_NAME("MapAnimHeavyGravity2"),
    PROC_YIELD,
    PROC_REPEAT(HeavyGravitySubAnimMain),
    PROC_END
};

STATIC_DECLAR bool HeavyGravitySubAnimExists(void)
{
    return !!Proc_Find(ProcScr_MapAnimHeavyGravitySubAnim);
}

STATIC_DECLAR void HeavyGravityExecSubAnim(struct HeavyGravityProc * proc)
{
    struct HeavyGravityProc2 * child;

    switch (++proc->timer) {
    case 10:
    case 20:
    case 30:
        child = Proc_Start(ProcScr_MapAnimHeavyGravitySubAnim, proc);
        child->ix = proc->x * 0x10 - gBmSt.camera.x;
        child->iy = proc->y * 0x10 - gBmSt.camera.y;
        child->timer = 0;
        break;
    }

    if (proc->timer > 30)
        Proc_Break(proc);
}

STATIC_DECLAR const struct ProcCmd ProcScr_MapAnimHeavyGravity[] = {
    PROC_NAME("MapAnimHeavyGravity"),
    PROC_YIELD,
    PROC_REPEAT(HeavyGravityExecSubAnim),
    PROC_WHILE(HeavyGravitySubAnimExists),
    PROC_END,
};

void CallMapAnim_HeavyGravity(ProcPtr parent, int x, int y)
{
    struct HeavyGravityProc * proc;

    Debugf("x %d, y %d", x, y);

    proc = Proc_Start(ProcScr_MapAnimHeavyGravity, parent);
    proc->x = x;
    proc->y = y;
    proc->timer = 0;

    Decompress(Img_HeavyGravity, OBJ_VRAM0 + GRAVITYFX_VRAMOFF);
    PlaySoundEffect(0x86);
}

bool MapAnimHeavyGravityExists(void)
{
    return !!Proc_Find(ProcScr_MapAnimHeavyGravity);
}

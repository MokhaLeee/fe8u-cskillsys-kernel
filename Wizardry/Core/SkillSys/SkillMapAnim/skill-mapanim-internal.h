#include "common-chax.h"

struct ProcMapAnimSkillfx {
    PROC_HEADER;

    u8 pos;
    u32 icon_idx;
    int timer;
    int x, y;
};

extern const struct ProcCmd ProcScr_MapAnimSkillfx[];

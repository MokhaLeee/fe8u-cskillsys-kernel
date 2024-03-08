#include "global.h"
#include "bm.h"

#include "common-chax.h"

typedef void (* ChapterInitHookFunc)(ProcPtr);
extern ChapterInitHookFunc const ChapterInitHooks[];

void ChapterInitHook(ProcPtr proc)
{
    int i;
    for (i = 0; ChapterInitHooks[i]; i++)
        ChapterInitHooks[i](proc);

    /* Vanilla */
    BmMain_StartIntroFx(proc);
}

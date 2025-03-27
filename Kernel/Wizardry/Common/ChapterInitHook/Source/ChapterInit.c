#include "common-chax.h"

typedef void (*ChapterInitHookFunc)(ProcPtr proc);
// extern ChapterInitHookFunc const ChapterInitHooks[];
extern ChapterInitHookFunc const *const gpChapterInitHooks;

void ChapterInitHook(ProcPtr proc)
{
	int i;
	for (i = 0; gpChapterInitHooks[i]; i++)
		gpChapterInitHooks[i](proc);

	/* Vanilla */
	UndeployEveryone();
}

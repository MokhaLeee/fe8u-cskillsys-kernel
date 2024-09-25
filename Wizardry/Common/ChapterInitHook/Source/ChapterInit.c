// SPDX-License-Identifier: GPL-2.0-only

#include "common-chax.h"

typedef void (* ChapterInitHookFunc)(ProcPtr);
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

#include "common-chax.h"

/* Reset helpbox when switching unit */

LYN_REPLACE_CHECK(StartGlowBlendCtrl);
void StartGlowBlendCtrl(void)
{
#if CHAX
	sLastHbi = NULL;
	gStatScreen.help = NULL;
#endif

	Proc_Start(gProcScr_SSGlowyBlendCtrl, PROC_TREE_3);
}

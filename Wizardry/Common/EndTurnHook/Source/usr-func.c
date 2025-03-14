#include "common-chax.h"

extern bool (*gpExternalEndTurnHook)(ProcPtr proc);

bool CallExternalEndTurnHook(ProcPtr proc)
{
	if (gpExternalEndTurnHook)
		return gpExternalEndTurnHook(proc);

	return false;
}

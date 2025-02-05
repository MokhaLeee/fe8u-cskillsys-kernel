#include "common-chax.h"

#define LOCAL_TRACE 0

typedef bool (*PreAiDecideFunc_t)(void);
// extern const PreAiDecideFunc_t gPreAi1DecideHook[];
extern PreAiDecideFunc_t const *const gpPreAi1DecideHook;
// extern const PreAiDecideFunc_t gPreAi2DecideHook[];
extern PreAiDecideFunc_t const *const gpPreAi2DecideHook;

LYN_REPLACE_CHECK(AiTryExecScriptA);
bool AiTryExecScriptA(void)
{
	gpAiScriptCurrent = gpAi1Table[0][gActiveUnit->ai1];
	gpAiScriptCurrent = gpAiScriptCurrent + gActiveUnit->ai_a_pc;

	gAiScriptEnded = true;
	gAiScriptKind = AI_SCRIPT_AI1;

	LTRACEF("uid=%x, ai1=%d, pc=%d", gActiveUnit->index & 0xFF, gActiveUnit->ai1, gActiveUnit->ai_a_pc);

#if CHAX
	const PreAiDecideFunc_t *it;

	for (it = gpPreAi1DecideHook; *it; it++) {
		if ((*it)() == true)
			return true;
	}
#endif

	AiScript_Exec(&gActiveUnit->ai_a_pc);
	return gAiScriptEnded;
}

LYN_REPLACE_CHECK(AiTryExecScriptB);
bool AiTryExecScriptB(void)
{
	gpAiScriptCurrent = gpAi2Table[0][gActiveUnit->ai2];
	gpAiScriptCurrent = gpAiScriptCurrent + gActiveUnit->ai_b_pc;

	gAiScriptEnded = true;
	gAiScriptKind = AI_SCRIPT_AI2;

	LTRACEF("uid=%x, ai2=%d, pc=%d", gActiveUnit->index & 0xFF, gActiveUnit->ai2, gActiveUnit->ai_a_pc);

#if CHAX
	const PreAiDecideFunc_t *it;

	for (it = gpPreAi2DecideHook; *it; it++) {
		if ((*it)() == true)
			return true;
	}
#endif

	AiScript_Exec(&gActiveUnit->ai_b_pc);
	return gAiScriptEnded;
}

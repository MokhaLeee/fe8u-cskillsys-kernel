#pragma once

#include "common-chax.h"

struct AiActionConf {
	void (*exec)(struct CpPerformProc *proc);
	bool (*idle)(struct CpPerformProc *proc);
};

// extern const struct AiActionConf gAiActionTable[];
extern struct AiActionConf const *const gpAiActionTable;

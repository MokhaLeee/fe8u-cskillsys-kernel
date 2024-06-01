#pragma once

#include "common-chax.h"

struct AiActionConf {
    void (* exec)(struct CpPerformProc *);
    bool (* idle)(struct CpPerformProc *);
};

// extern const struct AiActionConf gAiActionTable[];
extern struct AiActionConf const * const gpAiActionTable;

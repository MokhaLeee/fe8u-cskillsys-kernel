#pragma once

#include "common-chax.h"
#include "kernel-lib.h"

struct WrankBonusConfEnt {
	u8 wtype, wrank;
	u8 _pad_[2];
	s8 bonus[BATTLE_STATUS_MAX];
};

extern struct WrankBonusConfEnt const gWrankBonusConf[];
extern struct WrankBonusConfEnt const *const gpWrankBonusConf;

const struct WrankBonusConfEnt *GetWrankBonusConf(struct Unit *unit, int wtype, int wrank);

struct WrankRtextConfEnt {
	u16 msg;
	u16 wtype;
};

extern struct WrankRtextConfEnt const gWrankRtextConf[];
extern struct WrankRtextConfEnt const *const gpWrankRtextConf;

int GetWtypeFromRTextMsg(u16 msg);

void PreBattleCalc_WrankBonus(struct BattleUnit *attacker, struct BattleUnit *defender);
void HbPopuplate_WrankBonus(struct HelpBoxProc *proc);
void DrawHelpBoxLabels_WrankBonus(void);
void DrawHelpBoxStats_WrankBonus(struct ProcHelpBoxIntro *proc);

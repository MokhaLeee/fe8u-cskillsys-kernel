#pragma once

// Some functions/variables that not in decomp header
// Later we need to fix them

void ForEachUnitInRange(void(* func)(struct Unit * unit));
void AddUnitToTargetListIfNotAllied(struct Unit * unit);
extern struct Struct030017A0 gDungeonState;
void SetupBattleMOVEUNITs(void);

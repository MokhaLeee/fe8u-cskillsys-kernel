#pragma once

#include "common-chax.h"

/* Hack */
enum NewHelpBoxType {
	NEW_HB_DEFAULT = 0,
	NEW_HB_COMBAT_ART_BKSEL,
	NEW_HB_WRANK_STATSCREEN,
};
extern int sHelpBoxType;

/* From decomp */

extern struct ProcCmd ProcScr_Helpbox_bug_08A01678[];
int DrawHelpBoxWeaponLabels(int item);
void DrawHelpBoxWeaponStats(int item);
int DrawHelpBoxStaffLabels(int item);
void DrawHelpBoxSaveMenuLabels(void);
void DrawHelpBoxWeaponStats(int item);
void DrawHelpBoxSaveMenuStats(void);

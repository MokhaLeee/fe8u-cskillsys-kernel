#pragma once

#include "common-chax.h"
#include "kernel-lib.h"

enum heros_movement_types {
	HMU_TYPE_INVALID = 0,

	HMU_PIVOT,
	HMU_REPOSITION,
	HMU_SWAP,
	HMU_SHOVE,
	HMU_SMITE,
	HMU_SWARP,

	HMU_TYPE_COUNT,
};

enum heros_movement_menu_error_reason {
	HMU_ERROR_GENERIC,
	HMU_ERROR_NOALLY,
	HMU_ERROR_TERRAIN,
};

struct ProcHmu {
	PROC_HEADER;

	struct MuProc *mu;
	int counter;
};

u8 Pivot_Usability(const struct MenuItemDef *def, int number);
int Pivot_Hover(struct MenuProc *menu, struct MenuItemProc *item);

u8 HerosMovement_UM_Usability(const struct MenuItemDef *def, int number);
int HerosMovement_UM_OnDarw(struct MenuProc *menu, struct MenuItemProc *item);
u8 HerosMovement_UM_Effect(struct MenuProc *menu, struct MenuItemProc *menuItem);
int HerosMovement_UM_Hover(struct MenuProc *menu, struct MenuItemProc *menuItem);
int HerosMovement_UM_Unhover(struct MenuProc *menu, struct MenuItemProc *menuItem);

/* Only take effect when HEROES_MOVEMENT_SKILL_REQUIRED is not set */
extern u8 const *const gpHerosMovementTypes;

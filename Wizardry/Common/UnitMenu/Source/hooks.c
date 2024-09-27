#include "common-chax.h"
#include "skill-system.h"

u8 UnitActionMenu_OnHelpBoxRe(struct MenuProc *menu, struct MenuItemProc *item)
{
	return MenuSkills_OnHelpBox(menu, item);
}

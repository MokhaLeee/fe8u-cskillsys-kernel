#include "common-chax.h"
#include "skill-system.h"

void UnitActionMenu_OnInitRe(struct MenuProc * menu)
{
    MenuSkills_OnInit(menu);
}

u8 UnitActionMenu_OnHelpBox(struct MenuProc * menu, struct MenuItemProc * item)
{
    return MenuSkills_OnHelpBox(menu, item);
}

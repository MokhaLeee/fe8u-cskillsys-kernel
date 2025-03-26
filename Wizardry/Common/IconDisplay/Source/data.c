#include "common-chax.h"
#include "icon-rework.h"
#include "combat-art.h"
#include "skill-system.h"

const IconGetterFunc IconGetters[ICON_SHEET_AMT] = {
	[ICON_SHEET_VANILLA] = IconGetterVanilla,
	[ICON_SHEET_AFFIN] = GetAffinIcon,
	[ICON_SHEET_MOUNT] = GetMountIcon,
	[ICON_SHEET_WTYPE] = GetWTypeIcon,
	[ICON_SHEET_COMBATART] = GetCombatArtIcon,
	[ICON_SHEET_SKILL0] = GetSkillIcon1,
	[ICON_SHEET_SKILL1] = GetSkillIcon2,
	[ICON_SHEET_SKILL2] = GetSkillIcon3,
	[ICON_SHEET_SKILL3] = GetSkillIcon4,
};

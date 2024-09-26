#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

#if defined(SID_HealingFocus) && (COMMON_SKILL_VALID(SID_HealingFocus))
u8 HealingFocus_Usability(const struct MenuItemDef *def, int number)
{
	if (gActiveUnit->state & US_CANTOING)
		return MENU_NOTSHOWN;

	if (GetUnitCurrentHp(gActiveUnit) == GetUnitMaxHp(gActiveUnit))
		return MENU_DISABLED;

	return MENU_ENABLED;
}

u8 HealingFocus_OnSelected(struct MenuProc *menu, struct MenuItemProc *item)
{
	if (item->availability == MENU_DISABLED) {
		MenuFrozenHelpBox(menu, MSG_MenuSkill_HealingFocus_FRtext);
		return MENU_ACT_SND6B;
	}

	gActionData.unk08 = SID_HealingFocus;
	gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;
	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

static void ActionHealingFocus_CallBack1(ProcPtr proc)
{
	struct MuProc *mu;

	mu = GetUnitMu(gActiveUnit);
	if (!mu)
		mu = StartMu(gActiveUnit);

	FreezeSpriteAnim(mu->sprite_anim);
	SetMuDefaultFacing(mu);
	SetDefaultColorEffects();

	NewSkillMapAnimMini(gActiveUnit->xPos, gActiveUnit->yPos, SID_HealingFocus, proc);
}

bool Action_HealingFocus(ProcPtr parent)
{
	CallMapAnim_HealExt(
		parent,
		gActiveUnit,
		Div(GetUnitMaxHp(gActiveUnit) * SKILL_EFF0(SID_HealingFocus), 100),
		ActionHealingFocus_CallBack1,
		NULL);
	return true;
}
#endif

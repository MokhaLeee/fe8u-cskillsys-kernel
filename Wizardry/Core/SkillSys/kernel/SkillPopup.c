#include "common-chax.h"
#include "skill-system.h"
#include "icon-rework.h"
#include "popup-reowrk.h"
#include "constants/texts.h"

#define SKILL_POPSTACK_DEPTH 8

struct PopupSkillStack {
	int cur;
	u16 sids[SKILL_POPSTACK_DEPTH];
};

extern struct PopupSkillStack sPopupSkillStack;

void ResetPopupSkillStack(void)
{
	memset(&sPopupSkillStack, 0, sizeof(struct PopupSkillStack));

	sPopupSkillStack.cur = 0;
}

void PushSkillListStack(u16 sid)
{
	if (sPopupSkillStack.cur < SKILL_POPSTACK_DEPTH)
		sPopupSkillStack.sids[sPopupSkillStack.cur++] = sid;
}

int PopSkillListStack(void)
{
	if (sPopupSkillStack.cur >= 0)
		return sPopupSkillStack.sids[sPopupSkillStack.cur--];

	return 0;
}

/**
 * Components
 */
int PoprGetLen_SkillIcon(struct PopupProc *proc, const struct PopupInstruction *inst)
{
	proc->iconX = proc->xGfxSize;
	proc->iconId = SKILL_ICON(gPopupItem);
	LoadIconPalette(0, proc->iconPalId);
	return 0x10;
}

void PoprDisp_SkillIcon(struct Text *text, const struct PopupInstruction *inst)
{
	Text_Skip(text, 0x10);
}

int PoprGetLen_SkillName(struct PopupProc *proc, const struct PopupInstruction *inst)
{
	return GetStringTextLen(GetSkillNameStr(gPopupItem));
}

void PoprDisp_SkillName(struct Text *text, const struct PopupInstruction *inst)
{
	Text_DrawString(text, GetSkillNameStr(gPopupItem));
}

/**
 * Configs
 */
bool PopR_SetupLearnSkill(void)
{
	int sid = PopSkillListStack();

	if (COMMON_SKILL_VALID(sid)) {
		SetPopupItem(sid);
		return true;
	}

	return false;
}

const struct PopupInstruction PopupScr_LearnSkill[] = {
	POPUP_SOUND(0x5A),
	POPUP_MSG(MSG_0001),
	CHAX_POPUP_SKILL_ICON,
	POPUP_SPACE(1),
	POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE), // sad, we cannot change color in ekrpopup
	CHAX_POPUP_SKILL_NAME,
	POPUP_MSG(MSG_0001),
	POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
	POPUP_MSG(MSG_PopupSkillLearned),
	POPUP_END
};

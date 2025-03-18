#include "common-chax.h"
#include "help-box.h"
#include "combat-art.h"
#include "wrank-bonus.h"

LYN_REPLACE_CHECK(HbMoveCtrl_OnIdle);
void HbMoveCtrl_OnIdle(struct HelpBoxProc *proc)
{
	u8 boxMoved = FALSE;

	DisplayUiHand(
		sHbOrigin.x*8 + proc->info->xDisplay,
		sHbOrigin.y*8 + proc->info->yDisplay);

	if (gKeyStatusPtr->repeatedKeys & DPAD_UP)
		boxMoved |= TryRelocateHbUp(proc);

	if (gKeyStatusPtr->repeatedKeys & DPAD_DOWN)
		boxMoved |= TryRelocateHbDown(proc);

	if (gKeyStatusPtr->repeatedKeys & DPAD_LEFT)
		boxMoved |= TryRelocateHbLeft(proc);

	if (gKeyStatusPtr->repeatedKeys & DPAD_RIGHT)
		boxMoved |= TryRelocateHbRight(proc);

	if (gKeyStatusPtr->newKeys & (B_BUTTON | R_BUTTON)) {
#if CHAX
		sHelpBoxType = NEW_HB_DEFAULT;
#endif

		Proc_Break((void *) proc);
		return;
	}

	if (boxMoved) {
#if CHAX
		sHelpBoxType = NEW_HB_DEFAULT;
#endif

		PlaySoundEffect(0x67);
		Proc_Goto((void *) proc, 0); // TODO: label constants?
	}
}

STATIC_DECLAR void sub_808A200_vanilla(const struct HelpBoxInfo *info)
{
	int wTextBox;
	int hTextBox;

	struct HelpBoxProc *proc = Proc_Find(ProcScr_Helpbox_bug_08A01678);

	if (!proc) {
		proc = Proc_Start(ProcScr_Helpbox_bug_08A01678, PROC_TREE_3);

		PlaySoundEffect(0x70);
		sub_808A43C(proc, info->xDisplay, info->yDisplay);
		SetHelpBoxDefaultRect(proc);
	} else {
		proc->xBoxInit = proc->xBox;
		proc->yBoxInit = proc->yBox;
		proc->wBoxInit = proc->wBoxFinal;
		proc->hBoxInit = proc->hBoxFinal;
	}

	proc->info = info;
	proc->timer = 0;
	proc->timerMax = 12;

	proc->mid = info->mid;

	SetTextFontGlyphs(1);
	GetStringTextBox(GetStringFromIndex(proc->mid), &wTextBox, &hTextBox);
	SetTextFontGlyphs(0);

	sub_808A384(proc, wTextBox, hTextBox);
	sub_808A3C4(proc, info->xDisplay, info->yDisplay);

	ClearHelpBoxText();
	StartHelpBoxTextInit(proc->item, proc->mid);

	gpHelpBoxCurrentInfo = info;
}

LYN_REPLACE_CHECK(sub_808A200);
void sub_808A200(const struct HelpBoxInfo *info)
{
	sHelpBoxType = 0;
	sub_808A200_vanilla(info);
}

LYN_REPLACE_CHECK(HelpBoxSetupstringLines);
void HelpBoxSetupstringLines(struct ProcHelpBoxIntro *proc)
{
	int item = proc->item;

	SetTextFont(&gHelpBoxSt.font);
	SetTextFontGlyphs(0);

	if (sHelpBoxType == 0) {
		/* Vanilla */
		switch (GetHelpBoxItemInfoKind(item)) {
		case HB_EXTINFO_NONE:
			proc->pretext_lines = 0;
			break;

		case HB_EXTINFO_WEAPON:
			DrawHelpBoxWeaponLabels(item);
			proc->pretext_lines = 2;
			break;

		case HB_EXTINFO_STAFF:
			DrawHelpBoxStaffLabels(item);
			proc->pretext_lines = 1;
			break;

		case HB_EXTINFO_SAVEINFO:
			DrawHelpBoxSaveMenuLabels();
			proc->pretext_lines = 1;
			break;
		}
	} else {
		/* Hack here */
		switch (sHelpBoxType) {
		case NEW_HB_COMBAT_ART_BKSEL:
			if (!GetCombatArtInfo(proc->item)->battle_status.display_en_n) {
				DrawHelpBoxCombatArtBkselLabels();
				proc->pretext_lines = 2;
			} else {
				proc->pretext_lines = 0;
			}
			break;

		case NEW_HB_WRANK_STATSCREEN:
			DrawHelpBoxLabels_WrankBonus();
			proc->pretext_lines = 3;
			break;

		default:
			break;
		}
	}

	SetTextFont(0);
	Proc_Break(proc);
}

LYN_REPLACE_CHECK(HelpBoxDrawstring);
void HelpBoxDrawstring(struct ProcHelpBoxIntro *proc)
{
	int item = proc->item;

	SetTextFont(&gHelpBoxSt.font);

	if (sHelpBoxType == 0) {
		/* Vanilla */
		switch (GetHelpBoxItemInfoKind(item)) {
		case HB_EXTINFO_WEAPON:
			DrawHelpBoxWeaponStats(item);
			break;

		case HB_EXTINFO_SAVEINFO:
			DrawHelpBoxSaveMenuStats();
			break;
		}
	} else {
		/* Hack here */
		switch (sHelpBoxType) {
		case NEW_HB_COMBAT_ART_BKSEL:
			if (!GetCombatArtInfo(proc->item)->battle_status.display_en_n)
				DrawHelpBoxCombatArtBkselStats(proc);

			break;

		case NEW_HB_WRANK_STATSCREEN:
			DrawHelpBoxStats_WrankBonus(proc);
			break;

		default:
			break;
		}
	}

	SetTextFont(0);
	Proc_Break(proc);
}

LYN_REPLACE_CHECK(sub_808A454);
int sub_808A454(int item)
{
	if (sHelpBoxType == 0) {
		/* Vanilla */
		if (item == (u16)-2)
			return 3;

		if (GetItemAttributes(item) & IA_LOCK_3)
			return 0;

		if (GetItemAttributes(item) & IA_WEAPON)
			return 1;

		if (GetItemAttributes(item) & IA_STAFF)
			return 2;
	} else {
		/* Hack here */
		switch (sHelpBoxType) {
		case NEW_HB_COMBAT_ART_BKSEL:
			if (!GetCombatArtInfo(item)->battle_status.display_en_n)
				return 2;

			break;

		case NEW_HB_WRANK_STATSCREEN:
			return 2;

		default:
			break;
		}
	}

	return 0;
}

LYN_REPLACE_CHECK(ApplyHelpBoxContentSize);
void ApplyHelpBoxContentSize(struct HelpBoxProc *proc, int width, int height)
{
	#define AUTO_ADJUST_SIZE \
	do { \
		if (width < 0x90) \
			width = 0x90; \
		if (GetStringTextLen(GetStringFromIndex(proc->mid)) > 8) \
			height += 0x20; \
		else \
			height += 0x10; \
	} while (0)

	width = 0xF0 & (width + 15); // align to 16 pixel multiple

	if (sHelpBoxType == 0) {
		/* Vanilla */
		switch (GetHelpBoxItemInfoKind(proc->item)) {
		case 1: // weapon
			AUTO_ADJUST_SIZE;
			break;

		case 2: // staff
			if (width < 0x60)
				width = 0x60;

			height += 0x10;
			break;

		case 3: // save stuff
			width = 0x80;
			height += 0x10;
			break;
		}
	} else {
		/* Hack here */
		switch (sHelpBoxType) {
		case NEW_HB_COMBAT_ART_BKSEL:
			if (!GetCombatArtInfo(proc->item)->battle_status.display_en_n)
				AUTO_ADJUST_SIZE;

			break;

		case NEW_HB_WRANK_STATSCREEN:
			AUTO_ADJUST_SIZE;
			break;

		default:
			break;
		}
	}

	proc->wBoxFinal = width;
	proc->hBoxFinal = height;

	#undef AUTO_ADJUST_SIZE
}

LYN_REPLACE_CHECK(StartHelpBoxExt);
void StartHelpBoxExt(const struct HelpBoxInfo *info, int unk)
{
	struct HelpBoxProc *proc;
	int wContent, hContent;

	proc = (void *) Proc_Find(gProcScr_HelpBox);

	if (!proc) {
		proc = (void *) Proc_Start(gProcScr_HelpBox, PROC_TREE_3);

		proc->unk52 = unk;

		SetHelpBoxInitPosition(proc, info->xDisplay, info->yDisplay);
		ResetHelpBoxInitSize(proc);
	} else {
		proc->xBoxInit = proc->xBox;
		proc->yBoxInit = proc->yBox;

		proc->wBoxInit = proc->wBox;
		proc->hBoxInit = proc->hBox;
	}

	proc->info = info;

	proc->timer	= 0;
	proc->timerMax = 12;

	proc->item = 0;
	proc->mid = info->mid;

#if CHAX
	sHelpBoxType = NEW_HB_DEFAULT;
#endif

	if (proc->info->populate)
		proc->info->populate(proc);

	SetTextFontGlyphs(1);
	GetStringTextBox(GetStringFromIndex(proc->mid), &wContent, &hContent);
	SetTextFontGlyphs(0);

	ApplyHelpBoxContentSize(proc, wContent, hContent);
	ApplyHelpBoxPosition(proc, info->xDisplay, info->yDisplay);

	ClearHelpBoxText();
	StartHelpBoxTextInit(proc->item, proc->mid);

	sLastHbi = info;
}

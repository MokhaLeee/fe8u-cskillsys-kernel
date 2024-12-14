#ifndef BMMENU_H
#define BMMENU_H

struct SelectTarget;

u8 sub_80225AC(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 MapMenu_IsSuspendCommandAvailable(const struct MenuItemDef * def, int number);
u8 MapMenu_SuspendCommand(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 CommandEffectEndPlayerPhase(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 MapMenu_UnitCommand(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 MapMenu_OptionsCommand(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 sub_802263C(void);
u8 MapMenu_StatusCommand(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 MapMenu_IsGuideCommandAvailable(const struct MenuItemDef * def, int number);
int MapMenu_GuideCommandDraw(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 MapMenu_GuideCommand(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 MapMenu_DangerZone_UnusedEffect(void);
u8 sub_8022724(void);
u8 sub_8022734(void);
u8 EffectWait(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 GenericSelection_BackToUM(ProcPtr proc, struct SelectTarget * target);
void BackToUnitMenu_CamWatch(ProcPtr proc);
void BackToUnitMenu_RestartMenu(void);
u8 GenericSelection_BackToUM_CamWait(ProcPtr proc, struct SelectTarget * target);
u8 ItemMenu_ButtonBPressed(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 RescueSelection_OnHelp(ProcPtr proc, struct SelectTarget * target);
u8 RescueUsability(const struct MenuItemDef * def, int number);
u8 RescueEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 RescueSelection_OnSelect(ProcPtr proc, struct SelectTarget * target);
u8 DropUsability(const struct MenuItemDef * def, int number);
u8 DropEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 DropSelection_OnSelect(ProcPtr proc, struct SelectTarget * target);
u8 TakeUsability(const struct MenuItemDef * def, int number);
u8 TakeEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 GiveUsability(const struct MenuItemDef * def, int number);
u8 GiveEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
void MakeUnitRescueTransferGraphics(struct Unit * from, struct Unit * to);
u8 TakeSelection_OnSelect(ProcPtr proc, struct SelectTarget * target);
u8 GiveSelection_OnSelect(ProcPtr proc, struct SelectTarget * target);
u8 UnitActionMenu_Attack(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 StartUnitBallistaSelect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 StartUnitWeaponSelect(struct MenuProc * menu, struct MenuItemProc * menuItem);
int DisplayUnitStandingAttackRange(struct MenuProc * menu, struct MenuItemProc * menuItem);
int HideMoveRangeGraphicsWrapper(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 UnknownMenu_IsAvailable(const struct MenuItemDef * def, int number);
u8 UnknownMenu_Selected(struct MenuProc * menu, struct MenuItemProc * menuItem);
int UnknownMenu_Draw(struct MenuProc * menu, struct MenuItemProc * menuItem);
int UnknownMenu_SwitchIn(struct MenuProc * menu, struct MenuItemProc * menuItem);
int BallistaRangeMenu_SwitchOut(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 AttackMapSelect_Select(ProcPtr proc, struct SelectTarget * target);
void sub_8022E38(void);
void sub_8022E54(void);
u8 AttackMapSelect_Cancel(ProcPtr proc, struct SelectTarget * target);
u8 AttackMapSelect_SwitchIn(ProcPtr proc, struct SelectTarget * target);
int AttackMapSelect_End(ProcPtr proc);
u8 ItemSubMenu_IsTradeAvailable(const struct MenuItemDef * def, int number);
u8 TradeCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 TradeSelection_OnSelect(ProcPtr proc, struct SelectTarget * target);
u8 UnitActionMenu_CanSeize(const struct MenuItemDef * def, int number);
u8 UnitActionMenu_Seize(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 VisitCommandUsability(const struct MenuItemDef * def, int number);
u8 VisitCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 sub_80230F0(const struct MenuItemDef * def);
u8 PlayCommandUsability(const struct MenuItemDef * def, int number);
u8 DanceCommandUsability(const struct MenuItemDef * def, int number);
u8 PlayCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 RefreshMapSelect_Select(ProcPtr proc, struct SelectTarget * target);
u8 ItemCommandUsability(const struct MenuItemDef * def, int number);
u8 ItemCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
int ItemSelectMenu_TextDraw(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 ItemSelectMenu_Usability(const struct MenuItemDef * def, int number);
u8 ItemSelectMenu_Effect(struct MenuProc * menu, struct MenuItemProc * menuItem);
int Menu_SwitchIn(struct MenuProc * menu, struct MenuItemProc * menuItem);
int Menu_SwitchOut_DoNothing(struct MenuProc * menu, struct MenuItemProc * menuItem);
void sub_80234AC(int x, int y);
void ItemSubMenuEnd(struct MenuProc * menu);
u8 MenuCommand_SelectNo(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 sub_8023538(struct MenuProc * menu);
u8 sub_8023550(struct MenuProc * menu);
u8 sub_80235A8(struct MenuProc * menu);
u8 ItemSubMenu_IsUseAvailable(const struct MenuItemDef * def, int number);
u8 ItemSubMenu_IsEquipAvailable(const struct MenuItemDef * def, int number);
u8 ItemSubMenu_IsDiscardAvailable(const struct MenuItemDef * def, int number);
u8 ItemSubMenu_UseItem(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 ItemSubMenu_EquipItem(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 ItemSubMenu_TradeItem(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 ItemSubMenu_DiscardItem(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 MenuCommand_SelectYes(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 BallistaRangeMenu_BallistaUsability(const struct MenuItemDef * def, int number);
int BallistaRangeMenu_Draw(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 BallistaRangeMenu_Select(struct MenuProc * menu, struct MenuItemProc * menuItem);
int FillBallistaRange(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 StaffCommandUsability(const struct MenuItemDef * def, int number);
u8 StaffCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
int StaffCommandRange(struct MenuProc * menu, struct MenuItemProc * menuItem);
int HideMoveRangeGraphicsWrapper2(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 StaffItemSelect_Usability(const struct MenuItemDef * def, int number);
u8 StaffItemSelect_Effect(struct MenuProc * menu, struct MenuItemProc * menuItem);
int StaffItemSelect_TextDraw(struct MenuProc * menu, struct MenuItemProc * menuItem);
int StaffItemSelect_OnHover(struct MenuProc * menu, struct MenuItemProc * menuItem);
int StaffItemSelect_SwitchOut(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 TalkCommandUsability(const struct MenuItemDef * def, int number);
u8 TalkCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 TalkSelection_OnSelect(ProcPtr proc, struct SelectTarget * target);
u8 SupportCommandUsability(const struct MenuItemDef * def, int number);
u8 SupportCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 SupportSelection_OnSelect(ProcPtr proc, struct SelectTarget * target);
u8 DoorCommandUsability(const struct MenuItemDef * def, int number);
u8 DoorCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 ChestCommandUsability(const struct MenuItemDef * def, int number);
u8 ChestCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 PickCommandUsability(const struct MenuItemDef * def, int number);
u8 PickCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 PickSelection_OnSelect(ProcPtr proc, struct SelectTarget * target);
bool IsAdjacentForSupply(u8 pid);
u8 SupplyUsability(const struct MenuItemDef * def, int number);
u8 SupplyCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 ArmoryCommandUsability(const struct MenuItemDef * def, int number);
u8 ArmoryCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 VendorCommandUsability(const struct MenuItemDef * def, int number);
u8 VendorCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 SecretShopCommandUsability(const struct MenuItemDef * def, int number);
u8 SecretShopCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 ArenaCommandUsability(const struct MenuItemDef * def, int number);
u8 ArenaCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 StealCommandUsability(const struct MenuItemDef * def, int number);
u8 StealCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
void StealMapSelect_Init(ProcPtr menu);
u8 StealMapSelect_SwitchIn(ProcPtr proc, struct SelectTarget * target);
u8 StealMapSelect_Select(ProcPtr proc, struct SelectTarget * target);
u8 StealItemMenuCommand_Usability(const struct MenuItemDef * def, int number);
int StealItemMenuCommand_Draw(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 StealItemMenuCommand_Effect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 SummonCommandUsability(const struct MenuItemDef * def, int number);
u8 SummonCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 SummonSelection_OnSelect(ProcPtr proc, struct SelectTarget * target);
u8 YobimaCommandUsability(const struct MenuItemDef * def, int number);
u8 YobimaCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 sub_8024564(ProcPtr proc, struct SelectTarget * target);
u8 ConvoyMenu_HelpBox(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 ItemMenu_HelpBox(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 BallistaRangeMenuHelpBox(struct MenuProc * menu, struct MenuItemProc * menuItem);
void HealMapSelect_Init(ProcPtr proc);
u8 HealMapSelect_SwitchIn(ProcPtr proc, struct SelectTarget * target);
void RescueSelection_OnConstruction(ProcPtr proc);
u8 RescueSelection_OnChange(ProcPtr proc, struct SelectTarget * target);
void DropSelection_OnConstruction(ProcPtr menu);
void sub_80246DC(void);
void GiveSelection_OnInit(ProcPtr menu);
u8 GiveSelection_OnChange(ProcPtr proc, struct SelectTarget * target);
void TakeSelection_OnInit(ProcPtr menu);
u8 TakeSelection_OnChange(ProcPtr proc, struct SelectTarget * target);
void TradeTargetSelection_OnInit(ProcPtr menu);
u8 TradeSelection_OnChange(ProcPtr proc, struct SelectTarget * target);
void TalkSupportSelection_OnInit(ProcPtr menu);
u8 TalkSupportSelection_OnChange(ProcPtr proc, struct SelectTarget * target);
void RefreshMapSelect_Init(ProcPtr menu);
u8 RefreshMapSelect_SwitchIn(ProcPtr proc, struct SelectTarget * target);
void WarpUnitMapSelect_Init(ProcPtr menu);
u8 WarpUnitMapSelect_SwitchIn(ProcPtr proc, struct SelectTarget * target);
void SummonSelection_OnInit(ProcPtr menu);
u8 SummonSelection_OnChange(ProcPtr proc, struct SelectTarget * target);
void sub_8024888(ProcPtr menu);
void sub_80248A4(void);
void PickSelection_OnInit(ProcPtr menu);
u8 RideCommandUsability(const struct MenuItemDef * def, int number);
u8 RideCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 ExitCommandUsability(const struct MenuItemDef * def, int number);
u8 ExitCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 AttackCommandUsability(const struct MenuItemDef * def, int number);
u8 AttackBallistaCommandUsability(const struct MenuItemDef * def, int number);
u8 ItemMenu_Is1stCommandAvailable(const struct MenuItemDef * def, int number);
int ItemMenu_Draw1stCommand(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 ItemMenu_Select1stCommand(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 ItemMenu_AreOtherCommandsAvailable(const struct MenuItemDef * def, int number);
int ItemMenu_DrawOtherCommands(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 ItemMenu_SelectOtherCommands(struct MenuProc * menu, struct MenuItemProc * menuItem);
int ItemMenu_SwitchIn(struct MenuProc * menu, struct MenuItemProc * menuItem);
int ItemMenu_SwitchOut_DoNothing(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 ItemMenuHelpBox(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 MapMenu_IsRecordsCommandAvailable(const struct MenuItemDef * def, int number);
u8 MapMenu_RecordsCommand(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 MapMenu_IsRetreatCommandAvailable(const struct MenuItemDef * def, int number);
u8 MapMenu_RetreatCommand(struct MenuProc * menu, struct MenuItemProc * menuItem);

u8 EscapeCommandUsability(const struct MenuItemDef * def, int number);
u8 EscapeCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);


extern struct ProcCmd CONST_DATA gProcScr_BackToUnitMenu[];

#endif // BMMENU_H

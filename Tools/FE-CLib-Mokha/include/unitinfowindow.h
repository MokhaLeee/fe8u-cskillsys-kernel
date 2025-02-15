#ifndef GUARD_UNITINFOWINDOW_H
#define GUARD_UNITINFOWINDOW_H

// ??? UnitInfoWindow_OnLoop(???);
// ??? NewUnitInfoWindow(???);
// ??? UnitInfoWindow_PositionUnitName(???);
struct UnitInfoWindowProc *UnitInfoWindow_DrawBase(struct UnitInfoWindowProc *proc, struct Unit *unit, int x, int y, int width, int lines);
int GetUnitInfoWindowX(struct Unit *unit, int width);
// ??? DrawUnitHpText(???);
void DrawUnitConText(struct Text* text, struct Unit* unit);
void DrawUnitAidText(struct Text* text, struct Unit* unit);
void PutUnitAidIconForTextAt(struct Unit* unit, int x, int y);
// ??? DrawUnitStatusText(???);
// ??? DrawUnitResChangeText(???);
// ??? DrawUnitResUnkText(???);
// ??? DrawAccuracyText(???);
void StartUnitInventoryInfoWindow(ProcPtr);
void RefreshUnitInventoryInfoWindow(struct Unit*);
void RefreshUnitStealInventoryInfoWindow(struct Unit*);
void RefreshHammerneUnitInfoWindow(struct Unit*);
void StartUnitHpInfoWindow(ProcPtr);
void RefreshUnitHpInfoWindow(struct Unit*);
void StartUnitHpStatusInfoWindow(ProcPtr proc);
void RefreshUnitHpStatusInfoWindow(struct Unit* unit);
void StartUnitResChangeInfoWindow(ProcPtr proc);
void RefreshUnitResChangeInfoWindow(struct Unit* unit);
void StartUnitStaffOffenseInfoWindow(ProcPtr proc);
void RefreshUnitStaffOffenseInfoWindow(struct Unit* unit, int number);
void StartUnitRescueInfoWindowsCore(ProcPtr parent);
void RefreshUnitTakeRescueInfoWindows(ProcPtr);
void RefreshUnitRescueInfoWindows(struct Unit*);
void RefreshUnitTakeInfoWindows(struct Unit*);
void StartUnitGiveInfoWindows(ProcPtr);
void RefreshUnitGiveInfoWindows(struct Unit*);

#endif // GUARD_UNITINFOWINDOW_H

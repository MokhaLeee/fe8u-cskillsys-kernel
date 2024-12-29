#ifndef GUARD_BMUSAILMENT_H
#define GUARD_BMUSAILMENT_H

struct UnknownBMUSAilmentProc {
    PROC_HEADER;

    /* 29 */ u8 _pad1[0x2C-0x29];
    /* 2C */ int unk_2C;
    /* 30 */ int _pad2;
    /* 34 */ int unk_34;
    /* 38 */ u8 _pad3[0x4C-0x38];

    /* 4C */ s16 unk_4C;
    /* 4E */ u8 _pad4[0x58-0x4E];

    /* 58 */ int unk_58;
};


void ApplyHazardHealing(ProcPtr, struct Unit*, int, int);
// ??? RenderMapForFogFadeIfUnitDied(???);
// ??? BeginUnitHealAnim(???);
// ??? BeginUnitPoisonDamageAnim(???);
// ??? BeginGorgonEggHatchDamageAnim(???);
void BeginUnitCritDamageAnim(struct Unit*, int);
// ??? KillAllRedUnits_Init(???);
// ??? KillAllRedUnits_Loop(???);
// ??? StatusHealEffect_OverlayBg_Init(???);
// ??? StatusHealEffect_OverlayBg_Loop(???);
// ??? StatusHealEffect_BlendedSprite_Init(???);
// ??? StatusHealEffect_BlendedSprite_Loop(???);
// ??? StatusHealEffect_BlendedSprite_Finish(???);
// ??? StatusHealEffect_BlendSpriteAnim_InitIn(???);
// ??? StatusHealEffect_BlendSpriteAnim_InitOut(???);
// ??? StatusHealEffect_BlendSpriteAnim_Loop(???);
// ??? StatusHealEffect_PalSpriteAnim_Init(???);
// ??? StatusHealEffect_PalSpriteAnim_SetOutlineIntensity(???);
// ??? StatusHealEffect_PalSpriteAnim_LoopIn(???);
// ??? StatusHealEffect_PalSpriteAnim_LoopOut(???);
// ??? StatusHealEffect_Finish(???);
// ??? StartStatusHealEffect(???);
// ??? TerrainHealDisplay_Init(???);
// ??? MassEffectDisplay_Check(???);
// ??? MassEffectDisplay_Watch(???);
// ??? TerrainHealDisplay_Display(???);
// ??? FinishDamageDisplay(???);
// ??? TerrainHealDisplay_Next(???);
// ??? PoisonDamageDisplay_Init(???);
void PoisonDamageDisplay_Display(struct UnknownBMUSAilmentProc* proc); // ??? PoisonDamageDisplay_Display(???);
void PoisonDamageDisplay_Next(struct UnknownBMUSAilmentProc* proc); // ??? PoisonDamageDisplay_Next(???);
// ??? SetClassToHatchingGorgonEgg(???);
// ??? GorgonEggHatchDisplay_Init(???);
// ??? GorgonEggHatchDisplay_Display(???);
// ??? GorgonEggHatchDisplay_Next(???);
// ??? StatusDecayDisplay_Init(???);
// ??? StatusDecayDisplay_Display(???);
// ??? StatusDecayDisplay_Next(???);
// ??? TrapDamageDisplay_Init(???);
// ??? TrapDamageDisplay_Check(???);
// ??? TrapDamageDisplay_Watch(???);
// ??? TrapDamageDisplay_Display(???);
// ??? TrapDamageDisplay_Next(???);

extern struct ProcCmd CONST_DATA gProcScr_TerrainHealDisplay[];
extern struct ProcCmd CONST_DATA gProcScr_PoisonDamageDisplay[];
extern struct ProcCmd CONST_DATA gProcScr_GorgonEggHatchDisplay[];
extern struct ProcCmd CONST_DATA gProcScr_StatusDecayDisplay[];
extern struct ProcCmd CONST_DATA gProcScr_TrapDamageDisplay[];

#endif  // GUARD_BMUSAILMENT_H

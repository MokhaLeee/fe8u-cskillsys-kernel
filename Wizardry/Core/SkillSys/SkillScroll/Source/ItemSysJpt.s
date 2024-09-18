    .INCLUDE "macro.inc"

THUMB_FUNC_START ItemUseEffectJpt_SkillScroll
ItemUseEffectJpt_SkillScroll:
    mov r0, r5
    bl ItemUseEffect_SkillScroll
    ldr r0, =0x802905C + 1
    bx r0

THUMB_FUNC_START ItemUseActionJpt_SkillScroll
ItemUseActionJpt_SkillScroll:
    mov r0, r6
    bl ItemUseAction_SkillScroll
    ldr r0, =0x802FF76 + 1
    bx r0

THUMB_FUNC_START ItemUsbilityJpt_SkillScroll
ItemUsbilityJpt_SkillScroll:
    mov r0, r4
    mov r1, r5
    bl ItemUsbility_SkillScroll
    ldr r1, =0x8028C06 + 1
    bx r1

.thumb

.include "macros.inc"
.include "gba.inc"

@ Hook at 805975C
@ 1. Round start: triggered by ekrBattleTriggerNewRoundStart
@ 2. Next round: triggered by C06 from another side banim
THUMB_FUNC_START BanimSwitch_HookOnMainASM
BanimSwitch_HookOnMainASM:
    mov r0, r7
    blh BanimSwitch_HookOnMain, r1
    ldr r2, =0x80598AF
    bx r2

@ Hook at 0x59280
@ 1. Banim trigger next round by itself (C0D)
THUMB_FUNC_START BanimSwitch_HookC0D_OnDefaultASM
BanimSwitch_HookC0D_OnDefaultASM:
    mov r0, r7 @ anim
    blh BanimSwitch_HookC0D, r2
    ldr r1, =0x0805928D
    bx r1

@ Hook at 0x5937C
THUMB_FUNC_START BanimSwitch_HookC0D_OnStandingASM
BanimSwitch_HookC0D_OnStandingASM:
    mov r0, r7 @ anim
    blh BanimSwitch_HookC0D
    ldr r1, =0x08059389
    bx r1

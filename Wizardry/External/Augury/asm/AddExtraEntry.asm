@ Hooked at 0x955B0.
@ Adds an extra entry to prep screen.
.thumb

push  {r4-r5, r14}
sub   sp, #0x4


@ Overwritten vanilla, save entry.
ldr   r1, =0x8095211            @ Executed on save.
ldr   r3, =0x579
mov   r0, #0x0
str   r0, [sp]
mov   r0, #0x2
ldr   r4, =AddPrepMenuEntry
bl    GOTO_R4

@ New entry.
@ Disable entry if TextID == 0.
mov   r4, #0x0
sub   r5, r4, #0x1
Loop:
  mov   r0, r4
  bl    PREEXT_GetAuguryText
  add   r4, #0x1
  cmp   r0, r5
  beq   Loop

cmp   r0, #0x0
bne   L1
  mov   r2, #0x1
  b     L2
L1:
  mov   r2, #0x0
L2:
mov   r0, #0x0
str   r0, [sp]
mov   r0, #0x5
ldr   r1, =PREEXT_SelectExtraEntry+0x1
ldr   r3, =PREEXT_MenuOptionTextID
lsl   r3, #0x10
lsr   r3, #0x10
ldr   r4, =AddPrepMenuEntry
bl    GOTO_R4


add   sp, #0x4
pop   {r4-r5}
pop   {r0}
bx    r0
GOTO_R4:
bx    r4

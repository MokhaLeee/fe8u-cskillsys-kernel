@ Clears BG0 through BG3
.thumb

push  {r4-r7, r14}


ldr   r4, =BG_Fill
ldr   r5, =0x800
ldr   r6, =gpBG0MapBuffer
mov   r7, #0x0
Loop:
  mov   r0, r6
  mov   r1, #0x0
  bl    GOTO_R4
  add   r6, r5
  add   r7, #0x1
  cmp   r7, #0x3
  ble   Loop

mov   r0, #0xF
ldr   r4, =EnableBackgroundSyncByMask
bl    GOTO_R4


pop   {r4-r7}
pop   {r0}
bx    r0
GOTO_R4:
bx    r4

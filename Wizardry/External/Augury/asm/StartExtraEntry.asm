@ Starts the proc related to the extra entry.
@ Replaces stat screen proc start in 0x96350 switch.
.thumb

ldr   r0, =PREEXT_ExtraEntryProc
mov   r1, r4                          @ AtMenu proc.
ldr   r2, =NewBlocking6C
bl    GOTO_R2


ldr   r0, =0x8096384
mov   r15, r0
GOTO_R2:
bx    r2

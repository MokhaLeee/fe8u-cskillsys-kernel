.thumb

@ CallARM_DecompText hook
push {lr}				@ fake
lsr r2, r0, #31			@ fake
pop {r1}
ldr r1, .l1
bx r1
nop
nop
nop
.align
.l1:

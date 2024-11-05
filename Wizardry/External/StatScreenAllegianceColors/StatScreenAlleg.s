.thumb

.macro blh to, reg
    ldr \reg, =\to
    mov lr, \reg
    .short 0xF800
.endm

.equ EnemyPal, PlayerPal+4
.equ NPCPal,   PlayerPal+8
.equ OtherPal, PlayerPal+12

.equ gpStatScreenUnit, 0x2003C08

@fetches the appropriate palette based on unit allegiance and returns to 80885a4

ldr  r1, =gpStatScreenUnit
ldr  r1,[r1]
ldrb r0,[r1,#0xB]

CheckAlleg:
mov  r1,#0xC0
and  r1,r0
ldr  r0,PlayerPal
cmp  r1,#0
beq  End @If player unit, we're done

CheckNPC:
cmp  r1,#0x40
bne  CheckEnemy
ldr  r0,NPCPal
b    End

CheckEnemy:
cmp  r1,#0x80
bne  CheckOther
ldr  r0,EnemyPal
b    End

CheckOther:
cmp  r1,#0xC0
bne  End
ldr  r0,OtherPal

End:
mov  r1,#0xc0
lsl  r1,#1
mov  r2,#0x80
ldr  r3,=0x80885a4|1
bx   r3

.align
.ltorg

PlayerPal:
@POIN PlayerPal
@POIN EnemyPal
@POIN NPCPal
@POIN OtherPal

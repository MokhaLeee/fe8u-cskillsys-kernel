.thumb

.equ GetCharDeathQuoteEntry,	0x80846E5
.equ gArenaData,				0x203A8F0
.equ ReturnPoint,				0x80835E9

@ORG $835DC
@jumpToHack
@r0 has stuff

ldr		r1, NonLethalArena	@first make sure we're actually in non lethal arena mode
cmp		r1, #0x1
bne		Continue

ldr		r1, =gArenaData
ldrb	r1, [r1, #0xA]
cmp		r1, #0x2			@this byte is 0x2 if arena loss
bne		Continue

mov		r0, #0x0			@return value
bx		lr					@and skip the rest of the function

@stuff that we replaced
Continue:
push	{r4, r5, lr}
lsl		r0, r0, #0x18
lsr		r5, r0, #0x18
mov		r0, r5
ldr		r2, =GetCharDeathQuoteEntry
bl		BXR2
ldr		r2, =ReturnPoint
BXR2:
bx		r2

.ltorg
.align
NonLethalArena:
@WORD NonLethalArena
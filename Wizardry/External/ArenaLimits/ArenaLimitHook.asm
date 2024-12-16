.thumb

@ORG $24132

@no need to push lr - we don't want to return to the function we came from, we are replacing the end of that function, so use the value on the stack instead

@stuff that we replaced
mov		r1, #0x2
cmp		r0, #0x0
beq		AfterMov
mov		r1, #0x1
AfterMov:
mov		r0, r1

cmp		r0, #0x1
bne		End 					@ if r0 is not 0x1 then we aren't allowed to arena anyway, so we're out of here

ldr		r0, CheckArenaLimits
bl		BXR0

End:
pop 	{r1}
bx 		r1

BXR0:
bx		r0

.ltorg
.align
CheckArenaLimits:

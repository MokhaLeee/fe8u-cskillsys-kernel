@ Grabs chapter's Augury textID and
@ starts (battleEvent)text engine.
@   +0x29, byte, table index.
.thumb

push  {r4-r6, r14}
mov   r5, r0


@ Clear first byte in textbuffer, once this is un-cleared by
@ BattleEventEngine, it'll indicate we can start appending text.
ldr   r0, =TextBufferOffset
mov   r1, #0x0
strb  r1, [r0]

@ Start BattleEventEngine with first relevant textID.
mov   r4, #0x0
sub   r6, r4, #0x1
Loop:
  mov   r0, r4
  bl    PREEXT_GetAuguryText
  add   r4, #0x1
  cmp   r0, r6
  beq   Loop
mov   r1, #0x29
strb  r4, [r5, r1]
ldr   r4, =CallBattleQuoteTextEvents
bl    GOTO_R4


pop   {r4-r6}
pop   {r0}
bx    r0
GOTO_R4:
bx    r4

@ Appends text based on variables.
@   +0x29, byte, table index.
.thumb

push  {r4-r7, r14}
mov   r5, r0


@ Check if any text has been written yet,
@ If so, append text.
ldr   r0, =TextBufferOffset
ldrb  r0, [r0]
cmp   r0, #0x0
beq   Return

  mov   r0, r5
  ldr   r4, =Break6CLoop
  bl    GOTO_R4
  
  mov   r0, #0x29
  ldrb  r6, [r5, r0]
  mov   r0, #0x0
  sub   r7, r0, #0x1
  Loop:
    mov   r0, r6
    bl    PREEXT_GetAuguryText
    add   r6, #0x1
    cmp   r0, r7
    beq   Loop
      cmp   r0, #0x0
      beq   Return
        mov   r5, r0
        ldr   r0, =TextBufferOffset
        ldr   r4, =strlen
        bl    GOTO_R4
        ldr   r1, =TextBufferOffset
        add   r1, r0
        mov   r0, r5
        ldr   r4, =GetStringFromIndexInBuffer
        bl    GOTO_R4
        b     Loop
  

Return:
pop   {r4-r7}
pop   {r0}
bx    r0
GOTO_R4:
bx    r4

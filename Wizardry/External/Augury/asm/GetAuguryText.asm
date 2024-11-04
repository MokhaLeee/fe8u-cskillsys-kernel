@ Returns current chapter's Augury textID.
@ Args:
@   r0: table index.
@ Returns:
@   -1      if relevant unit is not alive or flag not set.
@   0       if table entry is a terminator.
@   textID  otherwise.
.thumb

push  {r4-r7, r14}
mov   r5, r0


ldr   r0, =ChapterData
ldrb  r0, [r0, #0xE]
lsl   r0, #0x2
ldr   r1, =PREEXT_AuguryText
ldr   r0, [r1, r0]
cmp   r0, #0x0                    @ Check if pointer != NULL
beq   Return
  lsl   r1, r5, #0x2
  add   r0, r1
  ldrh  r5, [r0]                  @ UnitID or Flag. 0 if no condition.
  ldrh  r6, [r0, #0x2]            @ TextID. 0 if terminator.
  mov   r0, r6
  cmp   r5, #0x0
  beq   Return
    lsr   r0, r5, #0xF
    cmp   r0, #0x0
    beq   L2
      
      @ Return TextID if flag is set.
      lsl   r0, r5, #0x11
      lsr   r0, #0x11
      ldr   r4, =CheckFlag
      bl    GOTO_R4
      mov   r1, r0
      sub   r0, #0x1
      cmp   r1, #0x0
      beq   Return
        mov   r0, r6
        b   Return

      @ Return TextID if unit is alive.
      L2:
      ldr   r4, =GetUnitStruct
      mov   r7, #0x1
      Loop:
        mov   r0, r7
        bl    GOTO_R4
        cmp   r0, #0x0
        beq   L1
          ldr   r1, [r0]
          cmp   r1, #0x0
          beq   L1
            ldr   r0, [r0, #0xC]
            mov   r2, #0x4
            tst   r0, r2
            bne   L1
              ldrb  r0, [r1, #0x4]  @ r1 has ROM unit pointer. +0x4 byte is unitID.
              cmp   r0, r5
              bne   L1
                mov   r0, r6
                b     Return
        L1:
        add   r7, #0x1
        cmp   r7, #0xBF
        ble   Loop 
      mov   r0, #0x0
      sub   r0, #0x1


Return:
pop   {r4-r7}
pop   {r1}
bx    r1
GOTO_R4:
bx    r4

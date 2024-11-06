
@ Credit to Contro for doing the digging on this one.

@ This file defines the function `__gnu_thumb1_case_uqi`, which
@ is a library function for optimized switch statements.
@ This should not be called normally.

.thumb

.include "InternalDefinitions.s"


__gnu_thumb1_case_uqi:

  .global __gnu_thumb1_case_uqi
  .type   __gnu_thumb1_case_uqi, %function

  @ Given a case index and a return address that
  @ points to a table of case handler offset bytes,
  @ jump to a case handler.

  @ The table's entries consist of a byte, which
  @ is the offset of the case handler from the
  @ table's start, shifted right once.

  @ Inputs:
  @   r0: case index
  @   lr: pointer to jump offset table

  @ Outputs:
  @   None

  push  {r1}

  @ Fetch our table start, stripping off the THUMB bit.

  mov   r1, lr
  lsr   r1, #1
  lsl   r1, #1

  @ Fetch our case handler offset and add it to the
  @ table start.

  ldrb  r1, [r1, r0]
  lsl   r1, #1

  add   lr, r1

  @ Jump to the case handler.

  pop   {r1}
  bx    lr

  .ltorg

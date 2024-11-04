@ Executed when selecting the new entry.
@ Sets +0x33 to 5, go to label 0xA, play select sound.
.thumb

push  {r4-r7, r14}
mov   r5, r0


@ Play Sfx
ldr   r0, =ChapterData
add   r0, #0x41
ldrb  r0, [r0]
lsl   r0, #0x1E
cmp   r0, #0x0
blt   L1
  mov   r0, #0x6A
  ldr   r4, =m4aSongNumStart
  bl    GOTO_R4
L1:

@ Set +0x33 to 5.
mov   r0, r5
add   r0, #0x33
mov   r1, #0x5
strb  r1, [r0]

@ Go to label 0xA
mov   r0, r5
mov   r1, #0xA
ldr   r4, =Goto6CLabel
bl    GOTO_R4


pop   {r4-r7}
pop   {r0}
bx    r0
GOTO_R4:
bx    r4

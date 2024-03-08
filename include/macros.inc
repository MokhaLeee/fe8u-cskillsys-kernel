.MACRO ARM_FUNC_START name
    .ALIGN 2, 0
    .GLOBAL \name
    .ARM
    .TYPE \name, function
.ENDM

.MACRO ARM_FUNC_END name
    .SIZE \name, .-\name
.ENDM

.set NULL, 0

.MACRO THUMB_FUNC_START name
    .ALIGN 2, 0
    .GLOBAL \name
    .THUMB
    .THUMB_FUNC
    .TYPE \name, function
.ENDM

.MACRO THUMB_FUNC_END name
    .SIZE \name, .-\name
.ENDM


@ Generates a glue function to be used when calling
@ the specified ARM function from THUMB code.
.MACRO THUMB_INTERWORK_VENEER name
    .SECTION .glue_7t
    .GLOBAL \name\()_t
    .THUMB
    .ALIGN 2, 0
    \name\()_t:
    bx pc            @ Use a bx instruction to switch into ARM mode. The value of PC is
    nop              @ 4 bytes ahead of the current instruction, so we pad with a nop.
    .ARM
    b \name          @ Now in ARM mode, jump to the actual function
    .SECTION .text
.ENDM

.macro blh to,reg=r3
    push {\reg}
    ldr \reg,=\to
    mov r14,\reg
    .short 0xF800
    pop {\reg}
.endm

.macro fun value, name
    .global \name
    .type \name, function
    .set \name, \value
.endm

.macro dat value, name
    .global \name
    .type \name, object
    .set \name, \value
.endm

@ todo
ANFMT_NOT_FORCESPRITE = 0x80000000
ANIM_INS_TYPE_END     = 1
.macro ANIMSCR_END
    .4byte ANFMT_NOT_FORCESPRITE + ANFMT_INS_TYPE_END
.endm

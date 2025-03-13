    .INCLUDE "macros.inc"
    .SYNTAX UNIFIED
    .section .text

THUMB_FUNC_START SkillTester
SkillTester:
    ldr r3, _SkillTester
    bx r3
.global _SkillTester
.align 2, 0
_SkillTester:
    .4byte ARM_SkillTester

    .arm
    .section .rodata

.global _ARM_SkillTester_CopyStart
_ARM_SkillTester_CopyStart:
    push {r4, r5, lr}

     @ UNIT_IS_VALID
    cmp r0, #0
    beq .Lend_false
    ldr r2, [r0]
    cmp r2, #0
    beq .Lend_false

    mov r5, r0

    /**
     * Here we may not judge:
     * 1. COMMON_SKILL_VALID(sid)
     * 2. SKILL_INDEX_LIST(sid) valid
     */
    mov r2, r1
    lsr r3, r1, #8          // r3 = sid_list
    add pc, pc, r3, lsl #2

.4byte 0

    /**
     * 1. Only skill index range from 0x01~0xFE can be found in RAM list
     * 2. person/job list aims to support all 0x400 skills
     * 3. item list is limited to judge only skill index 0x300~0x400 on consideration of performance issue 
     */

.Ltesters:
    b _SkillTester_Generic
    b _SkillTester_COMMON
    b _SkillTester_COMMON
    b _SkillTester_IInfo

.Lend_true:
    mov r0, #1
    pop {r4, r5, lr}
    bx lr

.Lend_false:
    mov r0, #0
    pop {r4, r5, lr}
    bx lr

_SkillTester_Generic:
    add r1, r0, #0x32
    ldrb r3, [r1], #1
    cmp r2, r3
    beq .Lend_true
    ldrb r3, [r1], #1
    cmp r2, r3
    beq .Lend_true
    ldrb r3, [r1], #1
    cmp r2, r3
    beq .Lend_true
    ldrb r3, [r1], #1
    cmp r2, r3
    beq .Lend_true
    ldrb r3, [r1], #1
    cmp r2, r3
    beq .Lend_true
    ldrb r3, [r1], #1
    cmp r2, r3
    beq .Lend_true
    ldrb r3, [r1], #1
    cmp r2, r3
    beq .Lend_true
    // b .Lend_false

_SkillTester_COMMON:
_SkillTester_PInfo:
    ldr r4, .LgpConstSkillTable_Person
    ldr r0, [r5]
    // adr lr, .Lend_false
    adr lr, _SkillTester_JInfo
    ldrb r0, [r0, #4]

.L_Table:
    ldr r1, [r4]
    add r1, r1, r0, lsl #2
    ldrh r0, [r1], #2
    cmp r2, r0
    beq .Lend_true
    ldrh r0, [r1]
    cmp r2, r0
    beq .Lend_true
    mov pc, lr

_SkillTester_JInfo:
    ldr r4, .LgpConstSkillTable_Job
    ldr r0, [r5, #4]
    ldrb r0, [r0, #4]
    adr lr, .Lend_false
    b .L_Table

_SkillTester_IInfo:
    add r3, r5, #0x1E
    ldr r4, .LgpConstSkillTable_Item
    ldrb r0, [r3], #2
    bl .L_Table
    ldrb r0, [r3], #2
    bl .L_Table
    ldrb r0, [r3], #2
    bl .L_Table
    ldrb r0, [r3], #2
    bl .L_Table
    ldrb r0, [r3], #2
    // bl .L_Table

    adr lr, _SkillTester_COMMON
    // adr lr, .Lend_false
    b .L_Table

.LgpConstSkillTable_Person:
    .4byte gpConstSkillTable_Person
.LgpConstSkillTable_Job:
    .4byte gpConstSkillTable_Job
.LgpConstSkillTable_Item:
    .4byte gpConstSkillTable_Item

.LgBattleActor:
    .4byte gBattleActor

.global _ARM_SkillTester_CopyEnd
_ARM_SkillTester_CopyEnd:

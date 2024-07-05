    .INCLUDE "macro.inc"

    .SYNTAX UNIFIED

.global _SkillTester
_SkillTester:
    .4byte ARM_SkillTester

.global _SkillTesterEnd
_SkillTesterEnd:
    .4byte ARM_SkillTesterEnd

    .arm
    .global _ARM_SkillTester_CopyStart
_ARM_SkillTester_CopyStart:
_ARM_SkillTester:
    push {r4, lr}

     @ UNIT_IS_VALID
    cmp r0, #0
    beq .Lend_false
    ldr r2, [r0]
    cmp r2, #0
    beq .Lend_false

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
    // b _SkillTester_PInfo
    // b _SkillTester_JInfo
    b _SkillTester_COMMON
    b _SkillTester_COMMON
    b _SkillTester_IInfo

.Lend_true:
    mov r0, #1
    pop {r4, lr}
    bx lr

.Lend_false:
    mov r0, #0
    pop {r4, lr}
    bx lr

_SkillTester_Generic:
    ldrb r3, [r0, #0x32]
    cmp r2, r3
    beq .Lend_true
    ldrb r3, [r0, #0x33]
    cmp r2, r3
    beq .Lend_true
    ldrb r3, [r0, #0x34]
    cmp r2, r3
    beq .Lend_true
    ldrb r3, [r0, #0x35]
    cmp r2, r3
    beq .Lend_true
    ldrb r3, [r0, #0x36]
    cmp r2, r3
    beq .Lend_true
    ldrb r3, [r0, #0x37]
    cmp r2, r3
    beq .Lend_true
    ldrb r3, [r0, #0x38]
    cmp r2, r3
    beq .Lend_true
    // b .Lend_false

_SkillTester_COMMON:
_SkillTester_PInfo:
    ldr r4, .LgpConstSkillTable_Person
    ldr r0, [r0]
    // adr lr, .Lend_false
    adr lr, _SkillTester_JInfo

.LPJ_Tabtle:
    ldrb r0, [r0, #4]

.L_Table:
    ldr r1, [r4]
    add r1, r1, r0, lsl #2
    ldrh r0, [r1]
    cmp r2, r0
    beq .Lend_true
    ldrh r0, [r1, #2]
    cmp r2, r0
    beq .Lend_true
    mov pc, lr

_SkillTester_JInfo:
    ldr r4, .LgpConstSkillTable_Job
    ldr r0, [r0, #4]
    adr lr, .Lend_false
    b .LPJ_Tabtle

_SkillTester_IInfo:
    add r3, r0, #0x1E
    ldr r4, .LgpConstSkillTable_Item
    ldrb r0, [r3]
    bl .L_Table
    ldrb r0, [r3, #2]
    bl .L_Table
    ldrb r0, [r3, #4]
    bl .L_Table
    ldrb r0, [r3, #6]
    bl .L_Table
    ldrb r0, [r3, #8]
    bl .L_Table
    // b .Lend_false
    b _SkillTester_COMMON

.LgpConstSkillTable_Person:
    .4byte gpConstSkillTable_Person
.LgpConstSkillTable_Job:
    .4byte gpConstSkillTable_Job
.LgpConstSkillTable_Item:
    .4byte gpConstSkillTable_Item

.global _ARM_SkillTester_CopyEnd
_ARM_SkillTester_CopyEnd:

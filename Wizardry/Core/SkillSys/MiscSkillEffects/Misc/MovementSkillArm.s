    .INCLUDE "macro.inc"
    .SYNTAX UNIFIED

    .arm
    .section .rodata

    .global _ARM_MapFloodCore_CopyStart
_ARM_MapFloodCore_CopyStart:

MapFloodCoreRe: @ 0x08000874
    push {r4, r5, r6, lr}
    mov r4, #0
    ldr r5, .LMovMapFillState  @ gMovMapFillState
.LMapFloodCoreReLoop:
    eors r4, r4, #1
    beq _0800089C
    ldr r0, .LMovMapFillStPool1  @ gMovMapFillStPool1
    str r0, [r5]
    ldr r0, .LMovMapFillStPool2  @ gMovMapFillStPool2
    str r0, [r5, #4]
    b _080008AC
_0800089C:
    ldr r0, .LMovMapFillStPool2  @ gMovMapFillStPool2
    str r0, [r5]
    ldr r0, .LMovMapFillStPool1  @ gMovMapFillStPool1
    str r0, [r5, #4]
_080008AC:
    ldr r6, [r5]
    ldrb r6, [r6, #2]
    cmp r6, #4
    beq _08000A18
_080008BC:
    ldr r6, [r5]
    ldrb r6, [r6, #2]
    mov r0, pc
    add r0, r0, #8
    add r0, r0, r6, lsl #2
    bx r0
    b _08000994
    b _080009C8
    b _08000960
    b _0800092C
    b _08000A14
_080008E8:
    mov r0, #3
    mov r1, #0
    mvn r2, #0
    bl MapFloodCoreReStep
    mov r0, #2
    mov r1, #0
    mov r2, #1
    bl MapFloodCoreReStep
    mov r0, #0
    mvn r1, #0
    mov r2, #0
    bl MapFloodCoreReStep
    mov r0, #1
    mov r1, #1
    mov r2, #0
    bl MapFloodCoreReStep
    b _080009F8
_0800092C:
    mov r0, #3
    mov r1, #0
    mvn r2, #0
    bl MapFloodCoreReStep
    mov r0, #0
    mvn r1, #0
    mov r2, #0
    bl MapFloodCoreReStep
    mov r0, #1
    mov r1, #1
    mov r2, #0
    bl MapFloodCoreReStep
    b _080009F8
_08000960:
    mov r0, #2
    mov r1, #0
    mov r2, #1
    bl MapFloodCoreReStep
    mov r0, #0
    mvn r1, #0
    mov r2, #0
    bl MapFloodCoreReStep
    mov r0, #1
    mov r1, #1
    mov r2, #0
    bl MapFloodCoreReStep
    b _080009F8
_08000994:
    mov r0, #3
    mov r1, #0
    mvn r2, #0
    bl MapFloodCoreReStep
    mov r0, #2
    mov r1, #0
    mov r2, #1
    bl MapFloodCoreReStep
    mov r0, #0
    mvn r1, #0
    mov r2, #0
    bl MapFloodCoreReStep
    b _080009F8
_080009C8:
    mov r0, #3
    mov r1, #0
    mvn r2, #0
    bl MapFloodCoreReStep
    mov r0, #2
    mov r1, #0
    mov r2, #1
    bl MapFloodCoreReStep
    mov r0, #1
    mov r1, #1
    mov r2, #0
    bl MapFloodCoreReStep
_080009F8:
    ldr r6, [r5, #4]
    mov r0, #4
    strb r0, [r6, #2]
    ldr r6, [r5]
    add r6, r6, #4
    str r6, [r5]
    b _080008BC
_08000A14:
    b .LMapFloodCoreReLoop
_08000A18:
    pop {r4, r5, r6, lr}
    bx lr

MapFloodCoreReStep: @ 0x08000784
    push {r4, r5, r6, r7, r8, r9, sl}
    ldr r3, .LMovMapFillState
    ldr r4, [r3]
    ldrb r5, [r4]
    add r1, r1, r5
    ldrb r6, [r4, #1]
    add r2, r2, r6
    ldr r7, .LBmMapTerrain
    ldr r7, [r7]
    ldr r7, [r7, r2, lsl #2]
    ldrb r7, [r7, r1]
    ldr r8, .LWorkingTerrainMoveCosts
    ldrb sl, [r8, r7]
    ldr r7, .LWorkingBmMap
    ldr r7, [r7]
    ldr r9, [r7, r6, lsl #2]
    ldrb r9, [r9, r5]
    add sl, sl, r9
    ldr r9, [r7, r2, lsl #2]
    ldrb r9, [r9, r1]
    cmp sl, r9
    bhs .LMapFloodCoreReStepEnd
    ldrb r4, [r3, #8]
    tst r4, r4
    beq _08000810
    ldr r7, .LBmMapUnit
    ldr r7, [r7]
    ldr r7, [r7, r2, lsl #2]
    ldrb r7, [r7, r1]
    tst r7, r7
    beq _08000810

    @ judge on PASS
    push {r0-r3, lr}
    ldr r0, .LSID_Pass
    ldr r0, [r0]
    bl .Lfunc_MapFloodSkillTester
    cmp r0, #1
    pop {r0-r3, lr}
    beq _08000810

    ldrb r4, [r3, #0xa]
    eor r4, r4, r7
    ands r4, r4, #0x80
    bne .LMapFloodCoreReStepEnd
_08000810:
    ldrb r4, [r3, #9]
    cmp sl, r4
    bhi .LMapFloodCoreReStepEnd
    ldr r4, [r3, #4]
    strb r1, [r4]
    strb r2, [r4, #1]
    strb r0, [r4, #2]
    strb sl, [r4, #3]
    add r4, r4, #4
    str r4, [r3, #4]
    ldr r7, .LWorkingBmMap
    ldr r7, [r7]
    ldr r7, [r7, r2, lsl #2]
    strb sl, [r7, r1]
.LMapFloodCoreReStepEnd:
    pop {r4, r5, r6, r7, r8, r9, sl}
    bx lr
    .align 2, 0
.LMovMapFillStPool1: .4byte gMovMapFillStPool1 @ pool
.LMovMapFillStPool2: .4byte gMovMapFillStPool2 @ pool

.LWorkingTerrainMoveCosts: .4byte gWorkingTerrainMoveCosts @ pool
.LMovMapFillState: .4byte gMovMapFillState @ pool
.LWorkingBmMap: .4byte gWorkingBmMap @ pool
.LBmMapTerrain: .4byte gBmMapTerrain @ pool
.LBmMapUnit: .4byte gBmMapUnit @ pool

.Lfunc_MapFloodSkillTester:
    mov r1, r0
    ldrb r0, [r3, #0xA]
    ldr r2, 1f
    ldr r0, [r2, r0, lsl #2]
    ldr r2, 2f
    ldr pc, [r2]
1:
    .4byte gUnitLookup
2:
    .4byte _SkillTester


.LSID_Pass:
    .4byte gSID_Pass

    .global _ARM_MapFloodCore_CopyEnd
_ARM_MapFloodCore_CopyEnd:

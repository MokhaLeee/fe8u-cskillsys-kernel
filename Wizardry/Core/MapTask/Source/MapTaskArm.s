    .INCLUDE "macros.inc"
    .SYNTAX UNIFIED
    .section .text

THUMB_FUNC_START MapTaskPutOamHi
MapTaskPutOamHi:
    ldr r3, _MapTaskPutOamHi
    bx r3
.global _MapTaskPutOamHi
.align 2, 0
_MapTaskPutOamHi:
    .4byte ARM_MapTask + (.Lfunc_MapTaskPutOamHi - _ARM_MapTask_CopyStart)

    .arm
    .section .rodata
    .global _ARM_MapTask_CopyStart
_ARM_MapTask_CopyStart:

/**
void MapTaskPutOamHi(int type, int oam2)
{
    CallARM_PushToSecondaryOAM(
        OAM1_X(gpMapTaskConfs[type].x + MapTaskVec.x),
        OAM0_Y(gpMapTaskConfs[type].y + MapTaskVec.y),
        gpMapTaskConfs[type].obj,
        oam2
    );
}
 */

@ r0 = type
@ r1 = oam2
.Lfunc_MapTaskPutOamHi:
    push {r4, r5}
    mov r3, r1
    ldr r2, .Ldata
    ldr r2, [r2]
    lsl r0, #3
    add r2, r0              @ r2 = MapTaskConf
    ldr r5, .Ldata + 4      @ r5 = MapTaskVec

    ldrh r0, [r5], #2
    ldrh r4, [r2], #2
    add r0, r4
    lsl r0, #23
    lsr r0, #23

    ldr r1, [r5]
    ldrh r4, [r2], #2
    add r1, r4
    lsl r1, #24
    lsr r1, #24

    ldr r2, [r2]

    pop {r4, r5}
    ldr pc, .Ldata + 8

.Ldata:
    .4byte gpMapTaskConfs
    .4byte MapTaskVec
    .4byte 0x30039BC @ address of ARM function PutOamHi

    .global _ARM_MapTask_CopyEnd
_ARM_MapTask_CopyEnd:

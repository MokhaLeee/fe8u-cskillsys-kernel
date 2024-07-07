    .INCLUDE "macro.inc"
    .SYNTAX UNIFIED

    .arm
    .section .rodata

    .global _MapFloodCoreRe
_MapFloodCoreRe:
    .4byte ARM_MapFloodCoreRe + (MapFloodCoreRe - _ARM_MapFloodCore_CopyStart)

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

/*
void MapFloodCoreStep(int connexion, int xPos, int yPos)
{
    u8 uid1, uid2;
    struct MovMapFillState * st = &gMovMapFillState;

    int xsrc = st->src->xPos;
    int xdst = xsrc + xPos;
    int ysrc = st->src->yPos;
    int ydst = ysrc + yPos;
    u32 cost = gWorkingTerrainMoveCosts[gBmMapTerrain[ydst][xdst]] + gWorkingBmMap[ysrc][xsrc];

#if CHAX
    if (KernelMoveMapFlags & FMOVSTRE_BARRIER)
        cost += KernelExtMoveBarrierMap[ydst][xdst];
#endif

    if (cost > gWorkingBmMap[ydst][xdst])
        return;

    uid1 = st->unitId;
    uid2 = gBmMapUnit[ydst][xdst];
    if (!st->hasUnit || !uid2 || (uid1 ^ uid2) & 0x80) // not allied
    {
#if CHAX
        if (!(KernelMoveMapFlags & FMOVSTRE_PASS))
#else
        if (1)
#endif 
            return;
    }

    if (cost > st->movement)
    {
#if CHAX
        if (!((KernelMoveMapFlags & FMOVSTRE_PIONEER) || KernelExtMovePioneerMap[ydst][xdst] == 1))
#else
        if (1)
#endif
            return;
    }

    st->dst->xPos = xdst;
    st->dst->yPos = ydst;
    st->dst->connexion = connexion;
    st->dst->leastMoveCost = cost;
    sr->dst++;

    gWorkingBmMap[ydst][xdst] = cost;
}
*/

MapFloodCoreReStep: @ 0x08000784
    push {r4-r10, lr}
    ldr r4, .LMovMapFillState       @ r4 = st = &gMovMapFillState

    ldr r3, [r4]
    ldrb r5, [r3]                   @ r5 = xsrc = st->src->xPos
    ldrb r3, [r3, #1]               @ r3 = ysrc = st->src->yPos
    add r7, r5, r1                  @ r7 = xdst = xsrc + xPos
    add r8, r3, r2                  @ r8 = ydst = ysrc + yPos
    mov r9, r0                      @ r9 = connexion
    ldr r10, .LKernelMoveMapFlags

    ldr r0, .LBmMapTerrain
    ldr r0, [r0]
    ldr r0, [r0, r8, lsl #2]
    ldrb r0, [r0, r7]               @ gBmMapTerrain[ydst][xdst]
    ldr r1, .LWorkingTerrainMoveCosts
    ldrb r0, [r1, r0]               @ gWorkingTerrainMoveCosts[gBmMapTerrain[ydst][xdst]]

    ldr r1, .LWorkingBmMap
    ldr r6, [r1]                    @ r6 = gWorkingBmMap
    ldr r1, [r6, r3, lsl #2]
    ldrb r1, [r1, r5]               @ gWorkingBmMap[ysrc][xsrc]

    add r5, r0, r1                  @ r5 = cost = gWorkingTerrainMoveCosts[gBmMapTerrain[ydst][xdst]] + gWorkingBmMap[ysrc][xsrc]

    /**
     * Judge on kernel external cost map (Obstruct skill):
     *
     * cost += KernelExtMoveBarrierMap[KERNEL_MOV_MAP_EXTCOST][ydst][xdst];
     */
    ldr r0, [r10]
    ands r0, #2                     @ <!> CHAX: FMOVSTRE_BARRIER
    beq 1f
    ldr r1, .LKernelExtMoveBarrierMap
    ldr r1, [r1]
    ldr r1, [r1, r8, lsl #2]
    ldrb r1, [r1, r7]
    add r5, r1

1:
    ldr r6, [r6, r8, lsl #2]
    ldrb r1, [r6, r7]
    cmp r5, r1                      @ if (cost >= gWorkingBmMap[ydst][xdst]) return;
    bhs .Lstep_end

    ldrb r0, [r4, #8]               @ st->hasUnit
    cmp r0, #0
    beq 1f

    ldr r1, .LBmMapUnit
    ldr r1, [r1]
    ldr r1, [r1, r8, lsl #2]
    ldrb r1, [r1, r7]               @ uid2 = gBmMapUnit[ydst][xdst]
    cmp r1, #0
    beq 1f

    ldrb r0, [r4, #10]              @ uid1 = st->unitId
    eor r0, r0, r1
    ands r0, r0, #0x80
    beq 1f

    /**
     * Judge on Pass skill
     */
    ldr r0, [r10]
    ands r0, #1                     @ <!> CHAX: FMOVSTRE_PASS
    beq .Lstep_end

1:
    ldrb r0, [r4, #9]               @ if (cost > st->movement)
    cmp r5, r0
    bhi 1f
    b 2f

    /**
     * Judge on kernel external guide map:
     *
     * cost -= KernelExtMoveBarrierMap[ydst][xdst];
     */
1:
    ldr r0, [r10]
    ands r0, #4                     @ <!> CHAX: FMOVSTRE_PIONEER
    beq .Lstep_end
    ldr r0, .LKernelExtMovePioneerMap
    ldr r0, [r0]
    ldr r0, [r0, r8, lsl #2]
    ldrb r0, [r0, r7]
    cmp r0, #0
    beq .Lstep_end

2:
    ldr r0, [r4, #4]
    strb r7, [r0], #1               @ st->dst->xPos = xdst
    strb r8, [r0], #1               @ st->dst->yPos = ydst
    strb r9, [r0], #1               @ st->dst->connexion = connexion
    strb r5, [r0], #1               @ st->dst->leastMoveCost = cost
    str r0, [r4, #4]                @ sr->dst++

    strb r5, [r6, r7]               @ gWorkingBmMap[ydst][xdst] = cost

.Lstep_end:
    pop {r4-r10, pc}

.LMovMapFillStPool1: .4byte gMovMapFillStPool1
.LMovMapFillStPool2: .4byte gMovMapFillStPool2

.LWorkingTerrainMoveCosts: .4byte gWorkingTerrainMoveCosts
.LMovMapFillState: .4byte gMovMapFillState
.LWorkingBmMap: .4byte gWorkingBmMap
.LBmMapTerrain: .4byte gBmMapTerrain
.LBmMapUnit: .4byte gBmMapUnit

@ Kernel related
.LKernelMoveMapFlags: .4byte KernelMoveMapFlags
.LKernelExtMoveBarrierMap: .4byte KernelExtMoveBarrierMap
.LKernelExtMovePioneerMap: .4byte KernelExtMovePioneerMap

    .global _ARM_MapFloodCore_CopyEnd
_ARM_MapFloodCore_CopyEnd:

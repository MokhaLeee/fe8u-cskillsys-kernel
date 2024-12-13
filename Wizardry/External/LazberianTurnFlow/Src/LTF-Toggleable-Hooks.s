
	.thumb

	.global LTFT_StartBmMainHook
	.type   LTFT_StartBmMainHook, function

	.global LTFT_FindBmMainHook
	.type   LTFT_FindBmMainHook, function

	.global LTFT_TurnEventCheckHook
	.type   LTFT_TurnEventCheckHook, function

	.global LTFT_UpdateTrapsHook
	.type   LTFT_UpdateTrapsHook, function

	.global LTFT_GetEnemyStartCursorPositionHook
	.type   LTFT_GetEnemyStartCursorPositionHook, function

	.global LTFT_CpDecideMainHook1
	.type   LTFT_CpDecideMainHook1, function

	.global LTFT_CpDecideMainHook2
	.type   LTFT_CpDecideMainHook2, function

	.global LTFT_CpOrderMainHook
	.type   LTFT_CpOrderMainHook, function

	.global LTFT_StartBskPhaseHook
	.type   LTFT_StartBskPhaseHook, function

	.global LTFT_ListFactionTickHook
	.type   LTFT_ListFactionTickHook, function

	.global LTFT_ListFactionTickHook_Continue
	.type   LTFT_ListFactionTickHook_Continue, function

	.global LTFT_ListTerrainHealHook
	.type   LTFT_ListTerrainHealHook, function

	.global LTFT_ListPoisonDamageHook
	.type   LTFT_ListPoisonDamageHook, function

	.global LTFT_ListEggHatchesHook
	.type   LTFT_ListEggHatchesHook, function

LTFT_StartBmMainHook:
	@ replacing more than I need that way I get some free Bl-Range estate
	@ (enough for 2 extra long jumps)

	push {r4-r5, lr}

	mov r4, r0

	@ LTF STUFF BEGIN

	ldr r3, =LTFT_UpdateLTFEnable
	bl BXR3

	ldr r3, =LTFT_IsActiveNow
	bl BXR3

	lsl r0, #2
	adr r1, lProcAddrs
	ldr r0, [r0, r1]

	mov r1, #2

	@ ldr r3, =ProcStart
	ldr r3, =Proc_Start
	bl BXR3

	@ LTF STUFF END

	mov r5, r0

	str r4, [r5, #0x54]

	mov  r1, #0x28
	ldrb r0, [r4, r1]
	add  r0, #1
	strb r0, [r4, r1]

	ldr r3, =(0x080311D4+1)
	BXR3: bx r3

LTFT_FindBmMainHook:
	push {lr}

	ldr r3, =LTFT_IsActiveNow
	bl BXR3

	lsl r0, #2
	adr r1, lProcAddrs
	ldr r0, [r0, r1]

	@ ldr r3, =ProcFind
	ldr r3, =Proc_Find
	bl BXR3

	pop {r1}
	bx r1

	.align

lProcAddrs:
	@ .word gProc_MapMain @ vanilla BMAPMAIN
	.word gProc_BMapMain @ vanilla BMAPMAIN
	.word LTFTMapMain   @ LTF BMAPMAIN

LTFT_TurnEventCheckHook:
	@ BL from 0808388A

	push {r2-r3, lr} @ big hacks

	ldr r3, =LTFT_IsActiveNow
	bl BXR3

	cmp r0, #0
	beq LTFT_TurnEventCheckHook.no

	mov r0, #0
	b LTFT_TurnEventCheckHook.end

LTFT_TurnEventCheckHook.no:
	ldrb r0, [r5, #15]
	sub  r0, r6 @ was a cmp but who cares

LTFT_TurnEventCheckHook.end:
	pop {r2-r3}

	cmp r0, #0

	pop {r1}
	bx r1

LTFT_UpdateTrapsHook:
	@ jumpToHack from 080154D2

	push {r0, lr}

	ldr r3, =LTFT_IsActiveNow
	bl BXR3

	pop {r1} @ r1 = proc

	cmp r0, #0
	bne LTFT_UpdateTrapsHook.yes

	@ ldr  r0, =gChapterData
	ldr  r0, =gPlaySt
	ldrb r0, [r0, #15] @ ChapterState.phase

	ldr r3, =(0x080154D0+1)
	bx r3

LTFT_UpdateTrapsHook.yes:
	ldr r3, =(0x080154D4+1)
	bx r3

LTFT_GetEnemyStartCursorPositionHook:
	@ jumpToHack from 0801DE88

	push {r4-r6, lr}

	mov r6, r0
	mov r5, r1

	ldr r3, =LTFT_IsActiveNow
	bl BXR3

	cmp r0, #0
	beq LTFT_GetEnemyStartCursorPositionHook.return

	@ ldr r0, =gChapterData
	ldr r0, =gPlaySt

	ldr r3, =(0x0801DE90+1)
	bx  r3

LTFT_GetEnemyStartCursorPositionHook.return:
	pop {r4-r6}

	pop {r1}
	bx r1

LTFT_CpDecideMainHook1:
	@ jumpToHack from 08039B04

	ldr r3, =LTFT_IsActiveNow
	bl BXR3

	@ ldr r4, =gAiData
	ldr r4, =gAiState

	mov r1, #0 @ game code assumes this is 0 after this

	cmp r0, #0
	bne LTFT_CpDecideMainHook1.end

	mov  r0, #0x79 @ AIData.decideId
	strb r1, [r4, r0]

LTFT_CpDecideMainHook1.end:
	ldr r3, =(0x08039B0E+1)
	bx r3

LTFT_CpDecideMainHook2:
	@ jumpToHack from 08039B90

	push {r2}

	ldr r3, =LTFT_IsActiveNow
	bl BXR3

	pop {r2}

	cmp r0, #0
	bne LTFT_CpDecideMainHook.end

	ldr r0, [r2, #0xC] @ Unit.state

	mov r1, #0x4
	lsl r1, #8 @ r1 = 0x400 = US_HAS_MOVED_AI

	orr r0, r1

	str r0, [r2, #0xC] @ Unit.state

LTFT_CpDecideMainHook.end:
	ldr r3, =(0x08039B9A+1)
	bx r3

LTFT_CpOrderMainHook:
	@ replace 08039788

	push {r0, lr}

	ldr r3, =LTFT_IsActiveNow
	bl BXR3

	lsl r0, #2
	adr r1, lCpOrderFuncListAddrs
	ldr r0, [r0, r1]

	@ ldr r2, =gAiData
	ldr r2, =gAiState
	add r2, #0x78 @ AIData.nextOrder

	ldrb r1, [r2]
	add  r3, r1, #1
	strb r3, [r2]

	lsl  r1, #2

	ldr r3, [r0, r1]

	pop {r0}

	bl BXR3

	pop {r0}
	bx r0

	.align

lCpOrderFuncListAddrs:
	@ .word gCpOrderCallTable
	.word sCpOrderFuncList
	.word LTFAi_NewCpOrderFuncList

LTFT_StartBskPhaseHook:
	@ jumpToHack from 0803975C

	ldr r3, =LTFT_IsActiveNow
	bl BXR3

	lsl r0, #2
	adr r1, lBskPhaseProcsAddrs
	ldr r0, [r0, r1]

	mov r1, r6

	@ ldr r3, =ProcStartBlocking
	ldr r3, =Proc_StartBlocking
	bl BXR3

	pop {r4-r6}

	pop {r0}
	bx r0

	.align

lBskPhaseProcsAddrs:
	@ .word gProc_BerserkOrder
	.word gProcScr_BerserkCpOrder
	.word gProc_LTFAiNewBskOrder

LTFT_ListFactionTickHook:
	@ jumpToHack from 0801886C

	mov r0, r10
	push {r0}

	ldr r3, =LTFT_IsActiveNow
	bl BXR3

	cmp r0, #0
	bne LTFT_ListFactionTickHook.init_ltf

	@ ldr  r0, =gChapterData
	ldr  r0, =gPlaySt
	ldrb r0, [r0, #15] @ ChapterState.phase

	add r6, r0, #1
	add r0, #0x40
	mov r10, r0

LTFT_ListFactionTickHook.lop:
	ldr r3, =(0x08018878+1)
	bx r3

LTFT_ListFactionTickHook.init_ltf:
	mov r6, #1
	mov r0, #0xC0
	mov r10, r0

	b LTFT_ListFactionTickHook.lop

LTFT_ListFactionTickHook_Continue:
	@ jumpToHAck from 0801891C

	add r6, #1

	cmp r6, r10
	blt LTFT_ListFactionTickHook.lop

	@ end

	pop {r0}
	mov r10, r0

	ldr r3, =(0x08018928+1)
	bx r3

LTFT_ListTerrainHealHook:
	@ jumpToHack from 08025904

	push {r4-r7,lr}

	mov  r1, r8
	push {r1}

	mov r7, r0
	add r7, #1 @ r7 = arg + 0x01

	mov r1, #0x40
	add r0, r1
	mov r8, r0 @ r8 = arg + 0x40

	ldr r3, =LTFT_IsActiveNow
	bl BXR3

	cmp r0, #0
	beq LTFT_ListTerrainHealHook.lop

	@ init ltf

	mov r7, #1
	mov r0, #0xC0
	mov r8, r0

LTFT_ListTerrainHealHook.lop:
	ldr r3, =InitTargets

	mov r0, #0
	mov r1, #0

	bl BXR3

	ldr r3, =(0x08025918+1)
	bx r3

LTFT_ListPoisonDamageHook:
	@ jumpToHack from 080259EC

	push {r4-r7,lr}

	mov  r1, r8
	push {r1}

	mov r7, r0
	add r7, #1 @ r7 = arg + 0x01

	mov r1, #0x40
	add r0, r1
	mov r8, r0 @ r8 = arg + 0x40

	ldr r3, =LTFT_IsActiveNow
	bl BXR3

	cmp r0, #0
	beq LTFT_ListPoisonDamageHook.lop

	@ init ltf

	mov r7, #1
	mov r0, #0xC0
	mov r8, r0

LTFT_ListPoisonDamageHook.lop:
	ldr r3, =InitTargets

	mov r0, #0
	mov r1, #0

	bl BXR3

	ldr r3, =(0x08025A00+1)
	bx r3

LTFT_ListEggHatchesHook:
	@ jumpToHack from 08025A64

	push {r4-r7,lr}

	mov  r1, r8
	push {r1}

	mov r6, r0
	add r6, #1 @ r6 = arg + 0x01

	mov r1, #0x40
	add r7, r0, r1 @ r7 = arg + 0x40

	ldr r3, =LTFT_IsActiveNow
	bl BXR3

	cmp r0, #0
	beq LTFT_ListEggHatchesHook.lop

	@ init ltf

	mov r6, #1
	mov r7, #0xC0

LTFT_ListEggHatchesHook.lop:
	ldr r3, =InitTargets

	mov r0, #0
	mov r1, #0

	bl BXR3

	mov r0, #0x5
	lsl r0, #24

	mov r8, r0

	ldr r3, =(0x08025A84+1)
	bx r3

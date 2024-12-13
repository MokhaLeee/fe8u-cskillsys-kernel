
	.thumb

	.global IsLtfEnabled
	.type   IsLtfEnabled, function

IsLtfEnabled:
	@ INTERFACE:
	@   arguments: none
	@   returns: r0 = non-zero if LTF is enabled for the current chapter

	@ NOTE: this function is called when starting a chapter
	@ but also when resuming from a suspend save

	@ DEFAULT BEHAVIOR: checks if the current chapterid is in the 0xFF-terminated list of chapter ids at gLTFChapterEnableList.

	@ var r1 = current chapter id
	@ ldr  r1, =gChapterData
	ldr  r1, =gPlaySt
	ldrb r1, [r1, #0x0E] @ ChapterState.chapterId

	@ var r3 = chapter list it
	@ beq return_false
	@ Jester 13/12/2024 - We're always going to return true as adding new chapters doesn't seem to work right now
	beq return_true
	ldr r3, =gLTFChapterEnableList

lop:
	@ var r0 = chapter in list
	ldrb r0, [r3]

	@ return false if we reach terminator (0xFF)
	cmp r0, #0xFF
	beq return_false
	@ return true if chapter in list
	cmp r0, r1
	beq return_true

	@ increment it and lop
	add r3, #1
	b lop

return_false:
	@ return 0
	movs r0, #0
	bx lr

return_true:
	@ return 1
	movs r0, #1
	bx lr

@ Definitions

@ Functions
.global EnableBackgroundSyncByMask
.type   EnableBackgroundSyncByMask, function
.set    EnableBackgroundSyncByMask, 0x8001FAD

.global NewBlocking6C
.type   NewBlocking6C, function
.set    NewBlocking6C, 0x8002CE1

.global Break6CLoop
.type   Break6CLoop, function
.set    Break6CLoop, 0x8002E95

.global m4aSongNumStart
.type   m4aSongNumStart, function
.set    m4aSongNumStart, 0x80D01FD

.global Goto6CLabel
.type   Goto6CLabel, function
.set    Goto6CLabel, 0x8002F25

.global BG_Fill
.type   BG_Fill, function
.set    BG_Fill, 0x8001221

.global AddPrepMenuEntry
.type   AddPrepMenuEntry, function
.set    AddPrepMenuEntry, 0x8097025

.global CallBattleQuoteTextEvents
.type   CallBattleQuoteTextEvents, function
.set    CallBattleQuoteTextEvents, 0x800D285

.global GetStringFromIndexInBuffer
.type   GetStringFromIndexInBuffer, function
.set    GetStringFromIndexInBuffer, 0x800A281

.global strlen
.type   strlen, function
.set    strlen, 0x80D1D89

.global GetUnitStruct
.type   GetUnitStruct, function
.set    GetUnitStruct, 0x8019431

.global CheckFlag
.type   CheckFlag, function
.set    CheckFlag, 0x8083DA9


@ RAM locations
.global gpBG0MapBuffer
.set    gpBG0MapBuffer, 0x2022CA8

.global gpBG1MapBuffer
.set    gpBG1MapBuffer, 0x20234A8

.global gpBG2MapBuffer
.set    gpBG2MapBuffer, 0x2023CA8

.global gpBG3MapBuffer
.set    gpBG3MapBuffer, 0x20244A8

.global ChapterData
.set    ChapterData, 0x202BCF0

.global TextBufferOffset
.set    TextBufferOffset, 0x202A6AC

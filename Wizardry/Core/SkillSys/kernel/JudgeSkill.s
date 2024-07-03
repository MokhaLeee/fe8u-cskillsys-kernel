    .INCLUDE "macro.inc"

    .SYNTAX UNIFIED

THUMB_FUNC_START _SkillTester
_SkillTester:
    ldr r2, =ARM_SkillTester
    bx r2

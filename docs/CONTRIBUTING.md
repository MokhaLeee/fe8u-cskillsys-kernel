[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](https://makeapullrequest.com)

[Discussion on PR review](https://github.com/MokhaLeee/fe8u-cskillsys-kernel/issues/249)

It is looked forward that this c-skillsys could be a community project and the PR's are welcome. To maintain the code quality, please understand that each PR will be subject to a strict review before merged.

1. **Mandatory Requirement**: To minimize the impact on potential bugs in each skill effects.For PRs with new skills, you need make sure that the Kernel can still compile and run even if the skill ID is commented out. To achieve this, all skill-related code should be processed by the following macro (please refer to section 3.1 of [SkillSys.md](./SkillSys.md) for details):

```c
#if defined(SID_xxx) && (COMMON_SKILL_VALID(SID_xxx))
    // Some effects
#endif
```

2. **Mandatory Requirement**: To avoid implicit bugs in the kernel due to updates in the C-Lib, defensive programming must be applied to `lynjump`.
    - 2.1. You must inject `LYN_REPLACE_CHECK` before rewriten functions:
	```c
	LYN_REPLACE_CHECK(ComputeBattleUnitAttack);
	void ComputeBattleUnitAttack(struct BattleUnit *attacker, struct BattleUnit *defender)
	{
		// ...
	}
	```

    - 2.2. You must create a file named `LynJump.event` in the same directory and copy the lynjump related code inside:
	```event
	PUSH
	ORG $2aabc
	ALIGN 4
	WORD $46C04778 $E59FC000 $E12FFF1C
	POIN ComputeBattleUnitAttack
	POP
	```

3. Code style

CSkillSys follows the standard of [Linux kernel coding style](https://www.kernel.org/doc/html/v4.10/process/coding-style.html). Please make sure that the code you submit complies with the corresponding rules. You can use the following script to check by yourself:

```bash
./check_codingstyle.sh <path-to-your-c-file>
```

Please resolve all reported ERRORs. Also, please resolve all WARNINGs unless there is a reasonable explanation.

## Suggestion

1. **Spend more time on [decomp](https://github.com/FireEmblemUniverse/fireemblem8u) studing rather than coding.**

2. More C and less ASM

We're not completely out of assembly language programming. However, for reasons such as code portability, it's best to use C as much as possible as the primary tool for implementing functionality. If too much ASM code is introduced into a PR without convincing reason, that PR may be most likely not merged.

3. Concern the performance

Some functions that may affect the operational experience should be especially taken care off. For now, the processes I can think of:

    - Battle calc related functions
    - Status getter related functions
    - SkillTester
    - map-task function in `gProc_MapTask`
    - Generate unit movement related function (such as `GenerateUnitCompleteAttackRange`)

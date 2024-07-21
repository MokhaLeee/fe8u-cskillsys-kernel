[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](https://makeapullrequest.com)

[Discussion on PR review](https://github.com/MokhaLeee/fe8u-cskillsys-kernel/issues/249)

It is looked forward that this c-skillsys could be a community project and the PR's are welcome. To maintain the code quality, please understand that each PR will be subject to a strict review before merged.

At present, I am not proficient in specifying rules, so I will only mention two requirements and one suggestion for the time being, which may be further refined according to specific needs.

## Functionality

1. Each PR needs to be compiled and approved in the following three cases:
    - On default
    - Uncoment line-5 in [config-debug.h](../include/Configs/config-debug.h): `#define CONFIG_USE_DEBUG`
    - For PR with new skill, the skill-index be commented out in [skills.enum.txt](../include/constants/skills.enum.txt)

2. When hacking on function originated from vanilla, make sure that the function definition and the basic functionality are not changed to avoid annoying other developers who may call the function.

3. **NEVER** change the C-LIB.

## Code style

The code style should follow the standard in [fe6](https://github.com/StanHash/fe6)

1. `if`
```c
if (condition)
{
    // block
}
```

2. `*`
```c
u8 * function(u32 * arg)
{
    // block
}
```

3. comment
```c
/**
 * Some comments ...
 * Some comments ...
 */

int a = 0; // Some comments ...
int b = a + 1;

if (b == 1)
{
    c = a + ((a + 1) * (b - 1));
    return c;
}
return a;
```

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

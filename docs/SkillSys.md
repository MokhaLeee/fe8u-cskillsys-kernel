Currently, C-Skillsys theoretically allows up to **1021** skills (`0x01~0xFE, 0x100~0x3FF`) to take effect at the same time. Each unit can carry up to **21** skills at the same time, which are distributed as follows:

- **7 equipable skills**, which allows player to freely select in prepscreen, also allows the game-play designer dynamically assign skills to enemies through events.
- **2 person skills**, fixed on ROM table and searched by unit character index.
- **2 job skills**, fixed on ROM table and searched by unit class index.
- **5x2 item skills**, fixed on ROM table and searched by item index. Just like an amulet, unit can obtain skills as long as they carry the corresponding items.

Note that the skill index is also divided into certain parts of categories in order to save memory and optimize performance. The following rules are generally followed for the division of skill index:

- In order to save the RAM space especially SRAM space, **equipable skill** can only range in `0x01~0xFE`, which is consistent to tranditional SkillSystem_FE8.
- In order to improve performance, only the skills ranging in `0x300-0x3FF` can involve on Item skills judgement.
- Other skills, including person/job skills have no particular restrictions.

# 1. Judge skill

In order to improve the performance of the skill system, we provide three judge-skill functions for developers:

- `SkillTester`
- `SkillListTester`
- `BattleFastSkillTester`

| Name | Location | Security | Note |
| :--------	| :-----------	| :-----------	| :----------- |
| `SkillTester` | IWRAM | SEC | Generic usage |
| `SkillListTester` | ROM | SEC | Generic usage |
| `BattleFastSkillTester` | IWRAM | None SEC | Only used on battle-calc specially for `gBattleActor` and `gBattleTarget` |

`SkillListTester` may temporarily stores unit's skill list in a buffer. However, if different characters are checked, a new list needs to be generated, which can lead to performance loss. Therefore, for fixed character checks, it is recommended to use `SkillListTester` (for example, when checking the `gActiveUnit`). On the other hand, when compiling for each character in the team, it is best to use `SkillTester`.

It is worth noting that `SkillListTester` will determine after all skills held by the unit are aggregated. Therefore, to a certain extent, it can ignore the principle of skill index distribution to determin more skills with greater efficiency. However, performance comes at cost on memory. There holds limitation on memory for the above optimization, so we only optimized it specifically for battle units.

`BattleFastSkillTester` is the most performant but least safe function: it directly searches from a preset skill list without checking if the list is correct. This means you must handle it with caution. Currently, it can only be used during the pre-battle calculation process and specially for `gBattleActor` and `gBattleTarget`.

# 2. Skill lists

## ROM table

ROM table can be configured in:

- [SkillTable-person.c](../Data/SkillSys/SkillTable-person.c)
- [SkillTable-job.c](../Data/SkillSys/SkillTable-job.c)
- [SkillTable-item.c](../Data/SkillSys/SkillTable-item.c)

## RAM table API

As for RAM table, developers may use the following API to give unit quipable skills. Each unit can get 7 RAM skills.

```C
#include "skill-system.h"
int AddSkill(struct Unit *unit, const u16 sid);
int RemoveSkill(struct Unit *unit, const u16 sid);
```

We have also offered event macros to add quipable skill from event:

- `Evt_AddSkill(sid, pid)`: give character [**pid**] a quipable skill [**sid**].
- `Evt_AddSkillAt(sid, x, y)`: give character at position [**x, y**] a quipable skill [**sid**].
- `Evt_AddSkillSC(sid)`: give character a quipable skill [**sid**], character index is picked from slot-C.

You may also use macro `Evt_RemoveSkill, Evt_RemoveSkillAt, Evt_RemoveSkillSC` to remove quipable skills.

RAM table use the support list inside unit struct, and the support data is lied inside BWL table by patch [BwlRework](../Wizardry/Common/BwlRework/BwlRework.event).

## Learn skills

Every time you `AddSkill` to unit, he may learn that quipable skill permanently. That is to say, he can always select the skill to equip in prep-screen, regardless whether you `RemoveSkill` from him. 

The most important purpose of the modern C-SkillSys is to allow players to load and unload skills in prep-screen. To do this, each character is given a list to store the skills they have learned. This list is saved to SRAM, and may be used during prep-skill list screen.

Character may learn 10 equipable skills when they level-up to lv 5/10/15/20..., 5 from class and 5 from character himself. You may edit on such file to configure character/class to learn which skills:

- [SkillTable-generic.c](../Data/SkillSys/SkillTable-generic.c)

We have also recorded unit level regardless he was promoted. Once one unit is promoted, his level will return to 1 but the record level will not change. When unit level-up, kernel may use `recorded level + current level` to judge on which skill should she learn for character table.

Now kernel also supports the same config on lvup skills as old asm skillsystem, you can edit at [LevelUpSkillEditor.c](../Data/SkillSys/LevelUpSkillEditor.c), please refer to [SME's tutorial](https://feuniverse.us/t/the-skill-system-and-you-maximizing-your-usage-of-fe8s-most-prolific-bundle-of-wizardry/8232/5)

# 3. Develop new skill

Since the skills are currently divided into four categories, developers need to select a category in advance to place new skills first:

| category | equipable | item | others |
| :--------	| :-----------	| :-----------	| :----------- |
| index preconfig | [skills-equip.enum.txt](../include/constants/skills-equip.enum.txt) | [skills-item.enum.txt](../include/constants/skills-item.enum.txt) | [skills-others.enum.txt](../include/constants/skills-others.enum.txt) |

## Basic skill info

A skill need the following components:

- Skill index
- Skill icon
- Skill description
- (optional) Skill name



```c
struct SkillInfo {
    const u8 *icon;
    u16 name, desc;
};
```

If you want to develop a new skill, you need to add such basic infos by the following step:

### 1. Skill index

Add skill index to **index preconfig** files, which is depend on your selection which categories to place. Kernel may auto generate a unique skill index and put them to **include/constants/skills.h**.

To avoid compilation errors, you'd better to add skill index detection on effect routine to fasten the game and avoid potential warnning on compiling:

```c
#if defined(SID_xxx) && (COMMON_SKILL_VALID(SID_xxx))
    // Some effects
#endif
```

### 2. Skill icon

Add icon to [gfx directory](../Contents/Gfx/Sources/SkillIcon/), you just need to give the ***.png*** file a proper name, then kernel may automatically generate variable as `GFX_SkillIcon_*` in [constants/gfx.h](../include/constants/gfx.h).

### 3. Skill description & name

Add text to [texts.txt](../Contents/Texts/Source/kernel.txt), then kernel may automatically generate msg index in [constants/texts.h](../include/constants/texts.h).

Skill name is optional. You can also add a msg to texts, but kernel may also directly find the skill name inside skill description (by finding the character "`:`" through function [SkillDescToName()](../Wizardry/Core/SkillSys/kernel/Infos.c#L46)).

Once you have done all of the components, go to [SkillInfo.c](../Data/SkillSys/SkillInfo.c) and append your info:

```c
#if (defined(SID_TEST) && COMMON_SKILL_VALID(SID_TEST))
    [SKILL_INDEX_REAL(SID_TEST)] = {
        .name = 0,
        .desc = MSG_SKILL_TEST,
        .icon = GFX_SkillIcon_TEST,
    },
#endif
```

## Skill anim info

It is a little bit complicated if you want to make your skill show anim effect during battle.

A skill anim need the following components:

- Anim index
- Priority
- Sound index

The common API to register a skill animation is shown as below:

```c
void RegisterActorEfxSkill(int round, const u16 sid);
void RegisterTargetEfxSkill(int round, const u16 sid);
```

Then kernel may register a efxskill anim at specific round. If there has already been a skill anim registered, then we may compare the priority and let the higher one be displayed.

## Battle order related skill

Battle order calculation can be splited into:

1. The order of battle between the enemy and ally
2. Number of attacks per round

Currently, the former, which is mainly handled in function `BattleUnwind`, is fixedly handled by four skills (`Vantage`, `Desperation`, `QuickRiposte`, `DoubleLion`), the display order of the skill animation is hardcoded.

The later, which is mainly handled in function `BattleComboGenerateHits`, is now dynamically changing. Users may register a skill to display anim via function `EnqueueRoundEfxSkill`.

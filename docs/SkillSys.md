C-Skillsys now supports up to **1016** skills which are divided into **four categories** as follows:

- 254 generic skills (`0x001 ~ 0x0FE`) which can be equipped or disassembled by player/event.
- 254 person skills (`0x101 ~ 0x1FE`) which is fixed to ROM table and can only be pre-set for characters.
- 254 job skills (`0x201 ~ 0x2FE`) which is fixed to ROM table and can only be pre-set for classes.
- 254 item skills (`0x301 ~ 0x3FE`) which is fixed to ROM table and can only be pre-set for items.

Note that the last three types of skills are NOT able to load and unload dynamically.

# 1. Skill lists

Each unit can use no more than 10 skills in battle map as below:

- 7 generic skills in RAM table and equipable by player.
- 2 fixed person skills binded to character.
- 2 fixed job skills binded to class.

At the same time, characters can also gain item skills based on the items, each item can hold 2 different fixed item skills.

## ROM table

ROM table can be configured in:

- [SkillTable-person.c](../Data/SkillSys/SkillTable-person.c)
- [SkillTable-job.c](../Data/SkillSys/SkillTable-job.c)
- [SkillTable-item.c](../Data/SkillSys/SkillTable-item.c)

## RAM table API

As for RAM table, developers may use the following API to give unit generic skills. Each unit can get 7 RAM skills.

```C
#include "skill-system.h"
int AddSkill(struct Unit * unit, const u16 sid);
int RemoveSkill(struct Unit * unit, const u16 sid);
```

We have also offered event macros to add generic skill from event:

- `Evt_AddSkill(sid, pid)`: give character [**pid**] a generic skill [**sid**].
- `Evt_AddSkillAt(sid, x, y)`: give character at position [**x, y**] a generic skill [**sid**].
- `Evt_AddSkillSC(sid)`: give character a generic skill [**sid**], character index is picked from slot-C.

You may also use macro `Evt_RemoveSkill, Evt_RemoveSkillAt, Evt_RemoveSkillSC` to remove generic skills.

RAM table use the support list inside unit struct, and the support data is lied inside BWL table by patch [BwlRework](../Wizardry/Common/BwlRework/BwlRework.event).

## Learn skills

Every time you `AddSkill` to unit, he may learn that generic skill permanently. That is to say, he can always select the skill to equip in prep-screen, regardless whether you `RemoveSkill` from him. 

The most important purpose of the modern C-SkillSys is to allow players to load and unload skills in prep-screen. To do this, each character is given a list to store the skills they have learned. This list is saved to SRAM, and may be used during prep-skill list screen.

Character may learn 10 generic skills when they level-up to lv 5/10/15/20..., 5 from class and 5 from character himself. You may edit on such file to configure character/class to learn which skills:

- [SkillTable-generic.c](../Data/SkillSys/SkillTable-generic.c)

We have also recorded unit level regardless he was promoted. Once one unit is promoted, his level will return to 1 but the record level will not change. When unit level-up, kernel may use `recorded level + current level` to judge on which skill should she learn for character table.

# Develop new skill

Since the skills are currently divided into four categories, developers need to select a category in advance to place new skills first:

| category | generic | person | job | item |
| :--------	| :-----------	| :-----------	| :-----------	| :-----------	|
| index preconfig | [skills.generic.enum.txt](../include/constants/skills.generic.enum.txt) | [skills.person.enum.txt](../include/constants/skills.person.enum.txt) | [skills.job.enum.txt](../include/constants/skills.job.enum.txt) | [skills.item.enum.txt](../include/constants/skills.item.enum.txt) |

## Basic skill info

A skill need the following components:

- Skill index
- Skill icon
- Skill description
- (optional) Skill name



```c
struct SkillInfo {
    const u8 * icon;
    u16 name, desc;
};
```

If you want to develop a new skill, you need to add such basic infos by the following step:

### 1. Skill index

Add skill index to **index preconfig** files, which is depend on your selection which categories to place. Kernel may auto generate a unique skill index and put them to **include/constants/skills.h**.

It should be noted that each category can only accommodate **254** skills. If there are too many skills in a category, the skills that exceed the limit will be considered as illegal and will not take effect.

To avoid compilation errors, you'd better to add skill index detection on effect routine to fasten the game and avoid potential warnning on compiling:

```c
#if defined(SID_xxx) && (COMMON_SKILL_VALID(SID_xxx))
    // Some effects
#endif
```

### 2. Skill icon

Add icon to [gfx directory](../Contants/Gfx/Sources/SkillIcon/), you just need to give the ***.png*** file a proper name, then kernel may automatically generate variable as `GFX_SkillIcon_*` in [constants/gfx.h](../include/constants/gfx.h).

### 3. Skill description & name

Add text to [texts.txt](../Contants/Texts/Source/texts.txt), then kernel may automatically generate msg index in [constants/texts.h](../include/constants/texts.h).

Skill name is optional. You can also add a msg to texts, but kernel may also directly find the skill name inside skill description (by finding the character "`:`" through function [SkillDescToName()](../Wizardry/Core/SkillSys/kernel/Infos.c#L40)).

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

The skill anim info is stored in `SkillAnimInfos` in kernel which is defined as:

```c
struct SkillAnimInfo {
    u8 aid;
    u8 priority;
    u16 sfx;
};
```

Also depend on your category selection, put your defined anim info to [SkillAnimInfo.c](../Data/SkillSys/SkillAnimInfo.c) to give your skill an efxskill anim effect.

```c
#if (defined(SID_TEST) && COMMON_SKILL_VALID(SID_TEST))
    [SID_TEST] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif
```

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

# 1. Overview
- Supports characters gaining skill effects through their character index, class index, and items.
- Supports **BattleUnit** gaining skill effects through equipped weapons and shields.
- Some skills can be dynamically acquired (dynamic skills), allowing characters to gain dynamic skills when loading, leveling up, or promoting. Developers can also give and remove dynamic skills to units via events.
- Players can freely equip or unequip dynamic skills in the prepscreen (FE16 style).

# 2. Skill Partitioning
Unlike the old system, **cskillsys** expands the skill ID range to 0x400. The currently valid skill ID range is `0x01~0xFE, 0x100~0x3FF` (up to **1021** skills in total).
With such a large number of skills, partitioning is necessary to save space and improve performance. Some skills enjoy special effects:

- `0x001-0x0FE` **Dynamic Skills**: Only this category of skills can be freely equipped and unequipped.
- `0x300-0x3FF` **Item Skills**: Only when items hold this category of skills can they take into effect.

Other skills are categorized as **generic skills**.

# 3. Setting Skills (CHAX New List)

## 3.1 Fixed Skills
1. Fixed skills can be assigned to characters via [`gConstSkillTable_Person`](../Data/SkillSys/SkillTable-person.c) or [Skills person table patch](../Patches/PATCH_Skills_Person.txt), with each character being able to hold up to **2** fixed skills.
2. Fixed skills can be assigned to classes via [`gConstSkillTable_Job`](../Data/SkillSys/SkillTable-job.c) or [Skills job table patch](../Patches/PATCH_Skills_Job.txt), with each class being able to hold up to **2** fixed skills.
3. Fixed skills can be assigned to items via [`gConstSkillTable_Item`](../Data/SkillSys/SkillTable-item.c) or [Skills item table patch](../Patches/PATCH_Skills_Item.txt). Characters gain the corresponding skill when carrying the item. Each item can hold up to **2** fixed skills, and these skills must be within the `0x300-0x3FF` range.
4. Fixed skills can be assigned to weapons via [`gConstSkillTable_Weapon`](../Data/SkillSys/SkillTable-weapon.c) or [Skills weapon table patch](../Patches/PATCH_Skills_Weapon.txt). Battle units gain the corresponding skill when equipping the weapon. Note that these skills are **ONLY effective** for **BattleUnit**.
5. Fixed skills can be assigned to shields via [`gShieldItemTable::info::skills`](../Data/Shield/ShieldConfig.c) or [Shield item config patch](../Patches/PATCH_ShieldItem.txt). Characters gain the corresponding skill when equipping the shield. Note that these skills are **ONLY effective** for **BattleUnit**. For more details on shields, refer to [ShieldItem.md](./ShieldItem.md).

## 3.2 Dynamic Skills

1. Only **dynamic skills** can be acquired or unequipped.
2. Each character can equip up to **7** dynamic skills.
3. Players can freely equip or unequip dynamic skills in the prepscreen (FE16 style).
4. If a character acquires a new skill while already holding 7 dynamic skills, the new skill is recorded as "learned" but does not take effect immediately. Players can equip it later in the prepscreen.
5. Skills that can be learned upon level-up can be configured via [`gSkillPreloadPData`](../Data/SkillSys/SkillTable-generic.c). Characters can only learn new skills at levels 5, 10, 15, etc. Each character can learn up to 5 skills every 5 levels.
6. Skills that can be learned upon level-up for classes can be configured via [`gSkillPreloadJData`](../Data/SkillSys/SkillTable-generic.c). Characters can only learn new skills at levels 5, 10, 15, etc. Each class can learn up to 5 skills every 5 levels.
7. For character based levelup table, the reference level is based on ([total count of level-up history] + [[hidden level](../Patches/PATCH_HiddenLevel.txt)]) regardless from promotion. For class based levelup table, the reference level is directly determined by the current level.
7. Developers can assign or remove skills directly via events. Refer to: [Skill OPS patch](../Patches/PATCH_EVENTSCRIPT_SkillOps.txt).

# 4. Setting Skills (Fitting to FEB Old List)

Starting from version **3.0.0**, CSkillSys additionally supports a skill list format compatible with the old skill system. Players can configure it using FEB or modify [FebList.c](../Data/SkillSys/FebList.c). For configuration instructions, refer to this document:

**https://feuniverse.us/t/the-skill-system-and-you-maximizing-your-usage-of-fe8s-most-prolific-bundle-of-wizardry/8232/5**

Note: To maintain compatibility with the old system, the skill ID range for this list is limited to `0x01-0xFF`.

# 5. Judge skill

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

# 6. Develop new skill

Since the skills are currently divided into four categories, developers need to select a category in advance to place new skills first:

| category | equipable | item | others |
| :--------	| :-----------	| :-----------	| :----------- |
| index preconfig | [skills-equip.enum.txt](../include/constants/skills-equip.enum.txt) | [skills-item.enum.txt](../include/constants/skills-item.enum.txt) | [skills-others.enum.txt](../include/constants/skills-others.enum.txt) |

## 6.1 Basic skill info

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

### 6.1.1 Skill index

Add skill index to **index preconfig** files, which is depend on your selection which categories to place. Kernel may auto generate a unique skill index and put them to **include/constants/skills.h**.

To avoid compilation errors, you'd better to add skill index detection on effect routine to fasten the game and avoid potential warnning on compiling:

```c
#if defined(SID_xxx) && (COMMON_SKILL_VALID(SID_xxx))
    // Some effects
#endif
```

### 6.1.2 Skill icon

Add icon to [gfx directory](../Contents/Gfx/Sources/SkillIcon/), you just need to give the ***.png*** file a proper name, then kernel may automatically generate variable as `GFX_SkillIcon_*` in [constants/gfx.h](../include/constants/gfx.h).

### 6.1.3 Skill description & name

Add text to [texts.txt](../Contents/Texts/Source/kernel.txt), then kernel may automatically generate msg index in [constants/texts.h](../include/constants/texts.h).

Skill name is optional. You can also add a msg to texts, but kernel may also directly find the skill name inside skill description (by finding the character "`:`" through function [SkillDescToName()](../Wizardry/Core/SkillSys/kernel/Infos.c#L47)).

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

## 6.2 Skill anim info

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

## 6.3 Battle order related skill

Battle order calculation can be splited into:

1. The order of battle between the enemy and ally
2. Number of attacks per round

Currently, the former, which is mainly handled in function `BattleUnwind`, is fixedly handled by four skills (`Vantage`, `Desperation`, `QuickRiposte`, `DoubleLion`), the display order of the skill animation is hardcoded.

The later, which is mainly handled in function `BattleComboGenerateHits`, is now dynamically changing. Users may register a skill to display anim via function `EnqueueRoundEfxSkill`.

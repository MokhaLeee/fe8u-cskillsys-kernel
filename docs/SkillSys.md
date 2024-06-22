# 1. Skill lists

Each unit can use no more than 10 skills in battle map as below:

- 2 skills in ROM table binded to character.
- 2 skills in ROM table binded to class.
- 6 skills in RAM table and equipable by player.

## ROM table

ROM table can be configured in [RomTable.c](../Data/SkillSys/RomTable.c).

## RAM table API

As for RAM table, developers may use the following API to give unit skills. Each unit can get 7 RAM skills.

```C
#include "skill-system.h"
int AddSkill(struct Unit * unit, const u16 sid);
int RemoveSkill(struct Unit * unit, const u16 sid);
```

We have also offered event macros to add skill from event:

- `Evt_AddSkill(sid, pid)`: give character [**pid**] a skill [**sid**].
- `Evt_AddSkillAt(sid, x, y)`: give character at position [**x, y**] a skill [**sid**].
- `Evt_AddSkillSC(sid)`: give character a skill [**sid**], character index is picked from slot-C.

You may also use macro `Evt_RemoveSkill, Evt_RemoveSkillAt, Evt_RemoveSkillSC` to remove skills.

RAM table use the support list inside unit struct, and the support data is lied inside BWL table by patch [BwlRework](../Wizardry/Common/BwlRework/BwlRework.event).

## Learn skills

Every time you `AddSkill` to unit, he may learn that skill permanently. That is to say, he can always select the skill to equip in prep-screen, regardless whether you `RemoveSkill` from him. 

The most important purpose of the modern C-SkillSys is to allow players to load and unload skills in prep-screen. To do this, each character is given a list to store the skills they have learned. This list is saved to SRAM, and may be used during prep-skill list screen.

Character may learn 10 skills when they level-up to lv 5/10/15/20..., 5 from class and 5 from character himself. You may edit on [PreLoadConfig.c](../Data/SkillSys/PreLoadConfig.c) to configure character/class to learn which skills.

We have also recorded unit level regardless he was promoted. Once one unit is promoted, his level will return to 1 but the record level will not change. When unit level-up, kernel may use `recorded level + current level` to judge on which skill should she learn for character table.

# Develop new skill

## Basic skill info

A skill need the following components:

- Skill index
- Skill icon
- Skill description
- (optional) Skill name

Such basic skill info is stored in `gSkillInfos_Generic` at [SkillInfo.c](../Data/SkillSys/SkillInfo.c).

```c
struct SkillInfo {
    const u8 * icon;
    u16 name, desc;
};

extern const struct SkillInfo gSkillInfos_Generic[0x100];
```

If you want to develop a new skill, you need to add such basic infos by the following step:

### 1. Skill index

Add skill index to in [constants/skills.enum.txt](../include/constants/skills.enum.txt). [enum2h.py](../Tools/scripts/enum2h.py) may auto generate **include/constants/skills.h** and update [combo.skills.txt](../Patches/combo.skills.txt) for FEBuilder users.

A valid skill index should range from 1 ~ 254 ([`SKILL_VALID`](../include/skill-system.h#L8)).

You'd better to add skill index detection on effect routine to fasten the game and avoid potential warnning on compiling:

```c
#if defined(SID_xxx) && (SID_xxx < MAX_SKILL_NUM)
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
#if (MAX_SKILL_NUM != SID_TEST)
    [SID_TEST] = {
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

The skill anim info is stored in `gSkillAnimInfos` at [EfxSkills.c](../Data/SkillSys/EfxSkills.c).

After that, you need to call for the following API at [efx-skill.h](../include/efx-skill.h) to generate efxskill anim.

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

# Skill lists

Each unit can use no more than 10 skills in battle map as below:

- 2 skills in ROM table binded to character.
- 2 skills in ROM table binded to class.
- 6 skills in RAM table and equipable by player.

---
## 1. ROM table

ROM table can be configured in [RomTable.c](../Data/SkillSys/RomTable.c).

---
## 2. RAM table API

As for RAM table, developers may use the following API to give unit skills. Each ally can get 7 RAM skills and enemy can get 3 skills limited by SRAM size.

```C
#include "skill-system.h"
int AddSkill(struct Unit * unit, const u8 sid);
int RemoveSkill(struct Unit * unit, const u8 sid);
```

We have also offered event macros to add skill from event:

- `Evt_AddSkill(sid, pid)`: give character [**pid**] a skill [**sid**].
- `Evt_AddSkillAt(sid, x, y)`: give character at position [**x, y**] a skill [**sid**].
- `Evt_AddSkillSC(sid)`: give character a skill [**sid**], character index is picked from slot-C.

You may also use macro `Evt_RemoveSkill, Evt_RemoveSkillAt, Evt_RemoveSkillSC` to remove skills.

RAM table use the support list inside unit struct, and the support data is lied inside BWL table by patch [BwlRework](../Wizardry/Common/BwlRework/BwlRework.event). Although we offered

---
## 3. Learn skills

Every time you `AddSkill` to unit, he may learn that skill permanently. That is to say, he can always select the skill to equip in prep-screen, regardless whether you `RemoveSkill` from him. 

The most important purpose of the modern C-SkillSys is to allow players to load and unload skills in prep-screen. To do this, each character is given a list to store the skills they have learned. This list is saved to SRAM, and may be used during prep-skill list screen.

Character may learn 10 skills when they level-up to lv 5/10/15/20..., 5 from class and 5 from character himself. You may edit on [PreLoadConfig.c](../Data/SkillSys/PreLoadConfig.c) to configure character/class to learn which skills.

However, the above rules introduce the following problems. Consider such a case:

- **Lute** is promoted to **Sage** when she is level 15. Then she level up to lv-5 as a **Sage**, should she get new lv-20 character skills or lv-5?

We have also recorded unit level regardless he was promoted. Once one unit is promoted, his level will return to 1 but the record level will not change. Then when unit level-up, kernel may use `recorded level + current level` to judge on which skill should she learn for character table.

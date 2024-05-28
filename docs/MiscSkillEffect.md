# Misc skill effects

---
```
SID_LEGEND_InoriAtk
SID_LEGEND_InoriAvo
SID_LEGEND_InoriDef
SID_LEGEND_4
SID_LEGEND_5
```

Legendory skill enables unit get a decent amount of bonus. Since it's powerful, each unit can only enjoy it on one turn and can only activcate one time per chapter. At the same time, each unit can only pick one legend-skill, if he has already have one legend-skill, other legend-skills will not be activated.

For now, we have defined 5 legend-skills in [LegendSkill.c](../Data/SkillSys/LegendSkill.c).

---
```
SID_PosReturn
SID_PosSwap
```

Unit can change position on post-action stage. For allies, players can choose whether or not to use the skill effect if possible. For enemies, they will automatically enable the skill effect and change the position if possible.

---
```
SID_Thunderstorm
```

It may cause an AOE attack at the *2 * 2 circle* area of the target. This effect can only be activated if he successfully hitted the enemy.

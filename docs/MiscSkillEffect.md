# Misc skill effects

---
##  legendory skill
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
## post-action position fix

```
SID_PosReturn
SID_Lunge
```

Unit can change position on post-action stage. For allies, players can choose whether or not to use the skill effect if possible. For enemies, they will automatically enable the skill effect and change the position if possible.

---
## thunderstorm
```
SID_Thunderstorm
```

It may cause an AOE attack at the *2 * 2 circle* area of the target. This effect can only be activated if he successfully hitted the enemy.

---
## weather control skills
```
SID_Airlock
SID_Drought
SID_Drizzle
SID_SnowWarning
SID_SandStream
```

Skill above can sets the weather to a specific type at the beginning of the chapter. They have a priority order, with flags to track which weathers have been triggered. Weathers with lower priorities cannot be triggered once a higher priority weather has been. The order is as follows:

1. Airlock
2. Drought
3. Drizzle
4. SnowWarning
5. SandStream

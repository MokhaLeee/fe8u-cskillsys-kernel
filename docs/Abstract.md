# Abstract

## Skill

[Skills glossary](./SkillInfo.md)
[SkillSystem](./SkillSys.md)

## Combat art

![image](./gfx/Home_CombatArt1.png)
![image](./gfx/Home_CombatArt2.png)

FE-ThreeHoses style combat-art has been implemented. You can select art during target-selection.

- Advantages
	1. More damage/hit.
	2. Some powerful special effects depends on each arts.

- Disadvantages
	1. More consumption on weapon.
	2. Cannot double attack except in special circumstances.

## Battle calculate

- Attack speed decay: (`weight - con`) --> (`weight - (con + atk * 20%)`)
- Nosferatu hp-drain percentage: 100% --> 50%
- Critical damage correction is 300% in default, can be changed by skills.
- Effectiveness damage correct is 200% in default, can be changed by skills.
- Unit can use real damage via skills to ignore vanilla damage calculation.
- Riders may suffer `avo-20%` indoor.

## Weapon

- Unit use weapon with **S rank** may hold atk+1 and neglect attack speed decay.

## Combo-attack

When attacking the enemy within the range of the companion, ally will participate in the combo attack.

## Surrounder

When a unit is attacked and adjacent to the enemy, each side with enemy may cause unit avoid -10%. If completely surrounded (each side stands an enemy), unit may cause def-5 additionally.

Flyer in outdoor environments are not affected by this effect

## Ranged attack

For non-ballista combat, it will cause the battle actor lose hit rate with the attack distance increases.

## Gaiden style magic

FE-Gaiden style B.Mag and W.Mag implemented have been implemented. Unit can cost HP to use magic.

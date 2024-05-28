# What is CHAX

It is origined from [StanHash/FE-CHAX](https://github.com/StanHash/FE-CHAX) with the main purpose to write EngineHacks in C rather than ASM. The early work mainly focus on how to achieve elf2ea (or say, [lyn utilly](https://feuniverse.us/t/ea-asm-tool-lyn-elf2ea-if-you-will/2986)). With the progress on [Decomp](https://github.com/FireEmblemUniverse/fireemblem8u) and other tools such as [ea-dep, a dependence generator](https://github.com/StanHash/ea-dep), now we may go further with the following goals:

## Main features for player:
1. Up to **7** skills equipable for ally units, player can select skills on prepscreen.
2. FE-Engage style combo-attack, enable allies participate on attacking.
3. FE-ThreeHouses style combat-art, achieve higher damage by increasing weapon cost.
4. Some other cool AOE effects and UI interfaces.

## Main features for wizardries
1. All components are all generated at once via a `make` command.
2. Use the same *.h* headers and symbol list to decomp.
3. More C-Hacks and less ASM-Hacks.
4. Useful debug kit.
5. A faster `SkillTester()` design via [SkillList](../Wizardry/Core/SkillSys/kernel/SkillList.c).
6. Better [free-space](../include/Configs/config-memmap.h) and [free-ram-space](../Configs//usr-defined.s) control.
7. A [fixed pointer list](../Reloc/Reloc.event) to get data location, which will facilitate collaborative development with FEB.
8. Better battle-system design and battle-hit is expanded to 20.

# Support

For now, unit support progress is located in BWL table, and the vanilla support table is used to save skills. If unit does not have BWL, (aka, unit character index > 0x45), we may directly use the pre-load supports, which means its support level may not develop in game playing.

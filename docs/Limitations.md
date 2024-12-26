1. Now ally characters is limited to no more than 51. You should also make sure the character index is also in limitation, otherwise he will fail to learn and equip skills.

2. Since move-path can display no more than 20 steps in vanilla, with the increase of unit's mobility, this limitation may cause risk of overflow and thus crush the game. If you get in error on move-path, you can directly remove it by FEB patch [designer config](../Patches/PATCH_DeignerConfig.txt)

3. BattleUnit struct is only valid for `gBattleActor` and `gBattleTarget`, any other pointer is unacceptable in battle. In vanilla, there is only one function that alloc a extra battle unit data in `UnitAutolevelRealistic`, which has been rewriten in kernel.

4. The following patches provided by FEBuilderGBA hold clear conflict with c-skillsys thus strictly forbiddened (WIP).
    - Talk AI
    - Anima Triangle
    - AnimNumbers
    - AoE Area of Effect
    - Auto save function
    - Status Ailment Swords
    - Base stat based promotion for reclassing
    - When calculating the attack speed, calculate with CON + Skill / 4.
    - Change from death to HP1 with Lethality
    - Use magic motion when physical attack hits
    - Fixed Exp value of staff
    - Increase the RAM area that can record the number of clear turns
    - CritCap. Fixed critcal from appearing in excess of 100%
    - Con-Reducing Diet Drug
    - Limit Weapon Rank Display
    - Multi-Class Pick Skill Installer
    - Modern Character Growths
    - ExModularSave
    - Exclude buff states from terrain recovery
    - Remove Creature Campaign
    - Remove Link Arena Option
    - Remove Sound Room
    - Remove Support Viewer
    - Fates EXP
    - Attack and capture enemies with reduced speed like FE5
    - Rogue Robbery
    - Staff Range Fix
    - FixedGrowthsMode
    - Fourth-Allegiance
    - Range Display Fix
    - Change weapon's range text
    - HeroesMovement
    - Increase Enemy's hit rate
    - Icon Display
    - Lose Weapon Ranks on Promotion
    - Magic Sword
    - Magic Sword Rework
    - MapAddInRange Efficiency Fix
    - MeleeAndMagicFix
    - Fix Weak Promoted Enemies
    - Define Multiple Weapons That Cannot Double Attack
    - Character/Class: Level Cap Editor
    - Change to have multiple units that can call supply
    - Passive Boosts Patches
    - Change Max HP limit to use set in class
    - Staff_Heal_Exp
    - Simple CANTO Fix
    - EXP Value in dance, steal, Summons, etc
    - Allow enemy growths over 100%
    - Staff Basal Hit Value
    - Battle crit threshold
    - Status given by Filla's Might
    - Status given by Ninis's Grace
    - Status given by Set's Litany
    - Status given by Thor's Ire
    - Great Shield Activation Rate
    - Great Shield Damage Reduction
    - Great Shield First Class
    - Great Shield Last Class
    - Silencer Activation Rate vs Boss
    - Silencer Activation Rate vs Exp 0
    - Silencer Activation Rate
    - Silencer-Immune Class
    - Slayer Skill First Class
    - Slayer Skill Last Class
    - Sure Strike Skill First Class
    - Sure Strike Skill Last Class
    - Stat Bar Max Length
    - Modify Maximum Number Of Supports
    - Skill Pick
    - Skill Pierce from this class
    - Skill Pierce to this class
    - Turnwheel
    - ExModularSave

    **Please note that this does not mean that other patches are safe**. All patches related as below are dangerous, You need to carefully consider the risks on each patches.

    - unit status calculation
    - battle status calculation
    - exp calculation
    - staff
    - ring
    - skills
    - save data

5. Do **NOT** directly change the unit level in ASM.

    As unit lvup history is stored in BWL table, and directly change the unit level may not affect the unit recorded level, thus the lvup skill-learning process may not act in exception. For more info, see section: **Learn skills** in [SkillSys](./SkillSys.md).

    As an alternative, kernel offered an API if you want to change unit level in ASM:

    ```c
    // bwl.h
    void WriteUnitLevelSafe(struct Unit *unit, int new_level);
    ```

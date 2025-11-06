# Icons

Just as the old system, the following special icons are splited to seperate sheet:

- Affin icon
- Mounted attribute icon
- Weapon type icon

# Character max status correction

In vanilla, the max status is dependent on class. In cskillsys, different characters could hold different max status even if they are in the same class. For example, Eirika could hold more speed when she is a swordmaster than Joshua.

- User may use patch [Character based modular max status](../Patches/PATCH_CharMaxStatus.txt) to config.

# Critial rate bonus

In vanilla only the swordmaster hold addition with hardcoded 15% critial bonus. Cskillsys make it configurable via patch [Critial bonus](../Patches/PATCH_CriticalBonus.txt).

# Job growth correction

Same as the **FE16** feature. The units' growth is dependent not only on the character growth, but also class based growth correction. User can config the class based correction via patch [Job growth correction](../Patches/PATCH_JobGrowth.txt).

# Leadership

Same as **FE4/5** and old system, leadership stars is both a game mechanic and a character attribute. During combat, the total star count of each team is calculated and is based on the corresponding combat attributes.

- Leadership features is disabled on default, user should enable the **enabled** bit in patch [Leadership bonus config](../Patches/PATCH_LeaderShip_bonus.txt) to use leadership features.
- User can also config battle bonus in patch [Leadership bonus config](../Patches/PATCH_LeaderShip_bonus.txt).
- User may config leadership stars via patch [Leadership person config](../Patches/PATCH_LeaderShip_pinfo.txt) for characters and patch [Leadership job config](../Patches/PATCH_LeaderShip_jinfo.txt) for classes.

# Weapon hp cost/drain

- If unit want to use a weapon to attack, then he should pay part of own hp as cost to use it. User may config it via patch [Weapon hp cost percentage](../Patches/PATCH_WeaponHpCostConfig.txt).
- If unit use a weapon to hurt the enemy, he may drain part of damage to recover his own hp. User may config it via patch [Weapon hp drain percentage](../Patches/PATCH_WeaponHpDrainConfig.txt).

# Weapon rank bonus

When unit use the weapon, his weapon-type rank may give himself a battle bonus. User may config it via patch [Weapon rank battle bonus](../Patches/PATCH_WrankBonus.txt).

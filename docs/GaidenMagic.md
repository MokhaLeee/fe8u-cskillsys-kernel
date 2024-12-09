The GaidenMagic system in c-skillsys is a remake of `B.Mag` and `W.Mag` in *Fire Emblem: Gaiden*. The related code is mainly stored in:

- [Misc/GaidenMagic](../Wizardry/Misc/GaidenMagic)
- [BattleSys/HpCost](../Wizardry/Core/BattleSys/HpCost)
- [BattleWeapon.c](../Wizardry/Core/BattleSys/Source/BattleWeapon.c).

Gaiden magic config lists have been actively adapted for FEB. Now users can directly use existing FEB form and patch to set unit gaiden magic list and item hp cost.

# Configs

We have put several config options to increase the freedom for developers. You can change the functionality via the following configs in [configs.h](../include/Configs/configs.h):

- `USE_GAIDEN_MAGIC`: enables the gaiden magic system.
- `MUST_BE_MAGIC`: allows none-magic weapons exist in B.Mag list.
- `REQUIRES_WRANK`: determines whether the character can use magic that exceeds his weapon rank.
- `AI_EN`: allows AI to use B/W magic.
- `EXT_CONF_EN`: enables external magic list config table: `gGaidenChaxConfigs`

To improve the user experience for non-wizardry developers, the configs above can be directly modified by `gKernelDesigerConfig` via FEB patch free from custom build.

# Tables

C-SkillSys supports the same table as old skillsys, which can be directly setup by FEB:

- `gGaidenPinfoConfigList`: give unit magic on considering their level, can be configured by FEB form **Gaiden-style Spell**.
- `gGaidenWeaponCostList`: set hp cost for item, can be directly configurable via existing FEB patch: **GaidenSpellCostTable**.

On this basis, we have added a new dynamic table to increase the freedom of configuration:

- `gGaidenChaxConfigs`: equip magic by:
	- character index
	- class index
	- unit level
	- unit faction
	- skill
	- event flag

# Limitations
- `B.Mag` can only support weapons.
- `B.Mag` supoorts non-magic weapons under the configuration of `CONFIG_GAIDEN_MAGIC_MUST_BE_MAGIC`.
- `W.Mag` can only supoort staff items.

> To following sections are mainly for wizardry.

# Weapon slot

The whole gaiden magic system in c-skillsys is built based on battle weapon-slot expansion. Based on vanilla function `SetBattleUnitWeapon()`, weapon slot 9-15 are allocated to gaiden B.Mag, and 16-22 to W.Mag. And kernel may find the related magic by function `GetGaidenMagicItem()`. For more details, see *Weapon slot expansion* section in [BattleSystem.md](./BattleSystem.md).

# Gaiden magic action

Both gaiden B.Mag and W.Mag hold their unique action seperated from existing combat/staff action to better handle weapon selection and hp cost issues:

```c
// configs.h
#define CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat 0x24
#define CONFIG_UNIT_ACTION_EXPA_GaidenMagicStaff 0x25
```

In order to reduce the modification of item effect functions, we directly insert a action transform in action routine, `TryChangeGaidenMagicAction()`, in which we may judge on `gActionData.itemSlotIndex`, and transform to gaiden magic action for gaiden magic weapon slots.

The item use effect (`DoItemUse()`) and action (`ActionStaffDoorChestUseItem()`) routine is just introduced the minimal modification: We just replace the function call on [item-getter in item-use](https://github.com/MokhaLeee/fireemblem8u/blob/master/src/bmitemuse.c#L296) to `AutoGetItemUseActionItem()`.

# HP cost

C-SkillSys hold a seperate HP-cost system in battle-sys. For more details, see *Hp cost* section in [BattleSystem.md](./BattleSystem.md).

# Kown issues

You need to take note that the unrestrained expansion of `gGaidenChaxConfigs` may cause potential performance issue. The weak GBA CPU may suffer a lot of time to calculate the magic list for each unit especially when the AI ​​calculates each enemies' combat radius to obtain dangerous area. Related call stack:

```c
// 1
AiAttemptOffensiveAction -->
AiFillReversedAttackRangeMap -->
GetUnitMaxRange -->
GetGaidenMagicList -->
UpdateGaidenMagicList

// 2
CpPerform_EquipBest -->
GetItemMaxRange -->
GetGaidenMagicList -->
UpdateGaidenMagicList
```

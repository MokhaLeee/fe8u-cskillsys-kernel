Overview of the call stack on vanilla battle-system:
```
ActionCombat
├── InitObstacleBattleUnit
├── BattleGenerateReal
│   └── BattleGenerateRealInternal
│       ├── InitBattleUnit: Unit2BattleHook
│       ├── SetBattleUnitWeapon
│       ├── BattleInitTargetCanCounter: CheckCanCounter
│       ├── BattleApplyWeaponTriangleEffect: WeaponTriangle
│       ├── BattleGenerate
│       │   ├── ComputeBattleUnitStats: PreBattleCalc
│       │   │   ├── ...
│       │   │   ├── ComputeBattleUnitAttack
│       │   │   │   ├── IsUnitEffectiveAgainst
│       │   │   │   └── IsItemEffectiveAgainst
│       │   │   └── ...
│       │   ├── ComputeBattleUnitEffectiveStats: BattlcCalcReal
│       │   │   ├── ComputeBattleUnitEffectiveHitRate
│       │   │   ├── ComputeBattleUnitEffectiveCritRate
│       │   │   ├── ComputeBattleUnitSilencerRate
│       │   │   └── ComputeBattleUnitSpecialWeaponStats
│       │   └── BattleUnwind
│       │       ├── BattleGetFollowUpOrder
│       │       └── BattleGenerateRoundHits
│       │           ├── GetBattleUnitHitCount
│       │           └── BattleGenerateHit
│       │               ├── BattleUpdateBattleStats
│       │               ├── BattleGenerateHitTriangleAttack: removed and put into PreBattleCalc
│       │               ├── BattleGenerateHitAttributes
│       │               │   ├── BattleCheckSureShot
│       │               │   ├── BattleCheckGreatShield
│       │               │   ├── BattleCheckPierce
│       │               │   ├── BattleCheckSilencer
│       │               │   └── BattleCheckPetrify
│       │               └── BattleGenerateHitEffects
│       ├── BattleUnitTargetCheckCanCounter
│       ├── BattleUnitTargetSetEquippedWeapon: mainly setup for banim UI, if unit has no weapon, give a weapon to show.
│       └── BattleApplyExpGains
│           ├── GetBattleUnitExpGain
│           └── CheckBattleUnitLevelUp
└── sProcScr_CombatAction
    ├── BeginBattleAnimations
    ├── BattleApplyGameStateUpdates
    │   └── BattleApplyUnitUpdates
    │       └── UpdateUnitFromBattle
    └── BATTLE_HandleCombatDeaths
```

# Damage calc formula

```
result = ([atk + correction - def]) * (100% + increase%) * (100%  + crit_correction%) / (100% + decrease%) + real_damage
```

# Combo-attack

When attacking the enemy within the range of the companion, ally will participate in the combo attack.

# Battle surround

When a unit is attacked and adjacent to the enemy, each side with enemy may cause unit avoid -10%. If completely surrounded (each side stands an enemy), unit may cause def-5 additionally.

Flyer in outdoor environments are not affected by this effect

# Hp drain

In vanilla, it directly uses `BattleHit::hpChange` to determine hp drain amount for each round.

In C-SkillSys, in order to modify hp drain, the amound is stored in `gExtBattleHitArray`, which is recorded round by round. If there hold hp-drain in one round, then flag `BATTLE_HIT_ATTR_HPSTEAL` should also be set in `BattleHit::attributes`, which is just consistent to vanilla.

In C-SkillSys, Hp drain is mainly calculated by function `BattleHit_CalcHpDrain`, which will be called after damage calculation. Meanwhile, an external API `AppendHpDrain` can also register the hp drain amount.

The related round anim effect functions are also directly rewriten:

- banim: `ParseBattleHitToBanimCmd`
- mapanim: `MapAnim_BeginRoundSpecificAnims`

# Hp cost



# Battle unit slot expansion

Taking gaiden/TH/engage style weapon system into consideration, the battle weapon slot is expanded:

- 0-4: unit items
- 5: special item (dance ring)
- 6-7: arena
- 8: ballista
- 9-15: (CHAX) gaiden B.Magic
- 16-22: (CHAX) gaiden W.Magic
- 23-35: (CHAX, preserved) ThreeHouses style magic
- 36-42: (CHAX, preserved) Engage style weapon

It should be noted that as for now the weapon slot index may cause overflow on `Unit::items`, it is **DANGROUS** to directly find the item by `unit->items[slot]`, as an alternative, kernel has offered the specific API to get weapon from slot:

```c
#include <battle-system.h>

int GetItemFromSlot(struct Unit *unit, int slot);
```

# Battle weapon auto-equipment

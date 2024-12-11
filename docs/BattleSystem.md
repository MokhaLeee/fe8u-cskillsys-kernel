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

# Hp drain

In vanilla, it directly uses `BattleHit::hpChange` to determine hp drain amount for each round.

In C-SkillSys, in order to modify hp drain, the amound is stored in `gExtBattleHitArray`, which is recorded round by round. If there hold hp-drain in one round, then flag `BATTLE_HIT_ATTR_HPSTEAL` should also be set in `BattleHit::attributes`, which is just consistent to vanilla.

In C-SkillSys, Hp drain is mainly calculated by function `BattleHit_CalcHpDrain`, which will be called after damage calculation. Meanwhile, an external API `AppendHpDrain` can also register the hp drain amount.

The related round anim effect functions are also directly rewriten:

- banim: `ParseBattleHitToBanimCmd`
- mapanim: `MapAnim_BeginRoundSpecificAnims`

# Hp cost

The **HP cost** is a new combat mechanic introduced in the c-Skillsys, inspired by the *Gaiden magic system*. The related code is mainly stored in [BattleSys/HpCost](../Wizardry/Core/BattleSys/HpCost) and [BattleSys/RoundAnim](../Wizardry/Core/BattleSys/RoundAnim) directory.

It is important to note the HP cost is designed **COMPLETE DIFFERENT** from regular battle damage. It is initiated proactively by the attacker before each battle round. The HP cost is non-lethal and can only be triggered when the user's HP is above a predefined threshold. The kernel stores this value in `ExtBattleHit::hp_cost` to preserve the value for each round. In principle, developers are not encouraged to directly modify this value in ext-hit. Instead, the kernel provides dedicated APIs for developers to register the HP cost:

```c
bool TryBattleHpCost(struct BattleUnit *bu, int hp_cost);
bool AddBattleHpCost(struct BattleUnit *bu, int round, int cost);
```

The anim effect on hp cost is also seperated designed:

- MapAnim

The hp-cost process is registered to pre-round hook `gPreMapAnimBattleRoundHooks`, which will insert the anim of the attacer's HP reduction after skill icon displaying and before round based attack anim starting.

- Banim

The hp-cost will allocate separate slot in `EfxHpLut` for each round if an HP cost exists via `ParseBattleHitToBanimCmd`. Then display the hp-cost anim and advance the hp-lut offset `gEfxHpLutOff` when anim command `C07` is executed.

# Weapon slot expansion

Taking gaiden/TH/engage style weapon system into consideration, the battle weapon slot is expanded:

- 0-4: unit items
- 5: special item (dance ring)
- 6-7: arena
- 8: ballista
- 9-15: (CHAX) gaiden B.Magic
- 16-22: (CHAX) gaiden W.Magic
- 23-36: (CHAX, preserved) ThreeHouses style magic
- 37-42: (CHAX, preserved) Engage style weapon

It should be noted that as for now the weapon slot index may cause overflow on `Unit::items`, it is **DANGROUS** to directly find the item by `unit->items[slot]`, as an alternative, kernel has offered the specific API to get weapon from slot:

```c
#include <battle-system.h>

int GetItemFromSlot(struct Unit *unit, int slot);
```

# Battle weapon auto-equipment

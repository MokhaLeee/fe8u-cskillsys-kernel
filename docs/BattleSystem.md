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

In addition to traditional base stats such as **attack**, **defense**, **avoid**, and **critical hits**, CSkillsys introduces multiple dimensions for stacking combat power and encourages players to enhance their characters across these dimensions simultaneously.

- **Damage Amplification**

    The base damage amplification value is 100%, which can be increased through skills. However, different damage amplification effects stack additively, leading to diminishing returns.

    For example:

    - The skill **UnstoppableForce** provides `+100%` damage.
    - The skill **SolarPower** provides `+25%` damage.

    Then total damage amplification: `(100 + 100 + 25) = 225%`. If the base damage is **20**, the actual damage dealt after amplification will be **45**.

- **Critical Hit Amplification**

    Critical hit amplification is an independent multiplier from damage amplification, applying multiplicatively. By default, critical hits deal 200% damage (compared to the original 300%, adjustable by designers).

    For example: With a total damage amplification of `225%`, a critical hit will deal `225% × 2 = 450%` damage.

- **Damage Reduction**

    Since damage reduction effects also stack additively (leading to diminishing returns), the calculation is more complex. CSkillsys calculates damage reduction as follows:

    Each damage reduction effect is assigned a reference value (`ref`). And the actual damage taken is determined by:

        result = [0x100 / (0x100 + ref)]

    For example, To achieve 50% damage reduction, the reference value is:

        ref = (0x100 / 0.5 - 0x100) = 256

    - For 25% reduction, `ref = 85`
    - For 75% reduction, `ref = 768`
    - ...

    When multiple damage reduction effects stack, their reference values are summed before calculation.

    Example:

    **50% + 25%** reduction → `ref_total = 256 + 85 = 341`

    Final damage taken: `[0x100 / (0x100 + 341)] = 43%` (i.e., **57%** damage reduction)

- Real Damage

    Real damage is an additional damage type that:

    - Ignores enemy defense and damage reduction.
    - Cannot be amplified by damage or critical multipliers.

    This dimension plays as an entirely independent damage source. It can be triggered by skills like `RuinedBlade`, making it ideal for low-attack, high-speed characters to pursue as a combat dimension.

Most damage amplification and reduction skills take effect during battle by rolling RN (Random Number) (e.g., the skill **DragonFang**).

We consolidated these processes along with the critical attack roll into a unified function, `BattleHit_CalcDamage`, which is ultimately called by the vanilla function `BattleGenerateHitAttributes` to apply the effects.

Additionally, to provide players with more accurate battle-forecast information, we also recommond to put non-RN-dependent damage amplification effects to pre-battle calc process (e.g., the skill **MeleeManiac**).These results are then reflected in the battle-forecast via the function `ModifyBattleStatusForUI`.

For Developers, If your damage amplification depends on an RN roll, it should be implemented within `BattleHit_CalcDamage`, otherwise it is rather recommend to apply it during pre-battle calculations (via `ComputeBattleUnitStats`) for optimization.

# Hp drain

In vanilla, it directly uses `BattleHit::hpChange` to determine hp drain amount for each round.

In C-SkillSys, in order to modify hp drain, the amound is stored in `gExtBattleHitArray`, which is recorded round by round. If there hold hp-drain in one round, then flag `BATTLE_HIT_ATTR_HPSTEAL` should also be set in `BattleHit::attributes`, which is just consistent to vanilla.

In C-SkillSys, Hp drain is mainly calculated by function `BattleHit_CalcHpDrain`, which will be called after damage calculation. Meanwhile, an external API `AppendHpDrain` can also register the hp drain amount.

The related round anim effect functions are also directly rewriten:

- banim: `ParseBattleHitToBanimCmd`
- mapanim: `MapAnim_BeginRoundSpecificAnims`

# Hp cost

The **HP cost** is a new combat mechanic introduced in the c-Skillsys, inspired by the *Gaiden magic system*. The related code is mainly stored in [BattleSys/HpCost](../Kernel/Wizardry/Core/BattleSys/HpCost/) and [BattleSys/RoundAnim](../Kernel/Wizardry/Core/BattleSys/RoundAnim/) directory.

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

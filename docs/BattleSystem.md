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

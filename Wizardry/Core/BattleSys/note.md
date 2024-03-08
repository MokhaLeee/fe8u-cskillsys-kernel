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

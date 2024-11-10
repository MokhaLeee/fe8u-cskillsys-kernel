#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "item-sys.h"

const KeyValuePair dict[] = { { "Absolve", 1000 },
                              { "Accost", 1000 },
                              { "Acrobat", 1000 },
                              { "Adaptable", 1000 },
                              { "AdaptiveLunge", 1000 },
                              { "AdaptiveStance", 1000 },
                              { "Adept", 1000 },
                              { "Admiration", 1000 },
                              { "AdrenalineRush", 1000 },
                              { "AdvantageChaser", 1000 },
                              { "Aegis", 1000 },
                              { "Aerobatics", 1000 },
                              { "Aether", 1000 },
                              { "Airlock", 1000 },
                              { "AirRaidAttack", 1000 },
                              { "AirRaidAvoid", 1000 },
                              { "AirRaidCrit", 1000 },
                              { "AirRaidDefense", 1000 },
                              { "AirRaidHit", 1000 },
                              { "AirRaidResistance", 1000 },
                              { "AirRaidSpeed", 1000 },
                              { "AlertStance", 1000 },
                              { "AlertStancePlus", 1000 },
                              { "Amaterasu", 1000 },
                              { "Amische", 1000 },
                              { "Analytic", 1000 },
                              { "Anathema", 1000 },
                              { "Anchor", 1000 },
                              { "AngerPoint", 1000 },
                              { "Aptitude", 1000 },
                              { "ArcaneBlade", 1000 },
                              { "Arcgift", 1000 },
                              { "ArcgiftPlus", 1000 },
                              { "ArdentSacrifice", 1000 },
                              { "ArenaTrap", 1000 },
                              { "Armorboost", 1000 },
                              { "ArmorMarch", 1000 },
                              { "Armsthrift", 1000 },
                              { "Assist", 1000 },
                              { "Astra", 1000 },
                              { "AversaNight", 1000 },
                              { "Avoid", 1000 },
                              { "AvoidSword", 1000 },
                              { "AxeBreaker", 1000 },
                              { "AxeFaith", 1000 },
                              { "Bane", 1000 },
                              { "Bargain", 1000 },
                              { "Barricade", 1000 },
                              { "BarricadePlus", 1000 },
                              { "BattleRange_Todo1", 1000 },
                              { "BattleRange_Todo2", 1000 },
                              { "BattleRange_Todo3", 1000 },
                              { "BattleVeteran", 1000 },
                              { "BeastAssault", 1000 },
                              { "BidingBlow", 1000 },
                              { "BlackMagic", 1000 },
                              { "Blacksmith", 1000 },
                              { "Bladegift", 1000 },
                              { "BladegiftPlus", 1000 },
                              { "Blessing", 1000 },
                              { "BloodTide", 1000 },
                              { "Blossom", 1000 },
                              { "BlowArmored", 1000 },
                              { "BlowDarting", 1000 },
                              { "BlowDeath", 1000 },
                              { "BlowDuelist", 1000 },
                              { "BlowFiendish", 1000 },
                              { "BlowKilling", 1000 },
                              { "BlowUncanny", 1000 },
                              { "BlowWarding", 1000 },
                              { "BlueFlame", 1000 },
                              { "BlueLionRule", 1000 },
                              { "BMagBreaker", 1000 },
                              { "BoldFighter", 1000 },
                              { "Bond", 1000 },
                              { "Boon", 1000 },
                              { "BoundlessVitality", 1000 },
                              { "BowBreaker", 1000 },
                              { "BreathOfLife", 1000 },
                              { "Bushido", 1000 },
                              { "Camaraderie", 1000 },
                              { "Canto", 1000 },
                              { "CantoPlus", 1000 },
                              { "Capture", 1000 },
                              { "Casual", 1000 },
                              { "CatchEmAll", 1000 },
                              { "CatchingUp", 1000 },
                              { "ChaosStyle", 1000 },
                              { "Charge", 1000 },
                              { "ChargePlus", 1000 },
                              { "Charisma", 1000 },
                              { "Charm", 1000 },
                              { "Chivalry", 1000 },
                              { "Chlorophyll", 1000 },
                              { "Clearance", 1000 },
                              { "ClearBody", 1000 },
                              { "CloseCombat", 1000 },
                              { "Colossus", 1000 },
                              { "Comatose", 1000 },
                              { "ConcealedWeapon", 1000 },
                              { "Contrary", 1000 },
                              { "Corona", 1000 },
                              { "Corrosion", 1000 },
                              { "Counter", 1000 },
                              { "CounterMagic", 1000 },
                              { "CounterRoar", 1000 },
                              { "Crit", 1000 },
                              { "CritAxe", 1000 },
                              { "CritBow", 1000 },
                              { "CriticalForce", 1000 },
                              { "CriticalOverload", 1000 },
                              { "CriticalPierce", 1000 },
                              { "CritLance", 1000 },
                              { "CritSword", 1000 },
                              { "CrusaderWard", 1000 },
                              { "Cultured", 1000 },
                              { "Dance", 1000 },
                              { "DancingBlade", 1000 },
                              { "DarkBargain", 1000 },
                              { "DarkHorse", 1000 },
                              { "Daunt", 1000 },
                              { "Dazzle", 1000 },
                              { "Deadeye", 1000 },
                              { "Deal", 1000 },
                              { "Debilitator", 1000 },
                              { "DefBonus", 1000 },
                              { "DefenseBoost", 1000 },
                              { "DefHone", 1000 },
                              { "DefiantAvoid", 1000 },
                              { "DefiantCrit", 1000 },
                              { "DefiantDef", 1000 },
                              { "DefiantLck", 1000 },
                              { "DefiantMag", 1000 },
                              { "DefiantRes", 1000 },
                              { "DefiantSkl", 1000 },
                              { "DefiantSpd", 1000 },
                              { "DefiantStr", 1000 },
                              { "Demoiselle", 1000 },
                              { "DemolitionExpert", 1000 },
                              { "Desperation", 1000 },
                              { "Despoil", 1000 },
                              { "DestinyBond", 1000 },
                              { "DetailedReport", 1000 },
                              { "DevilsLuck", 1000 },
                              { "DevilsPact", 1000 },
                              { "DevilsWhim", 1000 },
                              { "Discipline", 1000 },
                              { "Dismount", 1000 },
                              { "DivinelyInspiring", 1000 },
                              { "DivinePulse", 1000 },
                              { "Domain", 1000 },
                              { "DominantArm", 1000 },
                              { "Doppleganger", 1000 },
                              { "DoubleLion", 1000 },
                              { "DoubleUp", 1000 },
                              { "DownWithArch", 1000 },
                              { "DragonFang", 1000 },
                              { "DragonSkin", 1000 },
                              { "DragonWall", 1000 },
                              { "DragonWarth", 1000 },
                              { "DrawBack", 1000 },
                              { "DriveDef", 1000 },
                              { "DriveMag", 1000 },
                              { "DriveRes", 1000 },
                              { "DriveSpd", 1000 },
                              { "DriveStr", 1000 },
                              { "Drizzle", 1000 },
                              { "Drought", 1000 },
                              { "EarthBoost", 1000 },
                              { "Eclipse", 1000 },
                              { "EffectSpore", 1000 },
                              { "ElbowRoom", 1000 },
                              { "Enrage", 1000 },
                              { "Entrepreneur", 1000 },
                              { "EvenFooted", 1000 },
                              { "EvenRhythm", 1000 },
                              { "Expertise", 1000 },
                              { "ExpertsExperience", 1000},
                              { "FaerghusAncestry", 1000 },
                              { "FailGale", 1000 },
                              { "FaireAnima", 1000 },
                              { "FaireAxe", 1000 },
                              { "FaireBMag", 1000 },
                              { "FaireBow", 1000 },
                              { "FaireDark", 1000 },
                              { "FaireLance", 1000 },
                              { "FaireLight", 1000 },
                              { "FaireSword", 1000 },
                              { "FairyTaleFolk", 1000 },
                              { "FieryBlood", 1000 },
                              { "FireBoost", 1000 },
                              { "Flank", 1000 },
                              { "Flare", 1000 },
                              { "FlashingBlade", 1000 },
                              { "FlashingBladePlus", 1000 },
                              { "FlierFormation", 1000 },
                              { "FlierGuidance", 1000 },
                              { "Fluffy", 1000 },
                              { "Focus", 1000 },
                              { "Forager", 1000 },
                              { "ForeignPrincess", 1000 },
                              { "Foresight", 1000 },
                              { "FortifyArmor", 1000 },
                              { "FortifyingStaff", 1000},
                              { "FortressDef", 1000 },
                              { "FortressRes", 1000 },
                              { "Fortune", 1000 },
                              { "FranticSwing", 1000 },
                              { "Frenzy", 1000 },
                              { "FullMetalBody", 1000 },
                              { "Fury", 1000 },
                              { "FuryPlus", 1000 },
                              { "Galeforce", 1000 },
                              { "GaleWings", 1000 },
                              { "Gambit", 1000 },
                              { "Gentilhomme", 1000 },
                              { "GentleFlower", 1000 },
                              { "Glacies", 1000 },
                              { "GoddessDance", 1000 },
                              { "GoldDigger", 1000 },
                              { "GoodAsGold", 1000 },
                              { "GoodyBasket", 1000 }, 
                              { "Gracegift", 1000 },
                              { "GracegiftPlus", 1000 },
                              { "GreatShield", 1000 },
                              { "GridMaster", 1000 },
                              { "GrislyWound", 1000 },
                              { "GuardBearing", 1000 },
                              { "Guidance", 1000 },
                              { "Guts", 1000 },
                              { "Hackgift", 1000 },
                              { "HackgiftPlus", 1000 },
                              { "Hawkeye", 1000 },
                              { "HazeHunter", 1000 },
                              { "HealingFocus", 1000 },
                              { "HeavyBlade", 1000 },
                              { "HeavyBladePlus", 1000 },
                              { "HeavyMetal", 1000 },
                              { "HeavyStrikes", 1000 },
                              { "Hero", 1000 },
                              { "Hex", 1000 },
                              { "HighGround", 1000 },
                              { "HoardersBane", 1000 },
                              { "HolyAura", 1000 },
                              { "HoneArmor", 1000 },
                              { "HoneCavalry", 1000 },
                              { "HoneChaos", 1000 },
                              { "HoneFlier", 1000 },
                              { "Horseguard", 1000 },
                              { "HpBonus", 1000 },
                              { "HyperCutter", 1000 },
                              { "IdentityProblems", 1000 },
                              { "Ignis", 1000 },
                              { "Imbue", 1000 },
                              { "Impale", 1000 },
                              { "IndoorMarch", 1000 },
                              { "Infiltrator", 1000 },
                              { "InfinityEdge", 1000 },
                              { "InitCalm", 1000 },
                              { "InitClever", 1000 },
                              { "InitDeft", 1000 },
                              { "InitLucky", 1000 },
                              { "InitNimble", 1000 },
                              { "InitQuick", 1000 },
                              { "InitSpectrum", 1000 },
                              { "InitStrong", 1000 },
                              { "InitSturdy", 1000 },
                              { "Inori", 1000 },
                              { "Insomnia", 1000 },
                              { "Inspiration", 1000 },
                              { "Intimidate", 1000 },
                              { "KeenFighter", 1000 },
                              { "KeepUp", 1000 },
                              { "KillingMachine", 1000 },
                              { "KnightAspirant", 1000 },
                              { "LadyBlade", 1000 },
                              { "LanceBreaker", 1000 },
                              { "LastWord", 1000 },
                              { "LckBonus", 1000 },
                              { "LckHone", 1000 },
                              { "LeadByExample", 1000 },
                              { "LEGEND_4", 1000 },
                              { "LEGEND_5", 1000 },
                              { "LEGEND_InoriAtk", 1000 },
                              { "LEGEND_InoriAvo", 1000 },
                              { "LEGEND_InoriDef", 1000 },
                              { "Lethality", 1000 },
                              { "LifeAndDeath", 1000 },
                              { "Lifetaker", 1000 },
                              { "LightAndDark", 1000 },
                              { "LightRune", 1000 },
                              { "LightWeight", 1000 },
                              { "LilysPoise", 1000 },
                              { "LimitBreaker", 1000 },
                              { "LimitBreakerPlus", 1000 },
                              { "LiquidOoze", 1000 },
                              { "LiveToServe", 1000 },
                              { "LockTouch", 1000 },
                              { "Loyalty", 1000 },
                              { "LuckBoost", 1000 },
                              { "LuckySeven", 1000 },
                              { "Ludopathy", 1000 },
                              { "LullDefense", 1000 },
                              { "LullLuck", 1000 },
                              { "LullMagic", 1000 },
                              { "LullMovement", 1000 },
                              { "LullResistance", 1000 },
                              { "LullSkill", 1000 },
                              { "LullSpectrum", 1000 },
                              { "LullSpeed", 1000 },
                              { "LullStrength", 1000 },
                              { "Lumina", 1000 },
                              { "LuminaPlus", 1000 },
                              { "Luna", 1000 },
                              { "LunaAttack", 1000 },
                              { "LunarBrace", 1000 },
                              { "Lunge", 1000 },
                              { "MagBonus", 1000 },
                              { "MageSlayer", 1000 },
                              { "MagHone", 1000 },
                              { "MagicBoost", 1000 },
                              { "MagicBounce", 1000 },
                              { "MagicEye", 1000 },
                              { "MagicGambit", 1000 },
                              { "MagicSeal", 1000 },
                              { "MaleficAura", 1000 },
                              { "Mantle", 1000 },
                              { "MeleeManiac", 1000 },
                              { "Mentorship", 1000 },
                              { "Merciless", 1000 },
                              { "Mercurious", 1000 },
                              { "Mercy", 1000 },
                              { "Mimic", 1000 },
                              { "Mine", 1000 },
                              { "Minus", 1000 },
                              { "Moody", 1000 },
                              { "MoonBow", 1000 },
                              { "Moonlight", 1000 },
                              { "MountainClimber", 1000 },
                              { "MountainClimberPlus", 1000 },
                              { "MovBonus", 1000 },
                              { "MoveBoost", 1000 },
                              { "MovHone", 1000 },
                              { "Multiscale", 1000 },
                              { "MysticBoost", 1000 },
                              { "NaturalCover", 1000 },
                              { "NatureRush", 1000 },
                              { "NiceThighs", 1000 },
                              { "NightTide", 1000 },
                              { "Nihil", 1000 },
                              { "NoGuard", 1000 },
                              { "Nonconforming", 1000 },
                              { "Nullify", 1000 },
                              { "OgreBody", 1000},
                              { "OathDefense", 1000 },
                              { "OathLuck", 1000 },
                              { "OathMagic", 1000 },
                              { "OathMovement", 1000 },
                              { "OathResistance", 1000 },
                              { "OathSkill", 1000 },
                              { "OathSpeed", 1000 },
                              { "OathStrength", 1000 },
                              { "Obstruct", 1000 },
                              { "OddFooted", 1000 },
                              { "OddRhythm", 1000 },
                              { "Onimaru", 1000 },
                              { "Opportunist", 1000 },
                              { "OutdoorFighter", 1000 },
                              { "Outrider", 1000 },
                              { "PairUp", 1000 },
                              { "Paragon", 1000 },
                              { "Pass", 1000 },
                              { "PassionsFlow", 1000 },
                              { "PastelVeil", 1000 },
                              { "Pathfinder", 1000 },
                              { "Patience", 1000 },
                              { "Pavise", 1000 },
                              { "Peacebringer", 1000 },
                              { "Perfectionist", 1000 },
                              { "Petrify", 1000 },
                              { "Pickup", 1000 },
                              { "Piercegift", 1000 },
                              { "PiercegiftPlus", 1000 },
                              { "Pivot", 1000 },
                              { "Plus", 1000 },
                              { "PointBlank", 1000 },
                              { "Poise", 1000 },
                              { "PoisonHeal", 1000 },
                              { "PoisonPoint", 1000 },
                              { "PoisonStrike", 1000 },
                              { "PosReturn", 1000 },
                              { "PowerStaff", 1000 },
                              { "PowHone", 1000 },
                              { "Pragmatic", 1000 },
                              { "Prescience", 1000 },
                              { "PridefulWarrior", 1000 },
                              { "Provoke", 1000 },
                              { "PsychUp", 1000 },
                              { "Puissance", 1000 },
                              { "Pursuer", 1000 },
                              { "Pursuit", 1000 },
                              { "PushDefense", 1000 },
                              { "PushLuck", 1000 },
                              { "PushMagic", 1000 },
                              { "PushMovement", 1000 },
                              { "PushResistance", 1000 },
                              { "PushSkill", 1000 },
                              { "PushSpectrum", 1000 },
                              { "PushSpeed", 1000 },
                              { "PushStrength", 1000 },
                              { "QuickBurn", 1000 },
                              { "QuickDraw", 1000 },
                              { "QuickHands", 1000 },           
                              { "QuickLearner", 1000 },
                              { "QuickRiposte", 1000 },
                              { "RainDish", 1000 },
                              { "RallyDefense", 1000 },
                              { "RallyLuck", 1000 },
                              { "RallyMagic", 1000 },
                              { "RallyMovement", 1000 },
                              { "RallyResistance", 1000 },
                              { "RallySkill", 1000 },
                              { "RallySpectrum", 1000 },
                              { "RallySpeed", 1000 },
                              { "RallyStrength", 1000 },
                              { "Rampage", 1000 },
                              { "RampUp", 1000 },
                              { "RangeBonusBMag1", 1000 },
                              { "RangeBonusBMag2", 1000 },
                              { "RangeBonusBow1", 1000 },
                              { "RangeBonusBow2", 1000 },
                              { "ReciprocalAid", 1000 },
                              { "RecklessFighter", 1000 },
                              { "Reflex", 1000 },
                              { "Relief", 1000 },
                              { "Renewal", 1000 },
                              { "Reposition", 1000 },
                              { "ResBonus", 1000 },
                              { "ResHone", 1000 },
                              { "ResistanceBoost", 1000 },
                              { "Resolve", 1000 },
                              { "Resourceful", 1000 },
                              { "RightfulArch", 1000 },
                              { "RightfulGod", 1000 },
                              { "RightfulKing", 1000 },
                              { "RiskItAll", 1000 },
                              { "Roll12", 1000 },
                              { "RouseDefense", 1000 },
                              { "RouseLuck", 1000 },
                              { "RouseMagic", 1000 },
                              { "RouseMovement", 1000 },
                              { "RouseResistance", 1000 },
                              { "RouseSkill", 1000 },
                              { "RouseSpeed", 1000 },
                              { "RouseStrength", 1000 },
                              { "RuinedBlade", 1000 },
                              { "RuinedBladePlus", 1000 },
                              { "RunningStart", 1000 },
                              { "Sacrifice", 1000 },
                              { "SandRush", 1000 },
                              { "SandStream", 1000 },
                              { "SavageBlow", 1000 },
                              { "SealDefense", 1000 },
                              { "SealLuck", 1000 },
                              { "SealMagic", 1000 },
                              { "SealResistance", 1000 },
                              { "SealSkill", 1000 },
                              { "SealSpeed", 1000 },
                              { "SealStrength", 1000 },
                              { "Secret", 1000 },
                              { "Shade", 1000 },
                              { "ShadePlus", 1000 },
                              { "Shadowgift", 1000 },
                              { "ShadowgiftPlus", 1000 },
                              { "ShortShield", 1000 },
                              { "Shove", 1000 },
                              { "ShrewdPotential", 1000 },
                              { "SilentPride", 1000 },
                              { "SkillBoost", 1000 },
                              { "SklBonus", 1000 },
                              { "SklHone", 1000 },
                              { "Skybreaker", 1000 },
                              { "Skyguard", 1000 },
                              { "Slayer", 1000 },
                              { "SlowBurn", 1000 },
                              { "SlushRush", 1000 },
                              { "Smite", 1000 },
                              { "SnowWarning", 1000 },
                              { "SoaringGuidance", 1000 },
                              { "SoaringWings", 1000 },
                              { "SocialButterfly", 1000 },
                              { "Sol", 1000 },
                              { "SolarPower", 1000 },
                              { "Solidarity", 1000 },
                              { "SolidRock", 1000 },
                              { "SorceryBlade", 1000 },
                              { "SoulSap", 1000 },
                              { "SpdBonus", 1000 },
                              { "SpdHone", 1000 },
                              { "SpectrumBoost", 1000 },
                              { "SpeedBoost", 1000 },
                              { "SpurDef", 1000 },
                              { "SpurMag", 1000 },
                              { "Spurn", 1000 },
                              { "SpurRes", 1000 },
                              { "SpurSpd", 1000 },
                              { "SpurStr", 1000 },
                              { "StaffGuard", 1000},
                              { "StaffParagon", 1000 },
                              { "StaffSavant", 1000 },
                              { "StanceBracing", 1000 },
                              { "StanceDarting", 1000 },
                              { "StanceFierce", 1000 },
                              { "StanceKestrel", 1000 },
                              { "StanceMirror", 1000 },
                              { "StanceReady", 1000 },
                              { "StanceSteady", 1000 },
                              { "StanceSturdy", 1000 },
                              { "StanceSwift", 1000 },
                              { "StanceWarding", 1000 },
                              { "SteadyBrawler", 1000 },
                              { "Steal", 1000 },
                              { "StealPlus", 1000 },
                              { "StoneBody", 1000 },
                              { "Stormgift", 1000 },
                              { "StormgiftPlus", 1000 },
                              { "StrBonus", 1000 },
                              { "StrengthBoost", 1000 },
                              { "Stride", 1000 },
                              { "StrongCon", 1000 },
                              { "StrongRiposte", 1000 },
                              { "StunningSmile", 1000 },
                              { "Sturdy", 1000 },
                              { "Summon", 1000 },
                              { "SuperLuck", 1000 },
                              { "Supply", 1000 },
                              { "SureShot", 1000 },
                              { "Survivor", 1000 },
                              { "Swap", 1000 },
                              { "Swarm", 1000 },
                              { "Swarp", 1000 },
                              { "SwiftSwim", 1000 },
                              { "SwordBreaker", 1000 },
                              { "Synchronize", 1000 },
                              { "Synergism", 1000 },
                              { "Tantivy", 1000 },
                              { "Technician", 1000 },
                              { "Teleportation", 1000 },
                              { "Thighdeology", 1000 },
                              { "ThotSlayer", 1000 },
                              { "Thunderstorm", 1000 },
                              { "TowerShield", 1000 },
                              { "TowerShieldPlus", 1000 },
                              { "Trample", 1000 },
                              { "Transform", 1000 },
                              { "TriangleAdept", 1000 },
                              { "TriangleAdeptPlus", 1000 },
                              { "TriangleAttack", 1000 },
                              { "TripleUp", 1000 },
                              { "TripUp", 1000 },
                              { "Tsuzuku", 1000 },
                              { "Turncoat", 1000 },
                              { "Unaware", 1000 },
                              { "UpWithArch", 1000 },
                              { "Vanity", 1000 },
                              { "Vantage", 1000 },
                              { "Velocity", 1000 },
                              { "Vengeance", 1000 },
                              { "VengefulFighter", 1000 },
                              { "Vigilance", 1000 },
                              { "VigorDance", 1000 },
                              { "VoiceOfPeace", 1000 },
                              { "VoidCurse", 1000 },
                              { "WaryFighter", 1000 },
                              { "WatchfulEye", 1000 },
                              { "WaterBoost", 1000 },
                              { "WaterWalking", 1000 },
                              { "WaterWalkingPlus", 1000 },
                              { "WhitePool", 1000 },
                              { "WindBoost", 1000 },
                              { "WindDisciple", 1000 },
                              { "WonderGuard", 1000 },
                              { "Wrath", 1000 },
                              { "YuneWhispers", 1000 } };

const int dict_size = sizeof(dict) / sizeof(dict[0]);

// Custom binary search function
int binary_search(const KeyValuePair * array, int size, const char * key)
{
    int low = 0;
    int high = size - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(array[mid].key, key);

        if (cmp == 0)
        {
            return array[mid].value; // Key found
        }
        else if (cmp < 0)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    return -1; // Key not found
}

LYN_REPLACE_CHECK(GetItemPurchasePrice);
u16 GetItemPurchasePrice(struct Unit * unit, int item)
{
    int cost = 0;

#if CHAX
    // Check if it's a skill scroll
    if (IsDuraItem(item))
    {
        char * key = GetDuraItemName(item);
        int value = binary_search(dict, dict_size, key);
        if (value != -1)
            cost = value;
    }
    else
        cost = GetItemCost(item);
#else
    cost = GetItemCost(item);
#endif

    if (gBmSt.gameStateBits & BM_FLAG_PREPSCREEN)
        cost = cost + (cost / 2);

    if (UnitHasItem(unit, ITEM_SILVERCARD))
    {
        cost = cost / 2;
        return cost;
    }

#if (defined(SID_Bargain) && COMMON_SKILL_VALID(SID_Bargain))
    if (SkillTester(unit, SID_Bargain))
    {
        cost = Div(cost * 50, 1000);
        return cost;
    }
#endif

#if (defined(SID_Deal) && COMMON_SKILL_VALID(SID_Deal))
    if (SkillTester(unit, SID_Deal))
    {
        cost = Div(cost * 80, 1000);
        return cost;
    }
#endif

    return cost;
}

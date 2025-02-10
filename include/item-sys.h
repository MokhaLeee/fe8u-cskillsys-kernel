#pragma once

#include "common-chax.h"

typedef void (*PrepItemEffectFunc_t)(struct ProcPrepItemUse * proc, u16 item);
extern PrepItemEffectFunc_t const gPrepItemEffectFunc[0x100];

bool CanUnitUseMetiStome(struct Unit * unit, int item);
typedef bool (*PrepItemUsabilityFunc_t)(struct Unit * unit, int item);
extern PrepItemUsabilityFunc_t const gPrepItemUsabilityFuncs[0x100];

/* DuraItem */
bool IsDuraItem(int item);
char * GetDuraItemName(int item);
int GetDuraItemDescId(int item);
int GetDuraItemUseDescId(int item);
int GetDuraItemIconId(int item);

/* Misc API functions */
void PrepItemEffect_StatBooster(struct ProcPrepItemUse * proc, u16 item);
void PrepItemEffect_Promotion(struct ProcPrepItemUse * proc, u16 item);
void PrepItemEffect_JunaFruit(struct ProcPrepItemUse * proc, u16 item);

// Struct for key-value pairs
typedef struct
{
    const char * key;
    int values[2];
} KeyValuePair;

/* 
** First parameter of the outer strict is the skill name as it displays when used as a skill scroll
** Second parameter is the inner struct, containing two values: price of skill and capacity of skill
** Price of skill: How much a scroll of this skill costs
** Capacity of skill: Based on the Tellius skill system, how much of a unit's 'skill capacity' will this skill use
*/
static const KeyValuePair dict_skills[] = 
{ 
        { "Absolve", {1000, 1}},
        { "Absorb Melee", {1000, 1}},
        { "Absorb Magic", {1000, 1}},
        { "Accost", {1000, 1}},
        { "Acrobat", {1000, 1}},
        { "Adaptable", {1000, 1}},
        { "Adaptive Lunge", {1000, 1}},
        { "Adaptive Stance", {1000, 1}},
        { "Adept", {1000, 1}},
        { "Admiration", {1000, 1}},
        { "Adrenaline Rush", {1000, 1}},
        { "Advantage Chaser", {1000, 1}},
        { "Aegis", {1000, 1}},
        { "Aerobatics", {1000, 1}},
        { "Aether", {1000, 1}},
        { "Airlock", {1000, 1}},
        { "Air Raid (Atk)", {1000, 1}},
        { "Air Raid (Avo)", {1000, 1}},
        { "Air Raid (Crit)", {1000, 1}},
        { "Air Raid (Def)", {1000, 1}},
        { "Air Raid (Hit)", {1000, 1}},
        { "Air Raid (Res)", {1000, 1}},
        { "Air Raid (Spd)", {1000, 1}},
        { "Alacrity", {1000, 1}},
        { "Alert Stance", {1000, 1}},
        { "Alert Stance+", {1000, 1}},
        { "Amaterasu", {1000, 1}},
        { "Amische", {1000, 1}},
        { "Analytic", {1000, 1}},
        { "Anathema", {1000, 1}},
        { "Anchor", {1000, 1}},
        { "Anger Point", {1000, 1}},
        { "Anima Prowess", {1000, 1}},
        { "Aptitude", {1000, 1}},
        { "Arcane Blade", {1000, 1}},
        { "Arcgift", {1000, 1}},
        { "Arcgift+", {1000, 1}},
        { "Ardent Sacrifice", {1000, 1}},
        { "Arena Trap", {1000, 1}},
        { "Arise", {1000, 1}},
        { "Armored Blow", {1000, 1}},
        { "Armorboost", {1000, 1}},
        { "Armor March", {1000, 1}},
        { "Armsthrift", {1000, 1}},
        { "Assign Decoy", {1000, 1}},
        { "Assist", {1000, 1}},
        { "Astra", {1000, 1}},
        { "Aversa's Night", {1000, 1}},
        { "Avoid", {1000, 1}},
        { "Avoid Sword", {1000, 1}},
        { "Axe Break", {1000, 1}},
        { "Axefaith", {1000, 1}},
        { "Axe Prowess", {1000, 1}},
        { "Bane", {1000, 1}},
        { "Bargain", {1000, 1}},
        { "Barricade", {1000, 1}},
        { "Barricade+", {1000, 1}},
        { "Battle Range 1", {1000, 1}},
        { "Battle Range 2", {1000, 1}},
        { "Battle Range 3", {1000, 1}},
        { "Battle Veteran", {1000, 1}},
        { "Beast Assault", {1000, 1}},
        { "Biding Blow", {1000, 1}},
        { "Black Magic", {1000, 1}},
        { "Blacksmith", {1000, 1}},
        { "Bladegift", {1000, 1}},
        { "Bladegift+", {1000, 1}},
        { "Blessing", {1000, 1}},
        { "BloodTide", {1000, 1}},
        { "Blossom", {1000, 1}},
        { "Blue Flame", {1000, 1}},
        { "Blue Lion Rule", {1000, 1}},
        { "B.Mag Breaker", {1000, 1}},
        { "B.Mag RNG+1", {1000, 1}},
        { "B.Mag RNG+2", {1000, 1}},
        { "Bold Fighter", {1000, 1}},
        { "Bond", {1000, 1}},
        { "Boon", {1000, 1}},
        { "Boundless Vitality", {1000, 1}},
        { "Bow Breaker", {1000, 1}},
        { "Bow Prowess", {1000, 1}},
        { "Bow RNG+1", {1000, 1}},
        { "Bow RNG+2", {1000, 1}},
        { "Break", {1000, 1}},
        { "Breath Of Life", {1000, 1}},
        { "Bushido", {1000, 1}},
        { "Calm Initiative", {1000, 1}},
        { "Camaraderie", {1000, 1}},
        { "Canto", {1000, 1}},
        { "Canto+", {1000, 1}},
        { "Capacity-Half", {1000, 1}},
        { "Capacity-One", {1000, 1}},
        { "Capture", {1000, 1}},
        { "Casual", {1000, 1}},
        { "Catch Em All", {1000, 1}},
        { "Catching Up", {1000, 1}},
        { "Chaos Style", {1000, 1}},
        { "Charge", {1000, 1}},
        { "Charge+", {1000, 1}},
        { "Charisma", {1000, 1}},
        { "Charm", {1000, 1}},
        { "Chipper", {1000, 1}},
        { "Chivalry", {1000, 1}},
        { "Chlorophyll", {1000, 1}},
        { "Clearance", {1000, 1}},
        { "Clear Body", {1000, 1}},
        { "Clever Initiative", {1000, 1}},
        { "Close Combat", {1000, 1}},
        { "Colossus", {1000, 1}},
        { "Comatose", {1000, 1}},
        { "C-Weapon", {1000, 1}},
        { "Contrary", {1000, 1}},
        { "Corona", {1000, 1}},
        { "Corrosion", {1000, 1}},
        { "Counter", {1000, 1}},
        { "Counterattack", {1000, 1}},
        { "Counter Magic", {1000, 1}},
        { "Counter Roar", {1000, 1}},
        { "Crit", {1000, 1}},
        { "Crit Axe", {1000, 1}},
        { "Crit Bow", {1000, 1}},
        { "Critical Force", {1000, 1}},
        { "Critical Overload", {1000, 1}},
        { "Critical Pierce", {1000, 1}},
        { "Crit Lance", {1000, 1}},
        { "Crit Sword", {1000, 1}},
        { "Crusader Ward", {1000, 1}},
        { "Cultured", {1000, 1}},
        { "Dance", {1000, 1}},
        { "Dancing Blade", {1000, 1}},
        { "Dark Bargain", {1000, 1}},
        { "Dark Horse", {1000, 1}},
        { "Dark Prowess", {1000, 1}},
        { "Darting Blow", {1000, 1}},
        { "Daunt", {1000, 1}},
        { "Dazzle", {1000, 1}},
        { "Deadeye", {1000, 1}},
        { "Death Blow", {1000, 1}},
        { "Deal", {1000, 1}},
        { "Debilitator", {1000, 1}},
        { "Def+2", {1000, 1}},
        { "Defense Boost", {1000, 1}},
        { "Def Hone", {1000, 1}},
        { "Defiant Avoid", {1000, 1}},
        { "Defiant Crit", {1000, 1}},
        { "Defiant Def", {1000, 1}},
        { "Defiant Lck", {1000, 1}},
        { "Defiant Mag", {1000, 1}},
        { "Defiant Res", {1000, 1}},
        { "Defiant Skl", {1000, 1}},
        { "Defiant Spd", {1000, 1}},
        { "Defiant Str", {1000, 1}},
        { "Demoiselle", {1000, 1}},
        { "Demolition Expert", {1000, 1}},
        { "Desperation", {1000, 1}},
        { "Despoil", {1000, 1}},
        { "Destiny Bond", {1000, 1}},
        { "Detailed Report", {1000, 1}},
        { "Deft Initiative", {1000, 1}},
        { "Devil's Luck", {1000, 1}},
        { "Devil's Pact", {1000, 1}},
        { "Devil'ss Whim", {1000, 1}},
        { "Discipline", {1000, 1}},
        { "Dismount", {1000, 1}},
        { "Divinely Inspiring", {1000, 1}},
        { "Divine Pulse", {1000, 1}},
        { "Domain", {1000, 1}},
        { "D-Arm", {1000, 1}},
        { "Doppleganger", {1000, 1}},
        { "Double Lion", {1000, 1}},
        { "Double Up", {1000, 1}},
        { "Down With Arch", {1000, 1}},
        { "Dragon Fang", {1000, 1}},
        { "Dragon Skin", {1000, 1}},
        { "Dragon Wall", {1000, 1}},
        { "Dragon Warth", {1000, 1}},
        { "DrawBack", {1000, 1}},
        { "Drive Def", {1000, 1}},
        { "Drive Mag", {1000, 1}},
        { "Drive Res", {1000, 1}},
        { "Drive Spd", {1000, 1}},
        { "Drive Str", {1000, 1}},
        { "Drizzle", {1000, 1}},
        { "Drought", {1000, 1}},     
        { "Duel", {1000, 1}},   
        { "Duelist Blow", {1000, 1}},
        { "Earth Boost", {1000, 1}},
        { "Echo", {1000, 1}},
        { "Eclipse", {1000, 1}},
        { "Effect Spore", {1000, 1}},
        { "Elbow Room", {1000, 1}},
        { "Enrage", {1000, 1}},
        { "Entrepreneur", {1000, 1}},
        { "Even Footed", {1000, 1}},
        { "Even Rhythm", {1000, 1}},
        { "Expertise", {1000, 1}},
        { "E-Experience", {1000, 1}},
        { "Faerghus-A", {1000, 1}},
        { "Fail-Gale", {1000, 1}},
        { "Faire Anima", {1000, 1}},
        { "Faire Axe", {1000, 1}},
        { "Faire B.Mag", {1000, 1}},
        { "Faire Bow", {1000, 1}},
        { "Faire Dark", {1000, 1}},
        { "Faire Lance", {1000, 1}},
        { "Faire Light", {1000, 1}},
        { "Faire Sword", {1000, 1}},
        { "F-T Folk", {1000, 1}},        
        { "Fiendish Blow", {1000, 1}},
        { "Fiery Blood", {1000, 1}},
        { "Fire Boost", {1000, 1}},
        { "Flank", {1000, 1}},
        { "Flare", {1000, 1}},
        { "Flashing Blade", {1000, 1}},
        { "Flashing Blade+", {1000, 1}},
        { "Flier Formation", {1000, 1}},
        { "Flier Guidance", {1000, 1}},
        { "Fluffy", {1000, 1}},
        { "Focus", {1000, 1}},
        { "Forager", {1000, 1}},
        { "F-Princess", {1000, 1}},
        { "Foresight", {1000, 1}},
        { "Fortify Armor", {1000, 1}},
        { "Fortifying Staff", {1000, 1}},
        { "Fortress Def", {1000, 1}},
        { "Fortress Res", {1000, 1}},
        { "Fortune", {1000, 1}},
        { "Frantic Swing", {1000, 1}},
        { "Frenzy", {1000, 1}},
        { "Full Metal Body", {1000, 1}},
        { "Fury", {1000, 1}},
        { "Fury+", {1000, 1}},
        { "Galeforce", {1000, 1}},
        { "Gale Wings", {1000, 1}},
        { "Gambit", {1000, 1}},
        { "Gentilhomme", {1000, 1}},
        { "Gentle Flower", {1000, 1}},
        { "Glacies", {1000, 1}},
        { "Goddess Dance", {1000, 1}},
        { "Gold Digger", {1000, 1}},
        { "Good As Gold", {1000, 1}},
        { "Goody Basket", {1000, 1}}, 
        { "Gracegift", {1000, 1}},
        { "Gracegift+", {1000, 1}},
        { "Grazing Blow", {1000, 1}},
        { "Great Shield", {1000, 1}},
        { "Grid Master", {1000, 1}},
        { "Grid Master Atk", {1000, 1}},
        { "Grisly Wound", {1000, 1}},
        { "Guard Bearing", {1000, 1}},
        { "Guardian", {1000, 1}},
        { "Guidance", {1000, 1}},
        { "Guts", {1000, 1}},
        { "Hackgift", {1000, 1}},
        { "Hackgift+", {1000, 1}},
        { "Hawkeye", {1000, 1}},
        { "Haze Hunter", {1000, 1}},
        { "Healing Focus", {1000, 1}},
        { "Heavy Blade", {1000, 1}},
        { "Heavy Blade+", {1000, 1}},
        { "Heavy Metal", {1000, 1}},
        { "Heavy Strikes", {1000, 1}},
        { "Hero", {1000, 1}},
        { "Hex", {1000, 1}},
        { "High Ground", {1000, 1}},
        { "Hoarders Bane", {1000, 1}},
        { "Holy Aura", {1000, 1}},
        { "Hone Armor", {1000, 1}},
        { "Hone Cavalry", {1000, 1}},
        { "Hone Chaos", {1000, 1}},
        { "Hone Flier", {1000, 1}},
        { "Horse guard", {1000, 1}},
        { "Hp+5", {1000, 1}},
        { "Hyper Cutter", {1000, 1}},
        { "Hyper Focus", {1000, 1}},
        { "I-Problems", {1000, 1}},
        { "Ignis", {1000, 1}},
        { "Imbue", {1000, 1}},
        { "Impale", {1000, 1}},
        { "Indoor March", {1000, 1}},
        { "Infiltrator", {1000, 1}},
        { "Infinity Edge", {1000, 1}},
        { "Inori", {1000, 1}},
        { "Insomnia", {1000, 1}},
        { "Inspiration", {1000, 1}},
        { "Intimidate", {1000, 1}},
        { "Keen Fighter", {1000, 1}},
        { "Keep Up", {1000, 1}},
        { "Killing Blow", {1000, 1}},
        { "Killing Machine", {1000, 1}},
        { "Kill Streak", {1000, 1}},
        { "Knight Aspirant", {1000, 1}},
        { "Lady Blade", {1000, 1}},
        { "Lance Breaker", {1000, 1}},
        { "Lance Prowess", {1000, 1}},
        { "Last Word", {1000, 1}},
        { "Luck+2", {1000, 1}},
        { "Luck Hone", {1000, 1}},
        { "Lead By Example", {1000, 1}},
        { "Legend Inori ATK:", {1000, 1}},
        { "Legend Inori AVO", {1000, 1}},
        { "Legend Inori DEF", {1000, 1}},
        { "Lethality", {1000, 1}},
        { "Life And Death", {1000, 1}},
        { "Lifetaker", {1000, 1}},
        { "Light And Dark", {1000, 1}},
        { "Light Prowess", {1000, 1}},
        { "Light Rune", {1000, 1}},
        { "Light Weight", {1000, 1}},
        { "Lilys Poise", {1000, 1}},
        { "Limit Breaker", {1000, 1}},
        { "Limit Breaker+", {1000, 1}},
        { "Liquid Ooze", {1000, 1}},
        { "Live To Serve", {1000, 1}},
        { "Lock Touch", {1000, 1}},
        { "Loyalty", {1000, 1}},
        { "Luck Boost", {1000, 1}},
        { "Lucky Initiative", {1000, 1}},
        { "Lucky Seven", {1000, 1}},
        { "Ludopathy", {1000, 1}},
        { "Lull Defense", {1000, 1}},
        { "Lull Luck", {1000, 1}},
        { "Lull Magic", {1000, 1}},
        { "Lull Movement", {1000, 1}},
        { "Lull Resistance", {1000, 1}},
        { "Lull Skill", {1000, 1}},
        { "Lull Spectrum", {1000, 1}},
        { "Lull Speed", {1000, 1}},
        { "Lull Strength", {1000, 1}},
        { "Lumina", {1000, 1}},
        { "Lumina+", {1000, 1}},
        { "Luna", {1000, 1}},
        { "Luna Attack", {1000, 1}},
        { "Lunar Brace", {1000, 1}},
        { "Lunge", {1000, 1}},
        { "Mag+2", {1000, 1}},
        { "Mage Slayer", {1000, 1}},
        { "Mag Hone", {1000, 1}},
        { "Magic Boost", {1000, 1}},
        { "Magic Bounce", {1000, 1}},
        { "Magic Eye", {1000, 1}},
        { "Magic Gambit", {1000, 1}},
        { "Magic Seal", {1000, 1}},
        { "Make A Killing", {1000, 1}},
        { "Malefic Aura", {1000, 1}},
        { "Mantle", {1000, 1}},
        { "Melee Maniac", {1000, 1}},
        { "Mend Arms", {1000, 1}},
        { "Mentorship", {1000, 1}},
        { "Merciless", {1000, 1}},
        { "Mercurious", {1000, 1}},
        { "Mercy", {1000, 1}},
        { "Mercy+", {1000, 1}},
        { "Mimic", {1000, 1}},
        { "Mine", {1000, 1}},
        { "Minus", {1000, 1}},
        { "Moody", {1000, 1}},
        { "Moon Bow", {1000, 1}},
        { "Moonlight", {1000, 1}},
        { "Mountain Climber", {1000, 1}},
        { "Mountain Climber+", {1000, 1}},
        { "Mov+2", {1000, 1}},
        { "Move Boost", {1000, 1}},
        { "Mov Hone", {1000, 1}},
        { "Multiscale", {1000, 1}},
        { "Mystic Boost", {1000, 1}},
        { "Natural Cover", {1000, 1}},
        { "Nature Rush", {1000, 1}},
        { "Necro Copy", {1000, 1}},
        { "Nice Thighs", {1000, 1}},
        { "Night Tide", {1000, 1}},
        { "Nihil", {1000, 1}},
        { "Nimble Initiative", {1000, 1}},
        { "No Guard", {1000, 1}},
        { "Nonconforming", {1000, 1}},
        { "Nullify", {1000, 1}},
        { "Ogre Body", {1000, 1}},
        { "O-Defense", {1000, 1}},
        { "O-Luck", {1000, 1}},
        { "O-Magic", {1000, 1}},
        { "O-Movement", {1000, 1}},
        { "O-Resistance", {1000, 1}},
        { "O-Skill", {1000, 1}},
        { "O-Speed", {1000, 1}},
        { "O-Strength", {1000, 1}},
        { "Obstruct", {1000, 1}},
        { "Odd Footed", {1000, 1}},
        { "Odd Rhythm", {1000, 1}},
        { "Onimaru", {1000, 1}},
        { "Opportunist", {1000, 1}},
        { "Out-Fighter", {1000, 1}},
        { "Outrider", {1000, 1}},
        { "Pair Up", {1000, 1}},
        { "Paragon", {1000, 1}},
        { "Pass", {1000, 1}},
        { "P-Flow", {1000, 1}},
        { "P-Veil", {1000, 1}},
        { "Pathfinder", {1000, 1}},
        { "Patience", {1000, 1}},
        { "Pavise", {1000, 1}},
        { "Peacebringer", {1000, 1}},
        { "Perfectionist", {1000, 1}},
        { "Petrify", {1000, 1}},
        { "Pickup", {1000, 1}},
        { "Piercegift", {1000, 1}},
        { "Piercegift+", {1000, 1}},
        { "Pivot", {1000, 1}},
        { "Plus", {1000, 1}},
        { "Point Blank", {1000, 1}},
        { "Poise", {1000, 1}},
        { "Poison Heal", {1000, 1}},
        { "Poison Point", {1000, 1}},
        { "P-Strike", {1000, 1}},
        { "Pos-Return", {1000, 1}},
        { "Power Staff", {1000, 1}},
        { "Pow Hone", {1000, 1}},
        { "Pragmatic", {1000, 1}},
        { "Prescience", {1000, 1}},
        { "P-Warrior", {1000, 1}},
        { "Prodigy", {1000, 1}},
        { "Provoke", {1000, 1}},
        { "PsychUp", {1000, 1}},
        { "Puissance", {1000, 1}},
        { "Pursuer", {1000, 1}},
        { "Pursuit", {1000, 1}},
        { "P-Defense", {1000, 1}},
        { "P-Luck", {1000, 1}},
        { "P-Magic", {1000, 1}},
        { "P-Movement", {1000, 1}},
        { "P-Resistance", {1000, 1}},
        { "P-Skill", {1000, 1}},
        { "P-Spectrum", {1000, 1}},
        { "P-Speed", {1000, 1}},
        { "P-Strength", {1000, 1}},
        { "Quick Burn", {1000, 1}},
        { "Quick Draw", {1000, 1}},
        { "Quick Hands", {1000, 1}},   
        { "Quick Initiative", {1000, 1}},        
        { "Quick Learner", {1000, 1}},
        { "Quick Riposte", {1000, 1}},
        { "R-Aid", {1000, 1}},
        { "Rain Dish", {1000, 1}},
        { "Rally-Def", {1000, 1}},
        { "Rally-Lck", {1000, 1}},
        { "Rally-Mag", {1000, 1}},
        { "Rally-Mov", {1000, 1}},
        { "Rally-Res", {1000, 1}},
        { "Rally-Skl", {1000, 1}},
        { "Rally-Spec", {1000, 1}},
        { "Rally-Spd", {1000, 1}},
        { "Rally-Str", {1000, 1}},
        { "Rampage", {1000, 1}},
        { "Ramp Up", {1000, 1}},
        { "R-Fighter", {1000, 1}},
        { "Reflex", {1000, 1}},
        { "Relief", {1000, 1}},
        { "Renewal", {1000, 1}},
        { "Reposition", {1000, 1}},
        { "ResBonus", {1000, 1}},
        { "ResHone", {1000, 1}},
        { "Res-Boost", {1000, 1}},
        { "Resolve", {1000, 1}},
        { "Resourceful", {1000, 1}},
        { "Resurrection", {1000, 1}},
        { "Rightful Arch", {1000, 1}},
        { "Rightful God", {1000, 1}},
        { "Rightful King", {1000, 1}},
        { "Risk It All", {1000, 1}},
        { "Roll 12", {1000, 1}},
        { "R-Defense", {1000, 1}},
        { "R-Luck", {1000, 1}},
        { "R-Magic", {1000, 1}},
        { "R-Movement", {1000, 1}},
        { "R-Resistance", {1000, 1}},
        { "R-Skill", {1000, 1}},
        { "R-Speed", {1000, 1}},
        { "R-Strength", {1000, 1}},
        { "Ruined Blade", {1000, 1}},
        { "Ruined Blade+", {1000, 1}},
        { "R-Start", {1000, 1}},
        { "Sacrifice", {1000, 1}},
        { "Sand Rush", {1000, 1}},
        { "Sand Stream", {1000, 1}},
        { "Savage Blow", {1000, 1}},
        { "S-Defense", {1000, 1}},
        { "S-Luck", {1000, 1}},
        { "S-Magic", {1000, 1}},
        { "S-Resistance", {1000, 1}},
        { "S-Skill", {1000, 1}},
        { "S-Speed", {1000, 1}},
        { "S-Strength", {1000, 1}},
        { "Secret", {1000, 1}},
        { "Seer", {1000, 1}},
        { "Self Healing", {1000, 1}},
        { "Shade", {1000, 1}},
        { "Shade+", {1000, 1}},
        { "Shadowgift", {1000, 1}},
        { "Shadowgift+", {1000, 1}},
        { "Short Shield", {1000, 1}},
        { "Shove", {1000, 1}},
        { "Shrewd-P", {1000, 1}},
        { "Silent Pride", {1000, 1}},
        { "Skill Boost", {1000, 1}},
        { "Skl+2", {1000, 1}},
        { "Skl Hone", {1000, 1}},
        { "Skybreaker", {1000, 1}},
        { "Skyguard", {1000, 1}},
        { "Slayer", {1000, 1}},
        { "Slow Burn", {1000, 1}},
        { "Slush Rush", {1000, 1}},
        { "Smite", {1000, 1}},
        { "Snow Warning", {1000, 1}},
        { "S-Guidance", {1000, 1}},
        { "Soar-Wings", {1000, 1}},
        { "S-Butterfly", {1000, 1}},
        { "Sol", {1000, 1}},
        { "Solar Power", {1000, 1}},
        { "Solidarity", {1000, 1}},
        { "Solid Rock", {1000, 1}},
        { "Sorcery Blade", {1000, 1}},
        { "Soul Sap", {1000, 1}},
        { "Spd+2", {1000, 1}},
        { "Spd Hone", {1000, 1}},
        { "Spec-Boost", {1000, 1}},
        { "Spec-Init", {1000, 1}},
        { "Speed Boost", {1000, 1}},
        { "Spur Def", {1000, 1}},
        { "Spur Mag", {1000, 1}},
        { "Spurn", {1000, 1}},
        { "Spur Res", {1000, 1}},
        { "Spur Spd", {1000, 1}},
        { "Spur Str", {1000, 1}},
        { "Staff Guard", {1000}},
        { "Staff Paragon", {1000, 1}},
        { "Staff Savant", {1000, 1}},
        { "S-Bracing", {1000, 1}},
        { "S-Darting", {1000, 1}},
        { "S-Fierce", {1000, 1}},
        { "S-Kestrel", {1000, 1}},
        { "S-Mirror", {1000, 1}},
        { "S-Ready", {1000, 1}},
        { "S-Steady", {1000, 1}},
        { "S-Sturdy", {1000, 1}},
        { "S-Swift", {1000, 1}},
        { "S-Warding", {1000, 1}},
        { "Steady-B", {1000, 1}},
        { "Steal", {1000, 1}},
        { "Steal+", {1000, 50}},
        { "Stone Body", {1000, 1}},
        { "Stormgift", {1000, 1}},
        { "Stormgift+", {1000, 1}},
        { "Str+2", {1000, 1}},
        { "Str-Boost", {1000, 1}},
        { "Stride", {1000, 1}},
        { "Strong Con", {1000, 1}},
        { "Strong-Init", {1000, 1}},
        { "S-Riposte", {1000, 1}},
        { "Stun-Smile", {1000, 1}},
        { "Sturdy", {1000, 1}},
        { "Sturdy-Init", {1000, 1}},
        { "Summon", {1000, 1}},
        { "Super Luck", {1000, 1}},
        { "Supply", {1000, 1}},
        { "Sure Shot", {1000, 1}},
        { "Survivor", {1000, 1}},
        { "Swap", {1000, 1}},
        { "Swarm", {1000, 1}},
        { "Swarp", {1000, 1}},
        { "Swift Swim", {1000, 1}},
        { "Sword Breaker", {1000, 1}},
        { "Sword Prowess", {1000, 1}},
        { "Synchronize", {1000, 1}},
        { "Synergism", {1000, 1}},
        { "Taker Defense", {1000, 1}},
        { "Tantivy", {1000, 1}},
        { "Technician", {1000, 1}},
        { "Teleportation", {1000, 1}},
        { "Thighdeology", {1000, 1}},
        { "Thot Slayer", {1000, 1}},
        { "Thunderstorm", {1000, 1}},
        { "Tower Shield", {1000, 1}},
        { "Tower Shield+", {1000, 1}},
        { "Trader", {1000, 1}},
        { "Trample", {1000, 1}},
        { "Transform", {1000, 1}},
        { "T-Adept", {1000, 1}},
        { "T-Adept+", {1000, 1}},
        { "Tri-Attack", {1000, 1}},
        { "Triple Up", {1000, 1}},
        { "Trip Up", {1000, 1}},
        { "Tsuzuku", {1000, 1}},
        { "Turncoat", {1000, 1}},
        { "Unaware", {1000, 1}},
        { "Uncanny Blow", {1000, 1}},
        { "Up With Arch", {1000, 1}},
        { "Vanity", {1000, 1}},
        { "Vantage", {1000, 1}},
        { "Velocity", {1000, 1}},
        { "Vengeance", {1000, 1}},
        { "V-Fighter", {1000, 1}},
        { "Vigilance", {1000, 1}},
        { "Vigor Dance", {1000, 1}},
        { "Voice Of Peace", {1000, 1}},
        { "Void Curse", {1000, 1}},
        { "Warding Blow", {1000, 1}},
        { "W-Fighter", {1000, 1}},
        { "Watchful Eye", {1000, 1}},
        { "Water Boost", {1000, 1}},
        { "Water Walking", {1000, 1}},
        { "Water Walking+", {1000, 1}},
        { "White Pool", {1000, 1}},
        { "Wind Boost", {1000, 1}},
        { "Wind Disciple", {1000, 1}},
        { "Wonder Guard", {1000, 1}},
        { "Wrath", {1000, 1}},
        { "Wrestler", {1000, 1}},
        { "Yune Whispers", {1000, 1}}
};

// int compare(const void * a, const void * b);
// int lookup(KeyValuePair * array, int size, const char * key);
int binary_search_skills(const KeyValuePair *array, int size, const char *key, int value);
int getArraySize(const KeyValuePair *array);

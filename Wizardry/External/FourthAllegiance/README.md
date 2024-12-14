# FE8U-Fourth-Allegiance
 A hack that adds a fourth allegiance to FE8U, Based on the 4th army used by the link arena. Note that this hack is incompatible with the skill system family of engine hacks.
# How to Install

## Non-buildfile users
 Requires [Event Assembler](https://feuniverse.us/t/event-assembler/1749).
 You will need to install this engine hack into your ROM's Free Space. To do that, add ORG (YourFreeSpaceOffset) at the top of MasterAsmInstaller.event. 
 Then, use event assembler to assemble MasterAsmInstaller.event into your rom, and you should be good to go.
 
## Buildfile users
 Simply include MasterAsmInstaller.event into your buildfile, and everything should be good to go
 
# Usage

## Adding 4th allegiance units
 To add units of the 4th allegiance, simply set the faction in your unit definition to 3. In FEBuilder, that should appear as "Defeat".

## Setting Faction Relations per chapter
 Faction relations are handled via a bitmap. The short at +0x26 in the chapter data table is said bitmap. (Under "Enemy Phase BGM Flag 4" for FEBuilder users.)
 This hack automatically sets all chapters to have the vanilla relations for Blue, Red and Green units, and has Purple units be hostile to everyone.
 There are 4 bitmaps, in the order Blue-Green-Red-Purple; one per faction, and then each bit sets if you are allied or enemy to a faction, in the same order. Edit that bitmap to set your relations however you'd like.
 
## Setting Map music for the fourth faction
 Short 0x1C in the chapter data table is map music for the fourth phase("Player Phase BGM 2" for FEBuilder users), while short 0x1E is map music for the fourth phase when flag 4 is set("Enemy Phase BGM 2" for FEBuilder users).
 